// Created by maste on 4/13/2025.

#ifndef AIRLINEGRAPH_H
#define AIRLINEGRAPH_H

#include <string>
#include <vector>

// adjency list for each flight connection
struct AdjNode{
    std::string destination;
    int distance, cost;
    AdjNode* next;
    
    AdjNode(){
        next = nullptr;
    }
    
    AdjNode(const std::string& dest, int dist, int cost){
        destination = dest;
        distance = dist;
        cost = cost;
        next = nullptr;
    }
};

// path from origin to destination
struct Path{
    std::vector<std::string> air_code;
    int totalDistance;
    int totalCost;
    
    Path(){
        totalDistance = 0;
        totalCost = 0;
    }
};

//For edges in min spanning trees
struct mstEdge{
    std::string from, to;
    int cost;
    mstEdge(){}
    
    mstEdge(const std::string& f, const std::string& t, int cst){
        from = f;
        to = t;
        cost = cst;
    }
};

// flight connect info for each airport
struct Connections{
    std::string air_code;
    int in, out;
    
    Connections(){
        in = 0;
        out =0;
    }
    
    int totalConnections(){
        return in + out;
    }
};

// graoh node for each airport
struct GraphNode {
    std::string airport_code;
    std::string state_code;
    AdjNode* head; 

    GraphNode() {
        head = nullptr;
    }

    GraphNode(const std::string& code, const std::string& state) {
        airport_code = code;
        state_code = state;
        head = nullptr;
    }
};

struct AdjList{
    AdjNode* head;
    AdjList(): head(nullptr){} // head of the list
};

//airline graph system
class airlineGraph {
private:
    std::vector<GraphNode> nodes; 
    int getIndex(const std::string& airt_code);
    bool airportExists(const std::string& air_code);
    std::vector<mstEdge> undirectedEdges; 

    struct DisjointSet {
        int* parent;
        int* rank;
        int n;
        DisjointSet(int size);
        ~DisjointSet();  
        int find(int v);
        void unionSet(int u, int v);        
    
    };

public:
    airlineGraph(); 
    ~airlineGraph(); 
    
    // adds new airport node to graph
    void addAirportNode(const std::string& air_code, const std::string& state_code);
    
    //adds flight edges from origin to destination
    void addFlightEdge(const std::string& origin_airp, const std::string& destination, int distance, int cost);
    
    // shortest path from origin_airp to destination
    Path dijkstraPath(const std::string& origin_airp, const std::string& destination, bool useCost = false);
    
    // shortest path from origin to airports in a given state
    std::vector<Path> shortestPathsToState(const std::string& origin_airp, const std::string& state, bool useCost = false);

    // shortest path from origin_airp to destination with stops
    Path shortestPathWithStops(const std::string& origin_airp, const std::string& destination, int stops);
    
    // counts inbound/outbound flight connections
    std::vector<Connections> countConnections();
    
    // converts directed to undirected graph
    void createUndirectedGraph();

    // returns pair of edges and total cost
    std::pair<std::vector<mstEdge>, int> primMST();

    std::pair<std::vector<mstEdge>, int> kruskalMST();
};

#endif // AIRLINEGRAPH_H
