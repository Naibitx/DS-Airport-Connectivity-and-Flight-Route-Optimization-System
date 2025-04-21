//
// Created by maste on 4/20/2025.
//
#ifndef TRAVEL_H
#define TRAVEL_H

class Travel {
public:
    int origin;
    int dest;
    int distance;
    int cost;

    Travel(int o, int d, int dist, int c) : origin(o), dest(d), distance(dist), cost(c) {}

    int getOther(int airportIndex) const {
        if (airportIndex == origin) return dest;
        if (airportIndex == dest) return origin;
        return -1;
    }
};

#endif // TRAVEL_H