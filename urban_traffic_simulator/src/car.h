#ifndef __CAR_AGENT__H
#define __CAR_AGENT__H 

#include "enviro.h"
#include <vector>
#include <queue>

using namespace enviro;

//! Car Agent - An autonomous vehicle that navigates the city grid
class Car : public Agent {

    public:
        //! Constructor for Car agent
        Car(json spec, World& world);

        //! This method is called by the ENVIRO framework to initialize the agent
        void init() override;

        //! Update is called repeatedly to move and navigate the car
        void update() override;

        //! Sets a new destination for the car
        //! \param x The x-coordinate of the destination
        //! \param y The y-coordinate of the destination
        void set_destination(double x, double y);

        //! Gets the current state of the car
        //! \return A string representing the car's state (e.g., "driving", "stopped", etc.)
        std::string get_state() const;

    private:
        // Movement control methods
        void drive();
        void stop();
        void turn(double angle);
        
        // Navigation methods
        void find_path();
        void update_waypoints();
        bool at_waypoint() const;
        bool detect_obstacles();
        bool detect_traffic_light();
        
        // Car state
        enum class State {
            IDLE,
            DRIVING,
            STOPPED,
            TURNING,
            WAITING
        };
        
        State _state;
        
        // Navigation properties
        cpVect _destination;
        std::vector<cpVect> _path;
        int _current_waypoint;
        
        // Car dynamics
        double _desired_speed;
        double _max_speed;
        double _min_distance;
        double _stopping_distance;
        
        // Sensors
        double _front_sensor;
        double _left_sensor;
        double _right_sensor;
        double _rear_sensor;
};

#endif