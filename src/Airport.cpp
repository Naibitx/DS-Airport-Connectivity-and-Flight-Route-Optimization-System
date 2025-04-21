#include "airlineGraph.h"
#include <sstream>
#include <algorithm>
#include <limits>
#include <queue>

using namespace std;

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
        return cityState.substr(comma + 2);
    }
    return "";
}

void airlineGraph::readCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string originAir, destAir, originCityState, destCityState, distanceStr, costStr;
        
        getline(ss, originAir, ',');
        getline(ss, destAir, ',');
        getline(ss, originCityState, ',');
        getline(ss, destCityState, ',');
        getline(ss, distanceStr, ',');
        getline(ss, costStr, ',');

        string originState = extractState(originCityState);
        string destState = extractState(destCityState);

        if (originState.front() == '"') originState = originState.substr(1, originState.size() - 2);
        if (destState.front() == '"') destState = destState.substr(1, destState.size() - 2);

        addAirportNode(originAir, originState);
        addAirportNode(destAir, destState);
        addFlightEdge(originAir, destAir, stoi(distanceStr), stoi(costStr));
    }
    file.close();
}

Path airlineGraph::dijkstraPath(const string& origin, const string& dest, bool useCost) {
    if (!airportExists(origin) || !airportExists(dest)) {
        return Path();
    }

    unordered_map<string, int> dist;
    unordered_map<string, string> prev;
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;

    for (const auto& airport : airportToState) {
        dist[airport.first] = numeric_limits<int>::max();
    }
    
    dist[origin] = 0;
    pq.push({0, origin});

    while (!pq.empty()) {
        auto [curDist, cur] = pq.top();
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
    
    // Calculate total distance and cost
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
        auto [cur, curStops] = q.front();
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
    
    // Calculate total distance and cost
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
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;
    
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
    
    return {mst, totalCost};
  }