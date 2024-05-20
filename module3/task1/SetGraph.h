#include "IGraph.h"

#include <vector>
#include <unordered_set>

#ifndef SET_GRAPH_H
#define SET_GRAPH_H

class SetGraph: public IGraph {
public:
    SetGraph(int size);
    SetGraph(const IGraph &graph);
    ~SetGraph() override;
    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
private:
    std::vector<std::unordered_set<int>> hash_tables;
};

#endif