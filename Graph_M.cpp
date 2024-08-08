#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <string>
#include <sstream>
#include <limits>
#include <cmath>
#include <set>
#include <list>

using namespace std;

// Class to represent a vertex in the graph
class Vertex {
public:
    unordered_map<string, int> nbrs;
};

// Class to represent the graph
class Graph_M {
private:
    unordered_map<string, Vertex> vtces;

public:
    // Constructor
    Graph_M() {}

    // Function to get the number of vertices
    int numVertex() {
        return vtces.size();
    }

    // Function to check if a vertex exists
    bool containsVertex(const string& vname) {
        return vtces.find(vname) != vtces.end();
    }

    // Function to add a vertex
    void addVertex(const string& vname) {
        Vertex vtx;
        vtces[vname] = vtx;
    }

    // Function to remove a vertex
    void removeVertex(const string& vname) {
        Vertex vtx = vtces[vname];
        vector<string> keys;

        for (const auto& kv : vtx.nbrs) {
            keys.push_back(kv.first);
        }

        for (const auto& key : keys) {
            Vertex& nbrVtx = vtces[key];
            nbrVtx.nbrs.erase(vname);
        }

        vtces.erase(vname);
    }

    // Function to get the number of edges
    int numEdges() {
        int count = 0;

        for (const auto& kv : vtces) {
            count += kv.second.nbrs.size();
        }

        return count / 2;
    }

    // Function to check if an edge exists
    bool containsEdge(const string& vname1, const string& vname2) {
        if (vtces.find(vname1) == vtces.end() || vtces.find(vname2) == vtces.end()) {
            return false;
        }
        return vtces[vname1].nbrs.find(vname2) != vtces[vname1].nbrs.end();
    }

    // Function to add an edge
    void addEdge(const string& vname1, const string& vname2, int value) {
        if (containsEdge(vname1, vname2)) return;

        vtces[vname1].nbrs[vname2] = value;
        vtces[vname2].nbrs[vname1] = value;
    }

    // Function to remove an edge
    void removeEdge(const string& vname1, const string& vname2) {
        if (!containsEdge(vname1, vname2)) return;

        vtces[vname1].nbrs.erase(vname2);
        vtces[vname2].nbrs.erase(vname1);
    }

    // Function to display the map
    void displayMap() {
        cout << "\t Metro Map" << endl;
        cout << "\t------------------" << endl;
        cout << "----------------------------------------------------\n";

        for (const auto& kv : vtces) {
            const string& key = kv.first;
            const Vertex& vtx = kv.second;
            cout << key << " =>" << endl;

            for (const auto& nbr : vtx.nbrs) {
                cout << "\t" << nbr.first;
                if (nbr.first.length() < 16) cout << "\t";
                if (nbr.first.length() < 8) cout << "\t";
                cout << "\t" << nbr.second << endl;
            }
        }

        cout << "\t------------------" << endl;
        cout << "---------------------------------------------------\n";
    }

    // Function to display the stations
    void displayStations() {
        cout << "\n***********************************************************************\n";
        int i = 1;
        for (const auto& kv : vtces) {
            cout << i << ". " << kv.first << endl;
            i++;
        }
        cout << "\n***********************************************************************\n";
    }

    // Function to check if there is a path between two vertices
    bool hasPath(const string& vname1, const string& vname2, unordered_map<string, bool>& processed) {
        if (containsEdge(vname1, vname2)) return true;

        processed[vname1] = true;
        const Vertex& vtx = vtces[vname1];

        for (const auto& nbr : vtx.nbrs) {
            if (processed.find(nbr.first) == processed.end()) {
                if (hasPath(nbr.first, vname2, processed)) return true;
            }
        }

        return false;
    }

    // Helper class for Dijkstra's algorithm
    class DijkstraPair {
    public:
        string vname;
        string psf;
        int cost;

        bool operator<(const DijkstraPair& other) const {
            return cost > other.cost;
        }
    };

