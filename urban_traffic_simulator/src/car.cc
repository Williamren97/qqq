#include "car.h"
#include <cmath>
#include <iostream>

using namespace enviro;

// Constructor for Car agent
Car::Car(json spec, World& world) : Agent(spec, world) {
    // Initialize car properties
    _state = State::IDLE;
    _desired_speed = 30.0;
    _max_speed = 50.0;
    _min_distance = 15.0;
    _stopping_distance = 30.0;
    _current_waypoint = 0;
    
    // Set initial destination to current position (no movement)
    _destination = cpv(position().x, position().y);
}

// Initialize the car agent
void Car::init() {
    // API不兼容，暂时移除collision处理
    // 原代码：notice_collisions_with("Car", [this](Event& e) {...});
    
    std::cout << "Car initialized\n";
}

// Update is called repeatedly to move and navigate the car
void Car::update() {
    // Update sensor readings
    _front_sensor = sensor_value(0);
    _left_sensor = sensor_value(1);
    _right_sensor = sensor_value(2);
    _rear_sensor = sensor_value(3);
    
    // Basic state machine for car behavior
    switch (_state) {
        case State::IDLE:
            // Car is not moving
            omni_apply_force(0, 0);
            break;
            
        case State::DRIVING:
            if (detect_obstacles() || detect_traffic_light()) {
                stop();
                _state = State::STOPPED;
            } else if (at_waypoint()) {
                update_waypoints();
            } else {
                drive();
            }
            break;
            
        case State::STOPPED:
            // Check if obstacle is gone
            if (!detect_obstacles() && !detect_traffic_light()) {
                _state = State::DRIVING;
            }
            break;
            
        case State::TURNING:
            // Performed in the drive method
            // Will transition to DRIVING when turn is complete
            break;
            
        case State::WAITING:
            // Waiting at traffic light or for another condition
            // Will be triggered by external events
            break;
    }
}

// Set a new destination for the car
void Car::set_destination(double x, double y) {
    _destination = cpv(x, y);
    find_path();
    _state = State::DRIVING;
}

// Returns a string representing the car's state
std::string Car::get_state() const {
    switch (_state) {
        case State::IDLE:
            return "idle";
        case State::DRIVING:
            return "driving";
        case State::STOPPED:
            return "stopped";
        case State::TURNING:
            return "turning";
        case State::WAITING:
            return "waiting";
        default:
            return "unknown";
    }
}

// Move the car forward
void Car::drive() {
    // If no path, return to idle
    if (_path.empty()) {
        _state = State::IDLE;
        return;
    }
    
    // Calculate direction to next waypoint
    cpVect current_pos = cpv(position().x, position().y);
    cpVect target = _path[_current_waypoint];
    
    // Calculate direction vector
    cpVect direction = cpvsub(target, current_pos);
    double distance = cpvlength(direction);
    
    // If direction vector is too small, avoid division by zero
    if (distance < 0.1) {
        update_waypoints();
        return;
    }
    
    // Normalize direction vector
    direction = cpvmult(direction, 1.0 / distance);
    
    // Calculate angle difference
    double target_angle = atan2(direction.y, direction.x);
    double current_angle = angle();
    double angle_diff = target_angle - current_angle;
    
    // Normalize angle difference to [-pi, pi]
    while (angle_diff > M_PI) angle_diff -= 2 * M_PI;
    while (angle_diff < -M_PI) angle_diff += 2 * M_PI;
    
    // Apply turning force if angle is off
    if (std::abs(angle_diff) > 0.1) {
        _state = State::TURNING;
        double rotation_force = angle_diff * 10;
        omni_apply_force(0, 0);
        // 使用两个参数的apply_force
        apply_force(0, rotation_force);
    } else {
        // Apply driving force
        _state = State::DRIVING;
        
        // Adjust speed based on distance to waypoint
        double speed_factor = 1.0;
        if (distance < _stopping_distance) {
            speed_factor = distance / _stopping_distance;
        }
        
        // Apply force in the direction of travel
        double speed = std::min(_desired_speed * speed_factor, _max_speed);
        double force_x = direction.x * speed;
        double force_y = direction.y * speed;
        omni_apply_force(force_x, force_y);
        // 使用两个参数的apply_force
        apply_force(0, angle_diff * 2);
    }
}

// Stop the car
void Car::stop() {
    // Apply braking force opposite to current velocity
    cpVect velocity_vec = cpv(velocity().x, velocity().y);
    double velocity_magnitude = cpvlength(velocity_vec);
    
    // Only apply braking if moving
    if (velocity_magnitude > 0.1) {
        // Calculate braking force (opposite of velocity)
        cpVect braking_direction = cpvneg(cpvnormalize(velocity_vec));
        double braking_force = 50.0; // Adjust as needed
        
        omni_apply_force(
            braking_direction.x * braking_force,
            braking_direction.y * braking_force
        );
    } else {
        // If nearly stopped, just zero out velocity
        omni_apply_force(0, 0);
    }
}

// Turn the car by the specified angle
void Car::turn(double angle) {
    // 使用两个参数的apply_force
    double rotation_force = angle * 10;
    apply_force(0, rotation_force);
}

// Find path to destination
void Car::find_path() {
    // For simplicity, we're using a direct path
    // In a more complex implementation, this would use A* or similar
    _path.clear();
    _path.push_back(_destination);
    _current_waypoint = 0;
}

// Update navigation waypoints
void Car::update_waypoints() {
    if (_current_waypoint < _path.size() - 1) {
        _current_waypoint++;
    } else {
        // Reached final destination
        _state = State::IDLE;
        _path.clear();
    }
}

// Check if car is at current waypoint
bool Car::at_waypoint() const {
    if (_path.empty() || _current_waypoint >= _path.size()) {
        return true;
    }
    
    cpVect current_pos = cpv(position().x, position().y);
    cpVect waypoint = _path[_current_waypoint];
    double distance = cpvlength(cpvsub(waypoint, current_pos));
    
    // If within 5 units of waypoint, consider it reached
    return distance < 5.0;
}

// Detect obstacles using sensors
bool Car::detect_obstacles() {
    // Check front sensor for obstacles
    if (_front_sensor < _min_distance && _front_sensor > 0) {
        return true;
    }
    
    // You can add more complex obstacle detection here
    
    return false;
}

// Detect traffic lights
bool Car::detect_traffic_light() {
    // API不兼容，暂时使用简化版本
    // 原代码：auto ahead = sensor_reflection_type(0);
    
    // 简化的交通灯检测
    // 随机决定是否停车，作为临时解决方案
    static int counter = 0;
    counter++;
    return (counter % 100 < 30); // 30% chance of stopping
}