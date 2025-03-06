#ifndef __TRAFFIC_LIGHT_AGENT__H
#define __TRAFFIC_LIGHT_AGENT__H 

#include "enviro.h"

using namespace enviro;

//! TrafficLight - Controls the flow of traffic at intersections
class TrafficLight : public Agent {

    public:
        //! Constructor for Traffic Light agent
        TrafficLight(json spec, World& world);
        
        //! Initialize the traffic light
        void init() override;
        
        //! Update method called by the ENVIRO framework
        void update() override;
        
        //! Manually change the traffic light state
        //! \param new_state The new state to set (0 = red, 1 = yellow, 2 = green)
        void set_state(int new_state);
        
        //! Get the current state of the traffic light
        //! \return The current state (0 = red, 1 = yellow, 2 = green)
        int get_state() const;
        
        //! Returns whether the light is currently red
        //! \return True if the light is red, false otherwise
        bool is_red() const;
        
        //! Returns whether the light is currently yellow
        //! \return True if the light is yellow, false otherwise
        bool is_yellow() const;
        
        //! Returns whether the light is currently green
        //! \return True if the light is green, false otherwise
        bool is_green() const;

    private:
        //! Apply visual changes based on current state
        void update_visuals();
        
        //! Traffic Light States
        enum class State {
            RED = 0,
            YELLOW = 1,
            GREEN = 2
        };
        
        State _current_state;
        double _state_duration;
        double _time_in_state;
        
        // Timing configuration for each state (in seconds)
        static constexpr double RED_DURATION = 10.0;
        static constexpr double YELLOW_DURATION = 3.0;
        static constexpr double GREEN_DURATION = 10.0;
};

#endif