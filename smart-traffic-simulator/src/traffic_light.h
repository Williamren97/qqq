#ifndef __TRAFFIC_LIGHT_AGENT__H
#define __TRAFFIC_LIGHT_AGENT__H

#include "enviro.h"

using namespace enviro;

class TrafficLightController : public Process, public AgentInterface {
    public:
    TrafficLightController() : Process(), AgentInterface(), is_red(true), timer(0) {}

    void init() {
        watch("toggle_light", [this](Event e) {
            std::cout << "Toggle light event received" << std::endl;
            toggle();
            e.stop_propagation();
        });

        // Modify the screen_click handler to use a direct point-in-rectangle check
        watch("screen_click", [this](Event e) {
            double x = e.value()["x"];
            double y = e.value()["y"];
            
            // Check if the click is within the traffic light's bounding box
            // Assuming the traffic light is centered at (0,0) and has a size of 30x30
            if (x >= -15 && x <= 15 && y >= -15 && y <= 15) {
                std::cout << "Traffic light clicked directly" << std::endl;
                toggle();
                e.stop_propagation();
            }
        });

        // Add a label to show this is a traffic light
        label("Traffic Light", 0, -30);
    }

    void start() {
        // Initial red light with clearer visualization
        set_red_light();
    }

    void update() {
        // Use delta() for time-based updates (in seconds)
        timer += delta();

        // Red light for 5 seconds, green light for 10 seconds
        if ((is_red && timer > 5.0) || (!is_red && timer > 10.0)) {
            toggle();
            timer = 0;
        }
    }

    void toggle() {
        is_red = !is_red;
        if (is_red) {
            set_red_light();
        } else {
            set_green_light();
        }
    }

    void set_red_light() {
        // More visible red light with text
        decorate("<g>"
                 "<rect x='-15' y='-15' width='30' height='30' style='fill: #ff0000; stroke: black; stroke-width: 2'/>"
                 "<text x='-10' y='5' style='fill: white; font-size: 14px; font-weight: bold'>RED</text>"
                 "<circle cx='0' cy='0' r='7' style='fill: #ff5555; stroke: white'/>"
                 "</g>");
    }

    void set_green_light() {
        // More visible green light with text
        decorate("<g>"
                 "<rect x='-15' y='-15' width='30' height='30' style='fill: #00aa00; stroke: black; stroke-width: 2'/>"
                 "<text x='-14' y='5' style='fill: white; font-size: 14px; font-weight: bold'>GREEN</text>"
                 "<circle cx='0' cy='0' r='7' style='fill: #55ff55; stroke: white'/>"
                 "</g>");
    }

    bool get_is_red() { return is_red; }

    void stop() {}

    private:
    bool is_red;
    double timer;
};

class TrafficLight : public Agent {
    public:
    TrafficLight(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }

    bool is_red() { return c.get_is_red(); }

    private:
    TrafficLightController c;
};

DECLARE_INTERFACE(TrafficLight)

#endif