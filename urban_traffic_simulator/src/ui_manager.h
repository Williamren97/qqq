#ifndef __UI_MANAGER__H
#define __UI_MANAGER__H 

#include "enviro.h"
#include "road_network.h"
#include <vector>
#include <map>
#include <string>
#include <functional>

using namespace enviro;

//! UIManager - Handles user interactions with the simulation
class UIManager : public Agent {

    public:
        //! Constructor for UI Manager
        UIManager(json spec, World& world);
        
        //! Initialize the UI manager
        void init() override;
        
        //! Update method called by the ENVIRO framework
        void update() override;
        
        //! Set the road network reference
        //! \param network Pointer to the road network
        void set_road_network(RoadNetwork* network);
        
        //! Handle mouse click events
        //! \param x X-coordinate of click
        //! \param y Y-coordinate of click
        //! \param button Button that was clicked (0 = left, 1 = middle, 2 = right)
        void handle_click(double x, double y, int button);
        
        //! Add a new car at the specified location
        //! \param x X-coordinate
        //! \param y Y-coordinate
        //! \return ID of the newly created car
        int add_car(double x, double y);
        
        //! Set destination for a car
        //! \param car_id ID of the car agent
        //! \param x X-coordinate of destination
        //! \param y Y-coordinate of destination
        void set_car_destination(int car_id, double x, double y);
        
        //! Toggle manual traffic light control mode
        void toggle_traffic_light_control();
        
        //! Reset the simulation
        void reset_simulation();

    private:
        //! Reference to the ENVIRO world
        World& _world;
        
        //! Pointer to the road network
        RoadNetwork* _road_network;
        
        //! ID of the last added car
        int _last_car_id;
        
        //! State of UI interaction
        enum class UIState {
            NORMAL,
            ADDING_CAR,
            SETTING_DESTINATION,
            CONTROLLING_TRAFFIC_LIGHTS
        };
        
        UIState _ui_state;
        
        //! Map of button names to callback functions
        std::map<std::string, std::function<void()>> _button_callbacks;
        
        //! Whether the simulation is paused
        bool _paused;
        
        //! Add UI elements (buttons, info panels)
        void add_ui_elements();
        
        //! Handle button click events
        //! \param button_name Name of the button that was clicked
        void handle_button_click(const std::string& button_name);
        
        //! Update UI status
        void update_status();
};

#endif