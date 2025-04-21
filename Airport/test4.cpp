#include "AirGraph.h"
#include "Airport.h"
#include "read_cvs.h"
#include "travel.h"
#include <unordered_map>
#include <string>
#include <iostream>

using namespace std;

int main() {
    // Read data from CSV
    string filename = "C:\\Users\\maste\\CLionProjects\\ReadFromCSV\\airports.csv"; // Update with your actual path
    vector<vector<string>> data = read_cvs(filename);

    // Graph to store airports and travels
    AirGraph<string> airGraph;

    // Map to track existing airports
    unordered_map<string, Airport<string>> airportMap;

    // Step 1: Insert all unique airports
    for (const auto& row : data) {
        string originCode = row[0];
        string originCityState = row[2] + ", " + row[3];

        if (airportMap.find(originCode) == airportMap.end()) {
            Airport<string> originAirport(originCode, originCityState);
            airGraph.insert_airport(originAirport);
            airportMap[originCode] = originAirport;
        }

        string destCode = row[1];
        if (airportMap.find(destCode) == airportMap.end()) {
            Airport<string> destAirport(destCode, row[4] + ", " + row[5]);
            airGraph.insert_airport(destAirport);
            airportMap[destCode] = destAirport;
        }
    }

    // Step 2: Add travels between airports
    for (const auto& row : data) {
        string originCode = row[0];
        string destCode = row[1];
        int distance = stoi(row[6]);
        int cost = stoi(row[7]);

        airGraph.add_travel(
            airportMap[originCode],
            airportMap[destCode],
            distance,
            cost
        );
    }

    // Print the graph structure
    airGraph.print();

    // Test shortest paths (Task 2)
    vector<pair<string, string>> testRoutes = {
        {"IAD", "MIA"},
        {"PIT", "ACT"},
        {"ATL", "MIA"},
        {"ATL", "TPA"},
        {"ATL", "MCO"}
    };

    cout << "\n=== Shortest Path Tests ===\n";
    for (const auto& route : testRoutes) {
        if (airportMap.find(route.first) != airportMap.end() &&
            airportMap.find(route.second) != airportMap.end()) {
            airGraph.printShortestPath(airportMap[route.first], airportMap[route.second]);
        } else {
            cout << "One or both airports (" << route.first << ", " << route.second
                 << ") not found!\n";
        }
    }

    // Task 5: Airport Connections
    cout << "\n=== Airport Connections ===\n";
    airGraph.printAirportConnections();

    return 0;
}