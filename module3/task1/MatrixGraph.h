#include "IGraph.h"

#include <vector>

#ifndef MATRIX_GRAPH_H
#define MATRIX_GRAPH_H

class MatrixGraph: public IGraph {
public:
    MatrixGraph(int size);
    MatrixGraph(const IGraph &graph);
    ~MatrixGraph() override;
    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
private:
    std::vector<std::vector<bool>> adjacency_matrix;
};

#endif