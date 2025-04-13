#ifndef FLIGHTEDGE_H
#define FLIGHTEDGE_H

#include <string>

class FlightEdge{
    public:

    std::string origin;
    std::string destination;
    int distance;
    int cost;

    FlightEdge(const std::string origin, std::string dest, int dist, int cost );
};

#endif