#ifndef AIRPORT_H
#define AIRPORT_H

template <typename T>
class Airport {
public:
    Airport(const T& c = T(), const T& s = T()) : code(c), state(s), visited(false) {}

    const T& getData() const { return code; }
    const T& getState() const { return state; }
    bool getVisited() const { return visited; }
    void setVisited(bool v) { visited = v; }

private:
    T code;
    T state;
    bool visited;
};

#endif // AIRPORT_H