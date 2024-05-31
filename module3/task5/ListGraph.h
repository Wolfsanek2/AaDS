#include "Pair.h"
#include "Node.h"

#include <vector>
#include <stack>

class ListGraph {
public:
    ListGraph(int size);
    ~ListGraph();
    void AddEdge(int from, int to, double weight);
    int VerticesCount() const;
    std::vector<Pair> GetNextPairs(int vertex) const;
    double GetWeight(int from, int to) const;
private:
    void AddEdgePrivate(int from, int to, double weight);
    std::vector<Node *> adjacency_lists;
};