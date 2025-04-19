#include "FlightEdge.h"

//  create the flight edges with origin, destination, distance, and cost

FlightEdge::FlightEdge(const std::string origin, std::string dest, int dist, int cost){
  this->origin = origin;
  this->destination = dest;
  this->distance = dist;
  this->cost = cost;
