#ifndef __SIMULATOR_AGENT__H
#define __SIMULATOR_AGENT__H 

#include "enviro.h"
#include <random>

using namespace enviro;

class SimulatorController : public Process, public AgentInterface {

    public:
    SimulatorController() : Process(), AgentInterface() {}

    void init() {
        // Listen for add car button clicks
        watch("add_car", [this](Event e) {
            add_random_car();
            e.stop_propagation();
        });
        
        // Listen for toggle light button clicks
        watch("toggle_light", [this](Event e) {
            std::cout << "Toggle lights button clicked" << std::endl;
            toggle_all_lights();
            e.stop_propagation();
        });
        
        // Listen for reset cars button clicks
        watch("reset_cars", [this](Event e) {
            reset_all_cars();
            e.stop_propagation();
        });
        
        // Listen for screen clicks to add car at specific position
        watch("screen_click", [this](Event e) {
            double x = e.value()["x"];
            double y = e.value()["y"];
            
            // Snap to nearest road
            snap_to_road(x, y);
            
            add_car_at(x, y);
            e.stop_propagation();
        });
        
        // Listen for keyboard events
        watch("keydown", [this](Event e) {
            std::string key = e.value()["key"];
            if (key == "c" || key == "C") {
                add_random_car();
            } else if (key == "t" || key == "T") {
                toggle_all_lights();
            } else if (key == "R") {
                reset_all_cars();
            }
            e.stop_propagation();
        });

        // Display instructions in the upper right corner
        show_instructions();
    }

    void show_instructions() {
        // Create instruction text at top right
        label("CONTROL INSTRUCTIONS", 300, -350);
        label("W/A/S/D - Control selected car", 300, -330);
        label("C - Add new random car", 300, -315);
        label("T - Toggle all traffic lights", 300, -300);
        label("R - Reset all cars", 300, -285);
        label("r - Reset selected car", 300, -270);
        label("n - Set random destination", 300, -255);
        label("Click road - Add car at position", 300, -240);
    }

    void toggle_all_lights() {
        emit(Event("toggle_light"));
    }
    
    void reset_all_cars() {
        emit(Event("reset_cars"));
        std::cout << "Reset all cars to initial positions\n";
    }

    void start() {}
    void update() {}
    void stop() {}

    private:
    void add_random_car() {
        // Road point list - positions further from walls
        std::vector<std::pair<double, double>> road_points = {
            {-120, 0}, {120, 0}, {0, -120}, {0, 120}
        };
        
        int idx = rand() % road_points.size();
        auto point = road_points[idx];
        
        add_car_at(point.first, point.second);
    }
    
    void add_car_at(double x, double y) {
        try {
            // Determine orientation based on position
            double theta = 0;
            
            // Horizontal road
            if (abs(y) < 10) {
                if (x < 0) theta = 0; // Left side moving right
                else theta = M_PI; // Right side moving left
            } 
            // Vertical road
            else {
                if (y < 0) theta = M_PI/2; // Top side moving down
                else theta = -M_PI/2; // Bottom side moving up
            }
            
            // Random color
            std::vector<std::string> colors = {"blue", "red", "green", "orange", "purple"};
            std::string color = colors[rand() % colors.size()];
            
            // Build style JSON
            json style = {
                {"fill", color},
                {"stroke", "black"}
            };
            
            // Add car agent
            add_agent("Car", x, y, theta, style);
            
            std::cout << "Added car at (" << x << ", " << y << ")\n";
        } catch (const std::exception& e) {
            std::cout << "Exception in add_car_at: " << e.what() << std::endl;
        }
    }
    
    void snap_to_road(double& x, double& y) {
        // Horizontal road
        if (abs(y) < abs(x)) {
            y = 0;
        } 
        // Vertical road
        else {
            x = 0;
        }
    }
};

class Simulator : public Agent {
    public:
    Simulator(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    SimulatorController c;
};

DECLARE_INTERFACE(Simulator)

#endif