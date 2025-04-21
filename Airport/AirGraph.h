#ifndef AIRGRAPH_H
#define AIRGRAPH_H

#include "Airport.h"
#include "Travel.h"
#include <vector>
#include <string>
#include <queue>
#include <iostream>
#include <unordered_map>
#include <climits>
#include "MinHeap.h"

using namespace std;

template <typename T>
class AirGraph {
public:
    struct PathResult {
        vector<Airport<T>> path;
        int totalDistance;
        int totalCost;
    };

    AirGraph() {}

    void insert_airport(const Airport<T>& air) {
        if (get_airport_index(air) == -1) {
            airports.push_back(air);
            vector<Travel> tmp;
            travels.push_back(tmp);
        }
    }

    int get_airport_index(const Airport<T> &air) const {
        for (int i = 0; i < airports.size(); i++) {
            if (airports[i].getData() == air.getData()) {
                return i;
            }
        }
        return -1;
    }

    void add_travel(const Airport<T>& air1, const Airport<T>& air2, int distance, int cost) {
        int i1 = get_airport_index(air1);
        int i2 = get_airport_index(air2);
        if (i1 == -1 || i2 == -1) {
            throw string("Add travel: incorrect vertices");
        }
        Travel v(i1, i2, distance, cost);
        Travel v2(i2, i1, distance, cost); // For undirected graph
        travels[i1].push_back(v);
        travels[i2].push_back(v2);
    }

    void print() const {
        cout << "\n=== Airport Network ===\n";
        for (int i = 0; i < airports.size(); i++) {
            cout << "Airport: " << airports[i].getData() << " (" << airports[i].getState() << ")\n";
            cout << "Connections (" << travels[i].size() << "):\n";
            for (int j = 0; j < travels[i].size(); j++) {
                int dest_index = travels[i][j].dest;
                cout << " -> " << airports[dest_index].getData() 
                     << " (Distance: " << travels[i][j].distance 
                     << ", Cost: " << travels[i][j].cost << ")\n";
            }
            cout << "------------------\n";
        }
    }

    PathResult dijkstra_shortest_path(const Airport<T> &org, const Airport<T> &dest) {
        int i_org = get_airport_index(org);
        int i_dest = get_airport_index(dest);
        
        if (i_org == -1 || i_dest == -1) {
            return {vector<Airport<T>>(), -1, -1};
        }
        
        vector<int> distances(airports.size(), INT_MAX);
        vector<int> costs(airports.size(), INT_MAX);
        vector<int> previous(airports.size(), -1);
        vector<bool> visited(airports.size(), false);
        
        distances[i_org] = 0;
        costs[i_org] = 0;
        
        MinHeap<pair<int, int>> heap; // <distance, airport_index>
        heap.insert({0, i_org});
        
        while (!heap.isEmpty()) {
            auto current = heap.extractMin();
            int currentDist = current.first;
            int currentIndex = current.second;
            
            if (visited[currentIndex]) continue;
            visited[currentIndex] = true;
            
            if (currentIndex == i_dest) break;
            
            for (const Travel& travel : travels[currentIndex]) {
                int neighborIndex = travel.getOther(currentIndex);
                int newDist = currentDist + travel.distance;
                int newCost = costs[currentIndex] + travel.cost;
                
                if (newDist < distances[neighborIndex]) {
                    distances[neighborIndex] = newDist;
                    costs[neighborIndex] = newCost;
                    previous[neighborIndex] = currentIndex;
                    heap.insert({newDist, neighborIndex});
                }
            }
        }
        
        if (distances[i_dest] == INT_MAX) {
            return {vector<Airport<T>>(), -1, -1};
        }
        
        vector<Airport<T>> path;
        int current = i_dest;
        while (current != -1) {
            path.insert(path.begin(), airports[current]);
            current = previous[current];
        }
        
        return {path, distances[i_dest], costs[i_dest]};
    }

    void printShortestPath(const Airport<T>& org, const Airport<T>& dest) {
        PathResult result = dijkstra_shortest_path(org, dest);
        
        if (result.path.empty()) {
            cout << "Shortest route from " << org.getData() << " to " << dest.getData() << ": None\n";
            return;
        }
        
        cout << "Shortest route from " << org.getData() << " to " << dest.getData() << ": ";
        for (size_t i = 0; i < result.path.size(); ++i) {
            if (i != 0) cout << " -> ";
            cout << result.path[i].getData();
        }
        cout << ". The length is " << result.totalDistance << ". The cost is " << result.totalCost << ".\n";
    }

    void printAirportConnections() {
        for (int i = 0; i < airports.size(); i++) {
            cout << airports[i].getData() << ": " << travels[i].size() << " connections\n";
        }
    }

private:
    vector<Airport<T>> airports;
    vector<vector<Travel>> travels;
};

#endif // AIRGRAPH_H