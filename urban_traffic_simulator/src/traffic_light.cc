#include "traffic_light.h"
#include <cmath>
#include <iostream>

using namespace enviro;

// Constructor for Traffic Light
TrafficLight::TrafficLight(json spec, World& world) : Agent(spec, world) {
    _current_state = State::RED;
    _state_duration = RED_DURATION;
    _time_in_state = 0;
}

// Initialize the traffic light
void TrafficLight::init() {
    // API不兼容，暂时移除collision处理
    // 原代码：notice_collisions_with("Car", [this](Event& e) {...});
    
    std::cout << "Traffic light initialized\n";
}

// Update method called by the ENVIRO framework
void TrafficLight::update() {
    // Update the timer
    _time_in_state += delta();
    
    // Check if it's time to change state
    if (_time_in_state >= _state_duration) {
        // Change to next state
        switch (_current_state) {
            case State::RED:
                _current_state = State::GREEN;
                _state_duration = GREEN_DURATION;
                break;
                
            case State::YELLOW:
                _current_state = State::RED;
                _state_duration = RED_DURATION;
                break;
                
            case State::GREEN:
                _current_state = State::YELLOW;
                _state_duration = YELLOW_DURATION;
                break;
        }
        
        // Reset timer
        _time_in_state = 0;
        
        // Update visuals
        update_visuals();
    }
}

// Manually change the traffic light state
void TrafficLight::set_state(int new_state) {
    if (new_state >= 0 && new_state <= 2) {
        _current_state = static_cast<State>(new_state);
        _time_in_state = 0;
        
        // Set appropriate duration based on new state
        switch (_current_state) {
            case State::RED:
                _state_duration = RED_DURATION;
                break;
                
            case State::YELLOW:
                _state_duration = YELLOW_DURATION;
                break;
                
            case State::GREEN:
                _state_duration = GREEN_DURATION;
                break;
        }
        
        update_visuals();
    }
}

// Get the current state of the traffic light
int TrafficLight::get_state() const {
    return static_cast<int>(_current_state);
}

// Returns whether the light is currently red
bool TrafficLight::is_red() const {
    return _current_state == State::RED;
}

// Returns whether the light is currently yellow
bool TrafficLight::is_yellow() const {
    return _current_state == State::YELLOW;
}

// Returns whether the light is currently green
bool TrafficLight::is_green() const {
    return _current_state == State::GREEN;
}

// Apply visual changes based on current state
void TrafficLight::update_visuals() {
    // Visual updates 使用简化方法
    std::cout << "Traffic light changed to ";
    switch (_current_state) {
        case State::RED:
            std::cout << "RED";
            break;
            
        case State::YELLOW:
            std::cout << "YELLOW";
            break;
            
        case State::GREEN:
            std::cout << "GREEN";
            break;
    }
    std::cout << std::endl;
}