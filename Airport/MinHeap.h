#ifndef MINHEAP_H
#define MINHEAP_H

#include <vector>
#include <utility>
#include <stdexcept>

using namespace std;

template <typename T>
class MinHeap {
public:
    MinHeap() {}

    bool isEmpty() const { return heap.empty(); }

    void insert(const T& item) {
        heap.push_back(item);
        heapifyUp(heap.size() - 1);
    }

    T extractMin() {
        if (heap.empty()) {
            throw runtime_error("Heap is empty");
        }

        T minItem = heap[0];
        heap[0] = heap.back();
        heap.pop_back();

        if (!heap.empty()) {
            heapifyDown(0);
        }

        return minItem;
    }

private:
    vector<T> heap;

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index] < heap[parent]) {
                swap(heap[index], heap[parent]);
                index = parent;
            } else {
                break;
            }
        }
    }

    void heapifyDown(int index) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (left < heap.size() && heap[left] < heap[smallest]) {
            smallest = left;
        }

        if (right < heap.size() && heap[right] < heap[smallest]) {
            smallest = right;
        }

        if (smallest != index) {
            swap(heap[index], heap[smallest]);
            heapifyDown(smallest);
        }
    }
};


// Specialize for pair<int, int> to compare by first element

bool operator<(const pair<int, int>& lhs, const pair<int, int>& rhs) {
    return lhs.first < rhs.first;
}

#endif // MINHEAP_H