#ifndef __ROAD_NETWORK__H
#define __ROAD_NETWORK__H 

#include "enviro.h"
#include <vector>
#include <map>
#include <string>

using namespace enviro;

//! Waypoint structure for navigation
struct Waypoint {
    double x;
    double y;
    std::vector<int> connections; // Indices of connected waypoints
};

//! Road segment structure
struct RoadSegment {
    int id;
    std::string type; // "horizontal", "vertical", "intersection"
    double x;
    double y;
    double width;
    double length;
    double rotation;
    std::vector<int> waypoints; // Indices of waypoints on this road
};

//! RoadNetwork - Manages the grid layout and navigation graph for the city
class RoadNetwork {

    public:
        //! Constructor - Creates a new road network
        RoadNetwork(enviro::World& world);
        
        //! Destructor
        ~RoadNetwork();
        
        //! Initialize the road network with a grid layout
        //! \param grid_size Number of blocks in the grid (grid_size x grid_size)
        //! \param block_size Size of each city block in world units
        void init_grid(int grid_size, double block_size);
        
        //! Add a traffic light at a specific intersection
        //! \param intersection_id ID of the intersection
        //! \return ID of the created traffic light agent
        int add_traffic_light(int intersection_id);
        
        //! Get nearest road segment to a location
        //! \param x X-coordinate
        //! \param y Y-coordinate
        //! \return ID of the nearest road segment
        int get_nearest_road(double x, double y);
        
        //! Get nearest waypoint to a location
        //! \param x X-coordinate
        //! \param y Y-coordinate
        //! \return Index of the nearest waypoint
        int get_nearest_waypoint(double x, double y);
        
        //! Find a path between two waypoints
        //! \param start_waypoint Index of starting waypoint
        //! \param end_waypoint Index of ending waypoint
        //! \return Vector of waypoint indices forming the path
        std::vector<int> find_path(int start_waypoint, int end_waypoint);
        
        //! Get coordinates of a waypoint
        //! \param waypoint_index Index of the waypoint
        //! \return Pair of (x, y) coordinates
        std::pair<double, double> get_waypoint_location(int waypoint_index);
        
        //! Get all road segments
        //! \return Vector of all road segments
        const std::vector<RoadSegment>& get_road_segments() const;
        
        //! Get all waypoints
        //! \return Vector of all waypoints
        const std::vector<Waypoint>& get_waypoints() const;

    private:
        //! Reference to the ENVIRO world
        enviro::World& _world;
        
        //! Road segments in the network
        std::vector<RoadSegment> _road_segments;
        
        //! Waypoints for navigation
        std::vector<Waypoint> _waypoints;
        
        //! Map of intersection IDs to traffic light IDs
        std::map<int, int> _traffic_lights;
        
        //! Create horizontal road segment
        //! \param x X-coordinate of center
        //! \param y Y-coordinate of center
        //! \param length Length of road segment
        //! \return ID of created road segment
        int create_horizontal_road(double x, double y, double length);
        
        //! Create vertical road segment
        //! \param x X-coordinate of center
        //! \param y Y-coordinate of center
        //! \param length Length of road segment
        //! \return ID of created road segment
        int create_vertical_road(double x, double y, double length);
        
        //! Create intersection
        //! \param x X-coordinate of center
        //! \param y Y-coordinate of center
        //! \return ID of created intersection
        int create_intersection(double x, double y);
        
        //! Create a waypoint
        //! \param x X-coordinate
        //! \param y Y-coordinate
        //! \return Index of created waypoint
        int create_waypoint(double x, double y);
        
        //! Connect two waypoints (bidirectional)
        //! \param wp1 Index of first waypoint
        //! \param wp2 Index of second waypoint
        void connect_waypoints(int wp1, int wp2);
        
        //! Calculate distance between two points
        //! \param x1 X-coordinate of first point
        //! \param y1 Y-coordinate of first point
        //! \param x2 X-coordinate of second point
        //! \param y2 Y-coordinate of second point
        //! \return Distance between the points
        double distance(double x1, double y1, double x2, double y2);
};

#endif