#include "IGraph.h"
#include "Node.h"

#include <vector>

#ifndef LIST_GRAPH_H
#define LIST_GRAPH_H

class ListGraph: public IGraph {
public:
    ListGraph(int size);
    ListGraph(const IGraph &graph);
    ~ListGraph() override;
    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
private:
    std::vector<Node *> adjacency_lists;
};

#endif