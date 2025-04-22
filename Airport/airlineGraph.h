#ifndef AIRLINEGRAPH_H
#define AIRLINEGRAPH_H

#include <string>
#include <vector>
#include <unordered_map>
#include <queue>

struct Flight {
    std::string destination;
    int distance, cost;
    
    Flight() {}
    Flight(const std::string& dest, int dist, int cost)
        : destination(dest), distance(dist), cost(cost) {}
};

struct Path {
    std::vector<std::string> air_code;
    int totalDistance;
    int totalCost;
    
    Path() : totalDistance(0), totalCost(0) {}
};

struct mstEdge {
    std::string from, to;
    int cost;
    
    mstEdge() {}
    mstEdge(const std::string& f, const std::string& t, int cst)
        : from(f), to(t), cost(cst) {}
};

struct Connections {
    std::string air_code;
    int in, out;
    
    Connections() : in(0), out(0) {}
    
    int totalConnections() const {
        return in + out;
    }
};

class DisjointSet {
    std::vector<int> parent, rank;
public:
    DisjointSet(int size);
    int find(int v);
    void unionSet(int u, int v);
};

class airlineGraph {
private:
    std::unordered_map<std::string, std::vector<Flight>> adjList;
    std::unordered_map<std::string, std::string> airportToState;
    std::vector<mstEdge> undirectedEdges;
    
    bool airportExists(const std::string& air_code) const;
    std::vector<std::string> getAllAirports() const;
    int getIndex(const std::string& air_code) const;

public:
    airlineGraph() {}
    
    void addAirportNode(const std::string& air_code, const std::string& state_code);
    void addFlightEdge(const std::string& origin, const std::string& dest, int dist, int cost);
    Path dijkstraPath(const std::string& origin, const std::string& dest, bool useCost = false);
    std::vector<Path> shortestPathsToState(const std::string& origin, const std::string& state, bool useCost = false);
    Path shortestPathWithStops(const std::string& origin, const std::string& dest, int maxStops);
    std::vector<Connections> countConnections();
    void createUndirectedGraph();
    void readCSV(const std::string& filename);
    
    std::pair<std::vector<mstEdge>, int> primMST();
    std::pair<std::vector<mstEdge>, int> kruskalMST();
    
    const std::unordered_map<std::string, std::string>& getAirportToState() const { return airportToState; }
    const std::unordered_map<std::string, std::vector<Flight>>& getAdjList() const { return adjList; }
};

#endif