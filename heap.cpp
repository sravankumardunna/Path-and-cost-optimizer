#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

template <typename T>
class Heap {
private:
    std::vector<T> data;
    std::unordered_map<T, int> map;

    void swap(int i, int j) {
        std::swap(data[i], data[j]);
        map[data[i]] = i;
        map[data[j]] = j;
    }

    void upheapify(int ci) {
        int pi = (ci - 1) / 2;
        if (ci > 0 && isLarger(data[ci], data[pi]) > 0) {
            swap(pi, ci);
            upheapify(pi);
        }
    }

    void downheapify(int pi) {
        int lci = 2 * pi + 1;
        int rci = 2 * pi + 2;
        int mini = pi;

        if (lci < data.size() && isLarger(data[lci], data[mini]) > 0) {
            mini = lci;
        }
        
        if (rci < data.size() && isLarger(data[rci], data[mini]) > 0) {
            mini = rci;
        }
        
        if (mini != pi) {
            swap(mini, pi);
            downheapify(mini);
        }
    }

public:
    void add(const T& item) {
        data.push_back(item);
        map[item] = data.size() - 1;
        upheapify(data.size() - 1);
    }

    T remove() {
        swap(0, data.size() - 1);
        T rv = data.back();
        data.pop_back();
        map.erase(rv);
        if (!data.empty()) {
            downheapify(0);
        }
        return rv;
    }

    T get() const {
        if (!data.empty()) {
            return data[0];
        }
        throw std::runtime_error("Heap is empty");
    }

    void updatePriority(const T& pair) {
        auto it = map.find(pair);
        if (it != map.end()) {
            upheapify(it->second);
        }
    }

    bool isEmpty() const {
        return data.empty();
    }

    int size() const {
        return data.size();
    }

    void display() const {
        for (const auto& item : data) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }

private:
    int isLarger(const T& t, const T& o) const {
        return t < o ? -1 : (t > o ? 1 : 0);
    }
};

int main() {
    // Example usage of Heap class with integers
    Heap<int> heap;
    heap.add(10);
    heap.add(20);
    heap.add(5);
    heap.add(7);
    
    heap.display();

    std::cout << "Removed: " << heap.remove() << std::endl;
    heap.display();
    
    return 0;
}

