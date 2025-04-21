#include "AirGraph.h"
//#include "MinHeap.h"
//#include "MinHeap.cpp"

#include <queue>
#include<unistd.h>
#include <iostream>
#include "AirGraph.h"

#include "../MinHeap.h"
#include "MinHeap.h"
#include "MinHeap.cpp"

#define INT_MAX 1000
using namespace std;

template <typename T>
void AirGraph<T>::insert_airport(const Airport<T>& air) {
    if (get_airport_index(air)== -1) {
        airports.push_back(air); //insert the airport to the array of airports
        vector<Travel> tmp;
        travels.push_back(tmp); //insert empty vector to the edges
    }
}
template<typename T>
int AirGraph<T>::get_airport_index(const Airport<T> &air) {
    for (int i = 0; i < airports.size(); i++) {
        if (airports[i].getData() == air.getData()) {
            return i;
        }

    }
    return -1;
}

template <typename T>
void AirGraph<T>::add_travel(const Airport<T>& air1, const Airport<T>& air2, int distance, int cost) {
    int i1 = get_airport_index(air1);
    int i2 = get_airport_index(air2);
    if (i1 == -1 || i2 == -1) {
        throw string("Add travel: incorrect vertices");
    }
    Travel v(i1, i2, distance, cost);
    Travel v2(i1, i2, distance, cost);
    travels[i1].push_back(v);
    if (i1 != i2 ) {
        travels[i2].push_back(v2);
    }

}

template <typename T>
void AirGraph<T>::print() const {
    for (int i = 0; i < airports.size(); i++) {
        cout << "{ " << airports[i].getData() << ", ";
        for (int j =0; j < travels[i].size(); j++) {
            cout << '}' << airports[travels[i][j].dest].getData() << ", ";
            cout << travels[i][j].distance << travels[i][j].cost << "} ";
        }
        cout << " }\n";
    }
}

template <typename T>
void AirGraph<T>::DFS(Airport<T>& air) {
    clean_visited();
    DFS_helper(air);
    clean_visited();
}

template <typename T>
void print_queue(queue<Airport<T>> q) {
    while (!q.empty()) {
        cout << q.front().getData() << ", ";
        q.pop();
    }
    cout << endl;
}

template <typename T>
void AirGraph<T>::BFS(Airport<T>& ver) {
    clean_visited();

    int i = get_airport_index(ver);
    if (i == -1){
        throw string("BFS: AIRPORT is not in the graph");
    }
    queue<int> q;
    q.push(i);
    airports[i].setVisited(true);

    while (!q.empty()) {
        int i = q.front();
        cout << airports[i].getData() << ' ';
        for (int j = 0; j < travels[i].size(); j++) {
            int adjacent_ver = travels[i][j].dest;
            if (airports[adjacent_ver].getVisited() == false) {
                airports[adjacent_ver].getVisited(true);
                q.push(adjacent_ver);
            }
        }
        q.pop();
    }
    clean_visited();
}

template <typename T>
void AirGraph<T>::clean_visited() {
    for (Airport<T>& v : airports) {
        v.setVisited(false);
    }
}

template<typename T>
void AirGraph<T>::DFS_helper(Airport<T> &air) {
    int i = get_airport_index(air);
    if (i == -1) {
        throw string("DFS: AIRPORT is not in the graph");
    }
    airports[i].setVisited(true);
    cout << airports[i].getData() << ' ';
    for (int j = 0; j < travels[i].size(); j++) {
        Airport<int> adjacent_ver = airports[travels[i][j].dest];
        if (adjacent_ver.getVisited()==false) {
            DFS_helper(adjacent_ver);
        }
    }
}

template<typename T>
int AirGraph<T>::dijkstra_shortest_path(const Airport<T> &org, const Airport<T> &dest) {
    int i_org = get_airport_index(org);
    int i_dest = get_airport_index(dest);
    vector<int>distances(airports.size());

    for (int i = 0; i<distances.size(); i++) {
        distances[i] = (i == i_org) ? 0 : INT_MAX;
    }
    MinHeap<Travel> heap;
    int airports_visited = 0;
    int cur_ver = i_org;

    while (airports_visited < airports.size()) {
        int i = cur_ver;
        for (int j = 0; j < travels[i].size(); j++) {
            int i_adjacent_air = travels[i][j].dest;
            if (airports[i_adjacent_air].getVisited() == false) {
                heap.insert(travels[i][j]);

                int dist_from_org = distances[i] + travels[i][j].distance;
                if (dist_from_org < distances[i_adjacent_air]) {
                    distances[i_adjacent_air] = dist_from_org;
                }
            }
        }
    }
}




