#include "airlineGraph.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <algorithm>

using namespace std;

// Function to print a Path object
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

// Function to print MST results
void printMST(const vector<mstEdge>& mst, int totalCost) {
    cout << "Minimal Spanning Tree:\nEdge Weight\n";
    for (const auto& edge : mst) {
        cout << edge.from << " - " << edge.to << " " << edge.cost << endl;
    }
    cout << "Total Cost of MST: " << totalCost << endl;
}

// Helper function to clear input buffer
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Helper function to trim whitespace from a string
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    if (first == string::npos) return "";
    return str.substr(first, last - first + 1);
}

// Helper function to get a valid airport code
string getAirportCode(const string& prompt) {
    string code;
    while (true) {
        cout << prompt;
        getline(cin, code);
        code = trim(code);
        if (!code.empty() && code.length() <= 3) { // Assuming 3-letter codes like ATL
            transform(code.begin(), code.end(), code.begin(), ::toupper);
            return code;
        }
        cout << "Invalid airport code. Please enter a valid 3-letter code (e.g., ATL): ";
    }
}

// Helper function to get a valid state code
string getStateCode(const string& prompt) {
    string code;
    while (true) {
        cout << prompt;
        getline(cin, code);
        code = trim(code);
        if (!code.empty() && code.length() == 2) { // Assuming 2-letter state codes like FL
            transform(code.begin(), code.end(), code.begin(), ::toupper);
            return code;
        }
        cout << "Invalid state code. Please enter a valid 2-letter code (e.g., FL): ";
    }
}

// Helper function to get a valid number of stops
int getStops(const string& prompt) {
    string input;
    int stops;
    while (true) {
        cout << prompt;
        getline(cin, input);
        input = trim(input);
        try {
            stops = stoi(input);
            if (stops >= 0) return stops;
            cout << "Number of stops must be non-negative. Try again: ";
        } catch (const exception& e) {
            cout << "Invalid number. Please enter a valid integer: ";
        }
    }
}

// Function to display the menu
void displayMenu() {
    cout << "\n✈---Airline Graph Menu---✈\n";
    cout << "1. Shortest Path Between Two Airports (Dijkstra)\n";
    cout << "2. Shortest Paths to State Airports\n";
    cout << "3. Shortest Path with Specific Number of Stops\n";
    cout << "4. Count Airport Connections\n";
    cout << "5. Prim's Minimal Spanning Tree\n";
    cout << "6. Kruskal's Minimal Spanning Tree\n";
    cout << "7. Contributions\n";
    cout << "8. Exit\n";
    cout << "Enter your choice (1-8): ";
}

int main() {
    airlineGraph graph;
    
    // Read CSV file once at startup
    cout << "Loading airports.csv..." << endl;
    graph.readCSV("airports.txt");
    
    while (true) {
        displayMenu();
        
        string input;
        int choice;
        getline(cin, input);
        input = trim(input);
        
        try {
            choice = stoi(input);
        } catch (const exception& e) {
            cout << "Invalid choice. Please enter a number between 1 and 8.\n";
            continue;
        }
        
        if (choice < 1 || choice > 8) {
            cout << "Invalid choice. Please enter a number between 1 and 8.\n";
            continue;
        }
        
        if (choice == 8) {
            cout << "Have a safe flight!\n";
            break;
        }
        
        switch (choice) {
            case 1: {
                // Task 2: Shortest Path (Dijkstra)
                string src = getAirportCode("Enter source airport code (e.g., ATL): ");
                string dest = getAirportCode("Enter destination airport code (e.g., MIA): ");
                cout << "\nShortest path from " << src << " to " << dest << " :\n";
                Path path = graph.dijkstraPath(src, dest, false);
                printPath(path);
                break;
            }
            case 2: {
                // Task 3: Shortest Paths to State Airports
                string src = getAirportCode("Enter source airport code (e.g., ATL): ");
                string state = getStateCode("Enter destination state code (e.g., FL): ");
                cout << "\nShortest paths from " << src << " to " << state << " state airports:\n";
                auto paths = graph.shortestPathsToState(src, state, false);
                cout << "Path Length Cost\n";
                for (const auto& p : paths) {
                    for (size_t i = 0; i < p.air_code.size(); ++i) {
                        cout << p.air_code[i];
                        if (i < p.air_code.size() - 1) cout << "->";
                    }
                    cout << " " << p.totalDistance << " " << p.totalCost << endl;
                }
                break;
            }
            case 3: {
                // Task 4: Shortest Path with Stops
                string src = getAirportCode("Enter source airport code (e.g., ATL): ");
                string dest = getAirportCode("Enter destination airport code (e.g., MIA): ");
                int stops = getStops("Enter number of stops (e.g., 3): ");
                cout << "\nShortest path from " << src << " to " << dest << " with " << stops << " stops:\n";
                Path path = graph.shortestPathWithStops(src, dest, stops);
                printPath(path);
                break;
            }
            case 4: {
                // Task 5: Airport Connections
                cout << "\nAirport Connections:\n";
                auto connections = graph.countConnections();
                for (const auto& conn : connections) {
                    cout << conn.air_code << " " << conn.totalConnections() << endl;
                }
                break;
            }
            case 5: {
                // Task 7: Prim's MST
                cout << "\nPrim's MST:\n";
                pair<vector<mstEdge>, int> primResult = graph.primMST();
                vector<mstEdge> primMST = primResult.first;
                int primCost = primResult.second;
                printMST(primMST, primCost);
                break;
            }
            case 6: {
                // Task 8: Kruskal's MST
                cout << "\nKruskal's MST:\n";
                pair<vector<mstEdge>, int> kruskalResult = graph.kruskalMST();
                vector<mstEdge> kruskalMST = kruskalResult.first;
                int kruskalCost = kruskalResult.second;
                printMST(kruskalMST, kruskalCost);
                break;
            }
            case 7: {
                // Contributions
                cout << "\n===Contributtions︎===\n";
                cout <<"\nNaibys Alzugaray: Implemented flightedge.h,airport.h, and implemented tasks 3-6\n";
                cout <<"\nMaurcio Peralta: Implemented task 1-2, csv file reading \n";
                cout <<"\nDaniel Hamilton: Implemented airlineGraph.h, report and presentation\n";
                break;
            }
            default:
                cout << "Unexpected error. Please try again.\n";
                break;
        }
    }
    
    return 0;
}