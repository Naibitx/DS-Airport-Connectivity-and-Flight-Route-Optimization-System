#include "airlineGraph.h"
#include "../include/FlightEdge.h"
#include"../include/Airport.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <limits>
#include <stack>

using namespace std;

DisjointSet::DisjointSet(int size) {
    parent.resize(size);
    rank.resize(size, 0);
    for (int i = 0; i < size; ++i) {
        parent[i] = i;
    }
}

int DisjointSet::find(int v) {
    if (parent[v] != v) {
        parent[v] = find(parent[v]); // Path compression
    }
    return parent[v];
}

void DisjointSet::unionSet(int u, int v) {
    int rootU = find(u);
    int rootV = find(v);

    if (rootU != rootV) {
        if (rank[rootU] > rank[rootV]) {
            parent[rootV] = rootU;
        } else if (rank[rootU] < rank[rootV]) {
            parent[rootU] = rootV;
        } else {
            parent[rootV] = rootU;
            rank[rootU]++;
        }
    }
}

airlineGraph::airlineGraph() {}

void airlineGraph::addAirportNode(const std::string& air_code, const std::string& state_code) {
    if (!airportExists(air_code)) {
        nodes.push_back(GraphNode(air_code, state_code));
    }
}

bool airlineGraph::airportExists(const std::string& air_code) const {
    return std::any_of(nodes.begin(), nodes.end(), [&](const GraphNode& node) {
        return node.air_code == air_code;
    });
}

int airlineGraph::getIndex(const std::string& air_code) const {
    for (int i = 0; i < nodes.size(); ++i) {
        if (nodes[i].airport_code == air_code) return i;
    }
    return -1;
}

void airlineGraph::addFlightEdge(const std::string& origin, const std::string& dest, int dist, int cost) {
    addAirportNode(origin, "");
    addAirportNode(dest, "");

    int originIndex = getIndex(origin);
    nodes[originIndex].adjList.emplace_back(dest, dist, cost);

    int destIndex = getIndex(dest);
    nodes[destIndex].adjList.emplace_back(origin, dist, cost); // Undirected edge
}

using namespace std;

//extracting city and state
string extractState(const string& cityState){

    size_t comma=cityState.find(",");//finding position of comma
    if (comma != string::npos && comma +2 < cityState.length()) {
        return cityState.substr(comma + 2); //skip comma and space
    }
    return "";//if no state is found return empty string
};
//reading csv file
void airlineGraph::readCSV(const string& filename){
    ifstream file(filename);//open the cvs file
    if (!file.is_open() ){
        cerr << "Error opening file: " << filename << endl;//chekc if the file opened good 
        return;
    }

    string line;
    getline(file, line); //this will skip the header line

    while (getline(file,line)){
        stringstream strgstrm(line);// create string to go through the current line
        string originAir, destAir;
        string originState, destState;
        string distanceLine, costLine;
        //read every comma separated value in the line
        getline(strgstrm, originAir, ',');
        getline(strgstrm, destAir, ','); 
        getline(strgstrm, originState, ',');
        getline(strgstrm, destState, ',');

        string originState = extractState(originState);  // Extracts state from origin city/state
        string destState = extractState(destState); 

        getline(strgstrm, distanceLine, ',');
        getline(strgstrm, costLine, ',');   

        //remove extra quotes from the state names
        if (originState.front() == '"') originState = originState.substr(1, originState.size() - 2);
        if (destState.front() == '"') destState = destState.substr(1, destState.size() - 2);

        airportToState[originAir] = originState;// hold the relaitonship between airport code and its state
        airportToState[destAir] = destState;

        //creates flight object for informaiton about flight routes
        Flight flight;
        flight.destination= destAir;
        flight.distance = stoi(distanceLine);
        flight.cost = stoi(costLine);

        adjList[originAir].push_back(flight);//map holds list of files
    }

    file.close();
}
Path airlineGraph::dijkstraPath(const std::string& origin, const std::string& dest, bool useCost) {
        int src = getIndex(origin);
        int dst = getIndex(dest);
        int n = nodes.size();
    
        std::vector<int> dist(n, std::numeric_limits<int>::max());
        std::vector<std::string> prev(n, "");
        dist[src] = 0;
    
        using pii = std::pair<int, std::string>;
        std::priority_queue<pii, std::vector<pii>, std::greater<>> pq;
        pq.push({0, origin});
    
        while (!pq.empty()) {
            auto [curDist, curCode] = pq.top(); pq.pop();
            int u = getIndex(curCode);
            if (u == -1) continue;
    
            for (const auto& adj : nodes[u].adjList) {
                int v = getIndex(adj.destination);
                int weight = useCost ? adj.cost : adj.distance;
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    prev[v] = curCode;
                    pq.push({dist[v], adj.destination});
                }
            }
        }
    
        Path path;
        if (dist[dst] == std::numeric_limits<int>::max()) return path;
    
        for (std::string at = dest; !at.empty(); at = prev[getIndex(at)]) {
            path.air_code.push_back(at);
            if (at == origin) break;
        }
    
        std::reverse(path.air_code.begin(), path.air_code.end());
        path.totalCost = useCost ? dist[dst] : 0;
        path.totalDistance = useCost ? 0 : dist[dst];
        return path;
}
    
    std::vector<Path> airlineGraph::shortestPathsToState(const std::string& origin, const std::string& state, bool useCost) {
        std::vector<Path> result;
        for (const auto& node : nodes) {
            if (node.state_code == state) {
                result.push_back(dijkstraPath(origin, node.airport_code, useCost));
            }
        }
        return result;
}
    
    Path airlineGraph::shortestPathWithStops(const std::string& origin, const std::string& dest, int maxStops) {
        int src = getIndex(origin);
        int dst = getIndex(dest);
        int n = nodes.size();
    
        std::vector<int> dist(n, std::numeric_limits<int>::max());
        std::vector<std::string> prev(n, "");
        std::vector<int> stops(n, 0);
    
        dist[src] = 0;
        std::queue<int> q;
        q.push(src);
    
        while (!q.empty()) {
            int u = q.front(); q.pop();
            if (stops[u] >= maxStops) continue;
    
            for (const auto& adj : nodes[u].AdjList) {
                int v = getIndex(adj.destination);
                if (dist[u] + adj.distance < dist[v]) {
                    dist[v] = dist[u] + adj.distance;
                    prev[v] = nodes[u].airport_code;
                    stops[v] = stops[u] + 1;
                    q.push(v);
                }
            }
        }
    
        Path path;
        if (dist[dst] == std::numeric_limits<int>::max()) return path;
    
        for (std::string at = dest; !at.empty(); at = prev[getIndex(at)]) {
            path.air_code.push_back(at);
            if (at == origin) break;
        }
    
        std::reverse(path.air_code.begin(), path.air_code.end());
        path.totalDistance = dist[dst];
        return path;
    }
    
