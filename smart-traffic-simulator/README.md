Here's a README in English for your Smart Traffic Simulator project:
markdownCopy# Smart Traffic Simulator

A multi-agent traffic simulation system built using the Enviro framework to simulate vehicle traffic and traffic signal control in an urban environment.

## Features

- **Traffic Grid System**: Simple intersection layout with horizontal and vertical main roads
- **Dynamic Traffic Light Control**:
  - Smart traffic lights automatically cycle between red (5 seconds) and green (10 seconds)
  - Support for manual traffic light switching via UI buttons or keyboard shortcuts
- **Intelligent Vehicle Agents**:
  - Vehicles have automatic collision avoidance to prevent accidents with other vehicles and obstacles
  - Vehicles respond to traffic signals, stopping at red lights and proceeding on green
  - Support for keyboard control (WASD) for manual driving
- **Interactive User Interface**:
  - Add new vehicles at random positions via buttons
  - Add vehicles at specific locations by clicking on roads
  - Support for keyboard shortcuts (C to add vehicles, T to toggle traffic lights)

## Installation Guide

### Prerequisites

- Docker
- Git

### Installation Steps

1. Clone the project repository:
   ```bash
   git clone https://github.com/yourusername/smart-traffic-simulator.git
   cd smart-traffic-simulator

Start the Enviro environment using Docker:
bashCopy# Linux/macOS
docker run -p80:80 -p8765:8765 -v $PWD:/source -it klavins/enviro:v1.61 bash

# Windows
docker run -p80:80 -p8765:8765 -v C:\path\to\project:/source -it klavins/enviro:v1.61 bash

Inside the Docker container, compile and run the project:
bashCopymake clean
make all
esm start
enviro

View the simulation by visiting:
Copyhttp://localhost


Usage Instructions
UI Controls

Add Vehicles: Click the "Add Car" button to add a new car at a random position
Toggle Traffic Lights: Click the "Toggle Lights" button to manually change all traffic light states

Mouse Controls

Add Vehicles on Roads: Click directly on any road to add a new vehicle at that location

Keyboard Controls

C key: Add a new car at a random position
T key: Toggle the state of all traffic lights
WASD keys: Control vehicle movement (W forward, S backward, A left turn, D right turn)

Project Structure
Copysmart-traffic-simulator/
├── Makefile             # Main Makefile
├── config.json          # Main configuration file
├── defs/                # Model definition directory
│   ├── car.json         # Car model definition
│   ├── simulator.json   # Simulator model definition
│   └── traffic_light.json # Traffic light model definition
├── lib/                 # Compiled library files
├── src/                 # Source code directory
│   ├── Makefile         # Source code Makefile
│   ├── car.cc           # Car controller implementation
│   ├── car.h            # Car controller header
│   ├── simulator.cc     # Simulator controller implementation
│   ├── simulator.h      # Simulator controller header
│   ├── traffic_light.cc # Traffic light controller implementation
│   └── traffic_light.h  # Traffic light controller header
Expansion Possibilities
This project can be extended to implement additional features:

Add more complex road networks and intersections
Implement smarter autonomous driving algorithms
Add different types of traffic participants like pedestrians and bicycles
Implement adaptive traffic light control based on traffic flow
Add visual traffic data statistics and analysis

Technical Notes
This project is developed based on the Elma and Enviro frameworks:

Elma: An event loop and process manager
Enviro: A multi-agent simulation environment

Acknowledgements
Thanks to the Klavins Lab for providing the Enviro and Elma frameworks.
License
MIT License
