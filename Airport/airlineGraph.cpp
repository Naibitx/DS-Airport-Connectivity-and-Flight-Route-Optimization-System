#include "airlineGraph.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>
#include <queue>
#include <iostream>

using namespace std;

DisjointSet::DisjointSet(int size) : parent(size), rank(size, 0) {
    for (int i = 0; i < size; ++i) {
        parent[i] = i;
    }
}

int DisjointSet::find(int v) {
    if (parent[v] != v) {
        parent[v] = find(parent[v]);
    }
    return parent[v];
}

void DisjointSet::unionSet(int u, int v) {
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

bool airlineGraph::airportExists(const string& air_code) const {
    return airportToState.find(air_code) != airportToState.end();
}

vector<string> airlineGraph::getAllAirports() const {
    vector<string> airports;
    for (const auto& pair : airportToState) {
        airports.push_back(pair.first);
    }
    return airports;
}

int airlineGraph::getIndex(const string& air_code) const {
    auto airports = getAllAirports();
    for (size_t i = 0; i < airports.size(); ++i) {
        if (airports[i] == air_code) return static_cast<int>(i);
    }
    return -1;
}

void airlineGraph::addAirportNode(const string& air_code, const string& state_code) {
    if (!airportExists(air_code)) {
        airportToState[air_code] = state_code;
    }
}

void airlineGraph::addFlightEdge(const string& origin, const string& dest, int dist, int cost) {
    Flight flight(dest, dist, cost);
    adjList[origin].push_back(flight);
}

string extractState(const string& cityState) {
    size_t comma = cityState.find(",");
    if (comma != string::npos && comma + 2 < cityState.length()) {
        string state = cityState.substr(comma + 2);
        if (state.front() == '"') state = state.substr(1, state.size() - 2);
        return state;
    }
    return "";
}

void airlineGraph::readCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        std::vector<std::string> fields;
        std::string field;
        bool in_quotes = false;
        char c;

        for (size_t i = 0; i < line.size(); ++i) {
            c = line[i];

            if (c == '"') {
                in_quotes = !in_quotes; //toggle quote state
                continue;
            }

            if (c == ',' && !in_quotes) {
                fields.push_back(field); //end of field
                field.clear();
                continue;
            }

            field += c; //add character to current field
        }
        fields.push_back(field); //add the last field

        if (fields.size() != 6) {
            std::cerr << "Invalid CSV line: " << line << std::endl;
            continue;
        }

        std::string originAir = fields[0];
        std::string destAir = fields[1];
        std::string originCityState = fields[2];
        std::string destCityState = fields[3];
        std::string distanceStr = fields[4];
        std::string costStr = fields[5];

        if (!originCityState.empty() && originCityState.front() == '"' && originCityState.back() == '"') {
            originCityState = originCityState.substr(1, originCityState.size() - 2);
        }
        if (!destCityState.empty() && destCityState.front() == '"' && destCityState.back() == '"') {
            destCityState = destCityState.substr(1, destCityState.size() - 2);
        }

        std::string originState = extractState(originCityState);
        std::string destState = extractState(destCityState);

        try {
            int distance = std::stoi(distanceStr);
            int cost = std::stoi(costStr);
            addAirportNode(originAir, originState);
            addAirportNode(destAir, destState);
            addFlightEdge(originAir, destAir, distance, cost);
        } catch (const std::exception& e) {
            std::cerr << "Error parsing numbers in line: " << line << " (" << e.what() << ")" << std::endl;
            continue;
        }
    }
    file.close();
}

Path airlineGraph::dijkstraPath(const string& origin, const string& dest, bool useCost) {
    if (!airportExists(origin) || !airportExists(dest)) {
        return Path();
    }

    unordered_map<string, int> dist;
    unordered_map<string, string> prev;
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;

    for (const auto& airport : airportToState) {
        dist[airport.first] = numeric_limits<int>::max();
    }
    
    dist[origin] = 0;
    pq.push({0, origin});

    while (!pq.empty()) {
        pair<int, string> top = pq.top();
        int curDist = top.first;
        string cur = top.second;
        pq.pop();

        if (cur == dest) break;
        if (curDist > dist[cur]) continue;

        for (const auto& flight : adjList[cur]) {
            int weight = useCost ? flight.cost : flight.distance;
            if (dist[cur] + weight < dist[flight.destination]) {
                dist[flight.destination] = dist[cur] + weight;
                prev[flight.destination] = cur;
                pq.push({dist[flight.destination], flight.destination});
            }
        }
    }

    Path path;
    if (dist[dest] == numeric_limits<int>::max()) {
        return path;
    }

    for (string at = dest; !at.empty(); at = prev[at]) {
        path.air_code.push_back(at);
        if (at == origin) break;
    }
    
    reverse(path.air_code.begin(), path.air_code.end());
    
    for (size_t i = 1; i < path.air_code.size(); ++i) {
        string from = path.air_code[i-1];
        string to = path.air_code[i];
        for (const auto& flight : adjList[from]) {
            if (flight.destination == to) {
                path.totalDistance += flight.distance;
                path.totalCost += flight.cost;
                break;
            }
        }
    }
    
    return path;
}

vector<Path> airlineGraph::shortestPathsToState(const string& origin, const string& state, bool useCost) {
    vector<Path> paths;
    for (const auto& airport : airportToState) {
        if (airport.second == state && airport.first != origin) {
            Path path = dijkstraPath(origin, airport.first, useCost);
            if (!path.air_code.empty()) {
                paths.push_back(path);
            }
        }
    }
    return paths;
}