std::vector<Connections> airlineGraph::countConnections() const {
        std::vector<Connections> results;
        for (const auto& node : nodes) {
            Connections conn;
            conn.air_code = node.airport_code;
            conn.out = node.adjList.size();
    
            for (const auto& other : nodes) {
                for (const auto& adj : other.adjList) {
                    if (adj.destination == node.airport_code) conn.in++;
                }
            }
    
            results.push_back(conn);
        }
        return results;
}
    
    void airlineGraph::createUndirectedGraph() {
        undirectedEdges.clear();
        for (const auto& node : nodes) {
            for (const auto& adj : node.AdjList) {
                if (node.airport_code < adj.destination) {
                    undirectedEdges.emplace_back(node.airport_code, adj.destination, adj.cost);
                }
            }
        }
}
    
    std::pair<std::vector<mstEdge>, int> airlineGraph::primMST() {
        createUndirectedGraph();
        int n = nodes.size();
        std::vector<bool> inMST(n, false);
        std::vector<int> key(n, std::numeric_limits<int>::max());
        std::vector<std::string> parent(n);
        int totalCost = 0;
        std::vector<mstEdge> mst;
    
        key[0] = 0;
    
        for (int i = 0; i < n; ++i) {
            int u = -1;
            for (int j = 0; j < n; ++j) {
                if (!inMST[j] && (u == -1 || key[j] < key[u])) {
                    u = j;
                }
            }
    
            inMST[u] = true;
            totalCost += key[u];
    
            if (!parent[u].empty()) {
                mst.emplace_back(parent[u], nodes[u].airport_code, key[u]);
            }
    
            for (const auto& adj : nodes[u].adjList) {
                int v = getIndex(adj.destination);
                if (!inMST[v] && adj.cost < key[v]) {
                    key[v] = adj.cost;
                    parent[v] = nodes[u].airport_code;
                }
            }
        }
    
        return {mst, totalCost};
    }
    
    std::pair<std::vector<mstEdge>, int> airlineGraph::kruskalMST() {
        createUndirectedGraph();
        std::sort(undirectedEdges.begin(), undirectedEdges.end(), [](const mstEdge& a, const mstEdge& b) {
            return a.cost < b.cost;
        });
    
        DisjointSet ds(nodes.size());
        std::unordered_map<std::string, int> indexMap;
        for (int i = 0; i < nodes.size(); ++i) {
            indexMap[nodes[i].airport_code] = i;
        }
    
        std::vector<mstEdge> mst;
        int totalCost = 0;
    
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

