#ifndef AIRLINEGRAPH_H
#define AIRLINEGRAPH_H

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <queue>

// Flight information for edges
struct Flight {
    std::string destination;
    int distance, cost;
    
    Flight() {}
    Flight(const std::string& dest, int dist, int cost)
        : destination(dest), distance(dist), cost(cost) {}
};

// Path from origin to destination
struct Path {
    std::vector<std::string> air_code;
    int totalDistance;
    int totalCost;
    
    Path() : totalDistance(0), totalCost(0) {}
};

// For edges in minimum spanning trees
struct mstEdge {
    std::string from, to;
    int cost;

    mstEdge() {}
    mstEdge(const std::string& f, const std::string& t, int cst)
        : from(f), to(t), cost(cst) {}
};

// Flight connection info for each airport
struct Connections {
    std::string air_code;
    int in, out;
    
    Connections() : in(0), out(0) {}
    
    int totalConnections() const {
        return in + out;
    }
};

// Disjoint set for MST
struct DisjointSet {
    std::vector<int> parent;
    std::vector<int> rank;
    
    DisjointSet(int size) : parent(size), rank(size, 0) {
        for (int i = 0; i < size; ++i) {
            parent[i] = i;
        }
    }
    
    int find(int v) {
        if (parent[v] != v) {
            parent[v] = find(parent[v]); // Path compression
        }
        return parent[v];
    }
    
    void unionSet(int u, int v) {
        int rootU = find(u);
        int rootV = find(v);
        
        if (rootU != rootV) {
            if (rank[rootU] < rank[rootV]) {
                parent[rootU] = rootV;
            } else if (rank[rootU] > rank[rootV]) {
                parent[rootV] = rootU;
            } else {
                parent[rootV] = rootU;
                rank[rootU]++;
            }
        }
    }
};

class airlineGraph {
private:
    std::unordered_map<std::string, std::vector<Flight> > adjList;
    std::unordered_map<std::string, std::string> airportToState;
    std::vector<mstEdge> undirectedEdges;
    
    bool airportExists(const std::string& air_code) const;
    int getIndex(const std::string& air_code) const;
    std::vector<std::string> getAllAirports() const;

public:
    airlineGraph() {}
    
    void addAirportNode(const std::string& air_code, const std::string& state_code);
    void addFlightEdge(const std::string& origin_airp, const std::string& destination, int distance, int cost);
    Path dijkstraPath(const std::string& origin_airp, const std::string& destination, bool useCost = false);
    std::vector<Path> shortestPathsToState(const std::string& origin_airp, const std::string& state, bool useCost = false);
    Path shortestPathWithStops(const std::string& origin_airp, const std::string& destination, int stops);
    std::vector<Connections> countConnections();
    void createUndirectedGraph();
    void readCSV(const std::string& filename);
    
    std::pair<std::vector<mstEdge>, int> primMST();
    std::pair<std::vector<mstEdge>, int> kruskalMST();
    
    const std::unordered_map<std::string, std::string>& getAirportToState() const { return airportToState; }
    const std::unordered_map<std::string, std::vector<Flight> >& getAdjList() const { return adjList; }
};

#endif // AIRLINEGRAPH_H