#include "MatrixGraph.h"

MatrixGraph::MatrixGraph(int size)
: adjacency_matrix(size, std::vector<bool>(size, false)) {}

MatrixGraph::MatrixGraph(const IGraph &graph)
: adjacency_matrix(graph.VerticesCount(), std::vector<bool>(graph.VerticesCount(), false)) {
    for (int i = 0; i < graph.VerticesCount(); i++) {
        for (int vertex: graph.GetNextVertices(i)) {
            adjacency_matrix[i][vertex] = true;
        }
    }
}

MatrixGraph::~MatrixGraph() = default;

void MatrixGraph::AddEdge(int from, int to) {
    adjacency_matrix[from][to] = true;
}

int MatrixGraph::VerticesCount() const {
    return adjacency_matrix.size();
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    std::vector<int> array;
    for (int i = 0; i < VerticesCount(); i++) {
        if (adjacency_matrix[vertex][i]) {
            array.push_back(i);
        }
    }
    return array;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    std::vector<int> array;
    for (int i = 0; i < VerticesCount(); i++) {
        if (adjacency_matrix[i][vertex]) {
            array.push_back(i);
        }
    }
    return array;
}