    // Function to find the shortest distance using Dijkstra's algorithm
    int dijkstra(const string& src, const string& des, bool nan) {
        int val = 0;
        unordered_map<string, DijkstraPair> map;
        priority_queue<DijkstraPair> heap;

        for (const auto& kv : vtces) {
            DijkstraPair np;
            np.vname = kv.first;
            np.cost = numeric_limits<int>::max();

            if (kv.first == src) {
                np.cost = 0;
                np.psf = kv.first;
            }

            heap.push(np);
            map[kv.first] = np;
        }

        while (!heap.empty()) {
            DijkstraPair rp = heap.top();
            heap.pop();

            if (rp.vname == des) {
                val = rp.cost;
                break;
            }

            map.erase(rp.vname);

            const Vertex& v = vtces[rp.vname];
            for (const auto& nbr : v.nbrs) {
                if (map.find(nbr.first) != map.end()) {
                    int oc = map[nbr.first].cost;
                    int nc;
                    if (nan)
                        nc = rp.cost + 120 + 40 * nbr.second;
                    else
                        nc = rp.cost + nbr.second;

                    if (nc < oc) {
                        DijkstraPair gp = map[nbr.first];
                        gp.psf = rp.psf + nbr.first;
                        gp.cost = nc;

                        heap.push(gp);
                        map[nbr.first] = gp;
                    }
                }
            }
        }

        return val;
    }

    // Helper class for shortest path computation
    class Pair {
    public:
        string vname;
        string psf;
        int min_dis;
        int min_time;
    };

    // Function to get minimum distance
    string getMinimumDistance(const string& src, const string& dst) {
        int min = numeric_limits<int>::max();
        string ans = "";
        unordered_map<string, bool> processed;
        list<Pair> stack;

        Pair sp;
        sp.vname = src;
        sp.psf = src + "  ";
        sp.min_dis = 0;
        sp.min_time = 0;

        stack.push_front(sp);

        while (!stack.empty()) {
            Pair rp = stack.front();
            stack.pop_front();

            if (processed.find(rp.vname) != processed.end()) {
                continue;
            }

            processed[rp.vname] = true;

            if (rp.vname == dst) {
                int temp = rp.min_dis;
                if (temp < min) {
                    ans = rp.psf;
                    min = temp;
                }
                continue;
            }

            const Vertex& rpvtx = vtces[rp.vname];
            for (const auto& nbr : rpvtx.nbrs) {
                if (processed.find(nbr.first) == processed.end()) {
                    Pair np;
                    np.vname = nbr.first;
                    np.psf = rp.psf + nbr.first + "  ";
                    np.min_dis = rp.min_dis + nbr.second;
                    np.min_time = rp.min_time;
                    stack.push_front(np);
                }
            }
        }

        ans += to_string(min);
        return ans;
    }

    // Function to get minimum time
    string getMinimumTime(const string& src, const string& dst) {
        int min = numeric_limits<int>::max();
        string ans = "";
        unordered_map<string, bool> processed;
        list<Pair> stack;

        Pair sp;
        sp.vname = src;
        sp.psf = src + "  ";
        sp.min_dis = 0;
        sp.min_time = 0;

        stack.push_front(sp);

        while (!stack.empty()) {
            Pair rp = stack.front();
            stack.pop_front();

            if (processed.find(rp.vname) != processed.end()) {
                continue;
            }

            processed[rp.vname] = true;

            if (rp.vname == dst) {
                int temp = rp.min_time;
                if (temp < min) {
                    ans = rp.psf;
                    min = temp;
                }
                continue;
            }

            const Vertex& rpvtx = vtces[rp.vname];
            for (const auto& nbr : rpvtx.nbrs) {
                if (processed.find(nbr.first) == processed.end()) {
                    Pair np;
                    np.vname = nbr.first;
                    np.psf = rp.psf + nbr.first + "  ";
                    np.min_dis = rp.min_dis;
                    np.min_time = rp.min_time + nbr.second;
                    stack.push_front(np);
                }
            }
        }

        ans += to_string(min);
        return ans;
    }
};

int main() {
    Graph_M g;
    
    // Add code to populate the graph and use the various methods
    // Example:
    g.addVertex("A");
    g.addVertex("B");
    g.addEdge("A", "B", 5);

    g.displayMap();
    
    return 0;
}