Path airlineGraph::shortestPathWithStops(const string& origin, const string& dest, int maxStops) {
    if (!airportExists(origin) || !airportExists(dest)) {
        return Path();
    }

    queue<pair<string, int>> q;
    unordered_map<string, int> dist;
    unordered_map<string, string> prev;
    unordered_map<string, int> stops;

    for (const auto& airport : airportToState) {
        dist[airport.first] = numeric_limits<int>::max();
    }

    dist[origin] = 0;
    q.push({origin, 0});

    while (!q.empty()) {
        pair<string, int> front = q.front();
        string cur = front.first;
        int curStops = front.second;
        q.pop();

        if (cur == dest) break;
        if (curStops >= maxStops) continue;

        for (const auto& flight : adjList[cur]) {
            if (dist[cur] + flight.distance < dist[flight.destination]) {
                dist[flight.destination] = dist[cur] + flight.distance;
                prev[flight.destination] = cur;
                stops[flight.destination] = curStops + 1;
                q.push({flight.destination, curStops + 1});
            }
        }
    }

    Path path;
    if (dist[dest] == numeric_limits<int>::max()) {
        return path;
    }

    for (string at = dest; !at.empty(); at = prev[at]) {
        path.air_code.push_back(at);
        if (at == origin) break;
    }
    
    reverse(path.air_code.begin(), path.air_code.end());
    
    for (size_t i = 1; i < path.air_code.size(); ++i) {
        string from = path.air_code[i-1];
        string to = path.air_code[i];
        for (const auto& flight : adjList[from]) {
            if (flight.destination == to) {
                path.totalDistance += flight.distance;
                path.totalCost += flight.cost;
                break;
            }
        }
    }
    
    return path;
}

vector<Connections> airlineGraph::countConnections() {
    vector<Connections> results;
    
    for (const auto& airport : airportToState) {
        Connections conn;
        conn.air_code = airport.first;
        conn.out = adjList[airport.first].size();
        
        for (const auto& other : adjList) {
            for (const auto& flight : other.second) {
                if (flight.destination == airport.first) {
                    conn.in++;
                }
            }
        }
        
        results.push_back(conn);
    }
    
    sort(results.begin(), results.end(), 
        [](const Connections& a, const Connections& b) {
            return a.totalConnections() > b.totalConnections();
        });
    
    return results;
}

void airlineGraph::createUndirectedGraph() {
    undirectedEdges.clear();
    unordered_map<string, unordered_map<string, int>> edgeCosts;
    
    for (const auto& from : adjList) {
        for (const auto& flight : from.second) {
            string u = min(from.first, flight.destination);
            string v = max(from.first, flight.destination);
            edgeCosts[u][v] = min(edgeCosts[u][v] == 0 ? numeric_limits<int>::max() : edgeCosts[u][v], 
                                flight.cost);
        }
    }
    
    for (const auto& u : edgeCosts) {
        for (const auto& v : u.second) {
            undirectedEdges.emplace_back(u.first, v.first, v.second);
        }
    }
}

pair<vector<mstEdge>, int> airlineGraph::primMST() {
    createUndirectedGraph();
    vector<mstEdge> mst;
    int totalCost = 0;
    
    if (undirectedEdges.empty()) {
        return {mst, totalCost};
    }
    
    unordered_map<string, int> key;
    unordered_map<string, string> parent;
    unordered_map<string, bool> inMST;
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
    
    for (const auto& airport : airportToState) {
        key[airport.first] = numeric_limits<int>::max();
        inMST[airport.first] = false;
    }
    
    string start = airportToState.begin()->first;
    key[start] = 0;
    pq.push({0, start});
    
    while (!pq.empty()) {
        string u = pq.top().second;
        pq.pop();
        
        if (inMST[u]) continue;
        inMST[u] = true;
        
        if (!parent[u].empty()) {
            mst.emplace_back(parent[u], u, key[u]);
            totalCost += key[u];
        }
        
        for (const auto& edge : undirectedEdges) {
            string v = (edge.from == u) ? edge.to : (edge.to == u ? edge.from : "");
            if (!v.empty() && !inMST[v] && edge.cost < key[v]) {
                key[v] = edge.cost;
                parent[v] = u;
                pq.push({key[v], v});
            }
        }
    }
    
    // Check if all vertices are included
    for (const auto& airport : airportToState) {
        if (!inMST[airport.first]) {
            cout << "Graph is disconnected. MST cannot be formed for all vertices." << endl;
            break;
        }
    }
    
    return {mst, totalCost};
}

pair<vector<mstEdge>, int> airlineGraph::kruskalMST() {
    createUndirectedGraph();
    sort(undirectedEdges.begin(), undirectedEdges.end(),
         [](const mstEdge& a, const mstEdge& b) { return a.cost < b.cost; });
    
    vector<mstEdge> mst;
    int totalCost = 0;
    
    DisjointSet ds(airportToState.size());
    unordered_map<string, int> indexMap;
    int index = 0;
    for (const auto& airport : airportToState) {
        indexMap[airport.first] = index++;
    }
    
    for (const auto& edge : undirectedEdges) {
        int u = indexMap[edge.from];
        int v = indexMap[edge.to];
        if (ds.find(u) != ds.find(v)) {
            ds.unionSet(u, v);
            mst.push_back(edge);
            totalCost += edge.cost;
        }
    }
    
    // Check for disconnected components
    int root = ds.find(0);
    bool disconnected = false;
    for (int i = 1; i < airportToState.size(); ++i) {
        if (ds.find(i) != root) {
            disconnected = true;
            break;
        }
    }
    if (disconnected) {
        cout << "Graph is disconnected. Returning minimum spanning forest." << endl;
    }
    
    return {mst, totalCost};
}