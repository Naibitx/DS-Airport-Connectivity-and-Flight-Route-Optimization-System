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
    
    // Read CSV file (adjust path as needed)
    graph.readCSV("flights.csv");
    
    // Task 2: Shortest path between two airports
    cout << "\nTask 2: Shortest path example\n";
    Path path = graph.dijkstraPath("ATL", "MIA", false); // Distance-based
    cout << "Shortest path from ATL to MIA (distance):\n";
    printPath(path);
    
    path = graph.dijkstraPath("PIT", "ACT", false);
    cout << "\nShortest path from PIT to ACT (distance):\n";
    printPath(path);
    
    // Task 3: Shortest paths to state
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
    
    // Task 4: Shortest path with stops
    cout << "\nTask 4: Shortest path with stops\n";
    path = graph.shortestPathWithStops("ATL", "MIA", 3);
    cout << "Shortest path from ATL to MIA with 3 stops:\n";
    printPath(path);
    
    path = graph.shortestPathWithStops("PIT", "ACT", 2);
    cout << "\nShortest path from PIT to ACT with 2 stops:\n";
    printPath(path);
    
    // Task 5: Count connections
    cout << "\nTask 5: Airport Connections\n";
    auto connections = graph.countConnections();
    for (const auto& conn : connections) {
        cout << conn.air_code << " " << conn.totalConnections() << endl;
    }
    
    // Tasks 7 & 8: MST
    cout << "\nTask 7: Prim's MST\n";
    auto [primMST, primCost] = graph.primMST();
    printMST(primMST, primCost);
    
    cout << "\nTask 8: Kruskal's MST\n";
    auto [kruskalMST, kruskalCost] = graph.kruskalMST();
    printMST(kruskalMST, kruskalCost);
    
    return 0;
}