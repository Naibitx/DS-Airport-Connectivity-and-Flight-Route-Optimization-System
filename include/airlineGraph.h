//
// Created by maste on 4/13/2025.
//

#ifndef AIRLINEGRAPH_H
#define AIRLINEGRAPH_H

#include <vector>
#include <string>

// Pseudocode 
// struct Path{}
// struct Connections {}
// struct MST_Edge {}

class airlineGraph {
private:
  std::string origin_airp;
  std::string destination;
  std::string origin_city;
  int distance;
  int cost;
  /*struct adj Node
    destination
    cost->dist
    Adj *Node next
    Adj Node(std::string destination, distance, int cost)
  struct NodeList*/
public:
  airlineraph(); // constructor
  ~airlineGraph(); // deconstructor
  bool readCSV(const std::string& filename);
  bool shortestpath(const std::string& origin_airp);
};

#endif //AIRLINEGRAPH_H

//READ CSV
//Create Graph
//Shortest_path(Distance)
//S_P(D, cost)
