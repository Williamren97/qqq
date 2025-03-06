#ifndef __CAR_AGENT__H
#define __CAR_AGENT__H 

#include "enviro.h"
#include <cmath>
#include <vector>

using namespace enviro;

// Structure to represent a point
struct Point {
    double x;
    double y;
    Point(double _x, double _y) : x(_x), y(_y) {}
    
    double distance_to(double px, double py) const {
        return sqrt((x - px) * (x - px) + (y - py) * (y - py));
    }
};

class CarController : public Process, public AgentInterface {

    public:
    CarController() : Process(), AgentInterface() {}

    void init() {
        // Store initial position for reset functionality
        initial_x = x();
        initial_y = y();
        initial_theta = angle();

        watch("keydown", [this](Event e) {
            auto k = e.value()["key"].get<std::string>();
            if ( k == "w" ) {
                v = v_max;
                autonomous_mode = false;
            } else if ( k == "s" ) {
                v = -v_max;
                autonomous_mode = false;
            } else if ( k == "a" ) {
                omega = -omega_max;
                autonomous_mode = false;
            } else if ( k == "d" ) {
                omega = omega_max;
                autonomous_mode = false;
            } else if ( k == "r" ) {
                // Reset car position
                reset_position();
            } else if ( k == "n" ) {
                // Set new random destination
                set_random_destination();
                autonomous_mode = true;
            }
        });

        watch("keyup", [this](Event e) {
            auto k = e.value()["key"].get<std::string>();
            if ( k == "w" || k == "s" ) {
                v = 0;
            } else if ( k == "a" || k == "d" ) {
                omega = 0;
            }
        });

        // Watch for reset event
        watch("reset_cars", [this](Event e) {
            reset_position();
            e.stop_propagation();
        });
    }

    void update() {
        try {
            if (autonomous_mode && has_destination) {
                navigate_to_destination();
            }
            
            // Check front sensor for obstacles
            if (sensor_value(0) < 40 && v > 0) {
                // Check if it's a wall by testing sensor reflection type
                std::string reflection = sensor_reflection_type(0);
                
                if (reflection == "STATIC") {
                    // It's a wall, make a random turn
                    v = 0; // Stop first
                    if (rand() % 2 == 0) {
                        // Turn left
                        omega = -omega_max;
                        turning_duration = 40; // About 90 degrees at omega_max
                        turning = true;
                    } else {
                        // Turn right
                        omega = omega_max;
                        turning_duration = 40;
                        turning = true;
                    }
                    decorate("<circle cx='0' cy='0' r='5' style='fill: yellow'></circle>");
                } else {
                    // Regular obstacle, just stop
                    v = 0;
                    std::cout << "Stopping due to obstacle ahead\n";
                    decorate("<circle cx='0' cy='0' r='5' style='fill: red'></circle>");
                }
            } else if (turning) {
                // Continue turning until duration is complete
                turning_duration--;
                if (turning_duration <= 0) {
                    turning = false;
                    omega = 0;
                    v = v_max; // Resume forward motion
                    decorate(""); // Remove decoration
                }
            } else {
                // Normal driving - add a visual indicator
                if (has_destination) {
                    // Show destination marker
                    std::string marker = "<g><circle cx='0' cy='0' r='3' style='fill: green'></circle>";
                    marker += "<line x1='0' y1='0' x2='" + std::to_string(cos(angle_to_destination) * 10) + 
                             "' y2='" + std::to_string(sin(angle_to_destination) * 10) + 
                             "' style='stroke: green; stroke-width: 2'/></g>";
                    decorate(marker);
                } else {
                    decorate("<circle cx='0' cy='0' r='3' style='fill: green'></circle>");
                }
            }
            
            try {
                track_velocity(v, omega);
            } catch (const std::exception& e) {
                std::cout << "Exception in track_velocity: " << e.what() << std::endl;
                reset_position();
            }
        } catch (const std::exception& e) {
            std::cout << "Exception in update: " << e.what() << std::endl;
            reset_position();
        }
    }

    void reset_position() {
        try {
            teleport(initial_x, initial_y, initial_theta);
            v = 0;
            omega = 0;
            turning = false;
            has_destination = false;
            std::cout << "Car reset to initial position\n";
        } catch (const std::exception& e) {
            std::cout << "Exception in reset_position: " << e.what() << std::endl;
            // As a fallback, just place the car at origin
            teleport(0, 0, 0);
        }
    }
    
    void set_random_destination() {
        int idx = rand() % destinations.size();
        current_destination = destinations[idx];
        has_destination = true;
        std::cout << "Set new destination: (" << current_destination.x << ", " 
                  << current_destination.y << ")" << std::endl;
    }
    
    void navigate_to_destination() {
        // Calculate distance to destination
        double dx = current_destination.x - x();
        double dy = current_destination.y - y();
        double distance = sqrt(dx*dx + dy*dy);
        
        // If we're close enough, stop and consider it reached
        if (distance < 20) {
            v = 0;
            omega = 0;
            has_destination = false;
            std::cout << "Destination reached!\n";
            decorate("<circle cx='0' cy='0' r='8' style='fill: blue'></circle>");
            return;
        }
        
        // Calculate angle to destination
        angle_to_destination = atan2(dy, dx);
        
        // Calculate difference between current angle and angle to destination
        double angle_diff = angle_to_destination - angle();
        
        // Normalize angle difference to [-pi, pi]
        while (angle_diff > M_PI) angle_diff -= 2*M_PI;
        while (angle_diff < -M_PI) angle_diff += 2*M_PI;
        
        // Orient towards destination
        if (abs(angle_diff) > 0.1) {
            v = 0; // Stop while turning
            omega = angle_diff > 0 ? omega_max : -omega_max;
        } else {
            // We're oriented correctly, move forward
            v = v_max;
            omega = 0;
        }
        
        // Safety: check for obstacles and traffic lights
        if (sensor_value(0) < 30) {
            v = 0; // Stop if something is in the way
            
            // Try to navigate around by turning
            if (!turning) {
                turning = true;
                turning_duration = 30;
                omega = (rand() % 2 == 0) ? omega_max : -omega_max;
            }
        }
    }

    void start() {
        // Initialize destinations
        destinations = {
            Point(-150, 0),
            Point(150, 0),
            Point(0, -150),
            Point(0, 150),
            Point(-250, -250),
            Point(250, 250),
            Point(-250, 250),
            Point(250, -250)
        };
        
        // Set autonomous mode off by default
        autonomous_mode = false;
    }
    
    void stop() {}

    private:
    double v = 0;
    double omega = 0;
    double initial_x = 0;
    double initial_y = 0; 
    double initial_theta = 0;
    bool turning = false;
    int turning_duration = 0;
    const double v_max = 30.0;
    const double omega_max = 1.0;
    
    // Destination navigation
    std::vector<Point> destinations;
    Point current_destination = Point(0, 0);
    bool has_destination = false;
    double angle_to_destination = 0;
    bool autonomous_mode = false;
};

class Car : public Agent {
    public:
    Car(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    CarController c;
};

DECLARE_INTERFACE(Car)

#endif