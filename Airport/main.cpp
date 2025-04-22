#include "airlineGraph.h"
#include <iostream>
#include <iomanip>

using namespace std;

void printPath(const Path& path) {
    if (path.air_code.empty()) {
        cout << "No path exists" << endl;
        return;
    }
    
    for (size_t i = 0; i < path.air_code.size(); ++i) {
        cout << path.air_code[i];
        if (i < path.air_code.size() - 1) cout << " -> ";
    }
    cout << "\nDistance: " << path.totalDistance << ", Cost: " << path.totalCost << endl;
}

void printMST(const vector<mstEdge>& mst, int totalCost) {
    cout << "Minimal Spanning Tree:\nEdge Weight\n";
    for (const auto& edge : mst) {
        cout << edge.from << " - " << edge.to << " " << edge.cost << endl;
    }
    cout << "Total Cost of MST: " << totalCost << endl;
}

int main() {
    airlineGraph graph;
    
    graph.readCSV("airports.csv");
    
    cout << "\nTask 2: Shortest path example\n";
    Path path = graph.dijkstraPath("ATL", "MIA", false);
    cout << "Shortest path from ATL to MIA (distance):\n";
    printPath(path);
    
    path = graph.dijkstraPath("PIT", "ACT", false);
    cout << "\nShortest path from PIT to ACT (distance):\n";
    printPath(path);
    
    cout << "\nTask 3: Shortest paths to FL state airports from ATL\n";
    auto paths = graph.shortestPathsToState("ATL", "FL", false);
    cout << "Path Length Cost\n";
    for (const auto& p : paths) {
        for (size_t i = 0; i < p.air_code.size(); ++i) {
            cout << p.air_code[i];
            if (i < p.air_code.size() - 1) cout << "->";
        }
        cout << " " << p.totalDistance << " " << p.totalCost << endl;
    }
    
    cout << "\nTask 4: Shortest path with stops\n";
    path = graph.shortestPathWithStops("ATL", "MIA", 3);
    cout << "Shortest path from ATL to MIA with 3 stops:\n";
    printPath(path);
    
    path = graph.shortestPathWithStops("PIT", "ACT", 2);
    cout << "\nShortest path from PIT to ACT with 2 stops:\n";
    printPath(path);
    
    cout << "\nTask 5: Airport Connections\n";
    auto connections = graph.countConnections();
    for (const auto& conn : connections) {
        cout << conn.air_code << " " << conn.totalConnections() << endl;
    }
    
    cout << "\nTask 7: Prim's MST\n";
    pair<vector<mstEdge>, int> primResult = graph.primMST();
    vector<mstEdge> primMST = primResult.first;
    int primCost = primResult.second;
    printMST(primMST, primCost);
    
    cout << "\nTask 8: Kruskal's MST\n";
    pair<vector<mstEdge>, int> kruskalResult = graph.kruskalMST();
    vector<mstEdge> kruskalMST = kruskalResult.first;
    int kruskalCost = kruskalResult.second;
    printMST(kruskalMST, kruskalCost);
    
    return 0;
}