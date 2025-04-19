#include "FlightEdge.h"

//  create the flight edges with origin, destination, distance, and cost

FlightEdge::FlightEdge(const std::string origin, std::string dest, int dist, int cost){
  // assignning member variables from the header
  this->origin = origin;
  this->destination = dest;
  this->distance = dist;
  this->cost = cost;
}
