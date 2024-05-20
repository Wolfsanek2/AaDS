#include "ArcGraph.h"

ArcGraph::ArcGraph(int size)
: vertices_count(size) {}

ArcGraph::ArcGraph(const IGraph &graph) {
    vertices_count = graph.VerticesCount();
    for (int i = 0; i < graph.VerticesCount(); i++) {
        for (int vertex: graph.GetNextVertices(i)) {
            edges.push_back(std::pair<int, int>(i, vertex));
        }
    }
}

ArcGraph::~ArcGraph() = default;

void ArcGraph::AddEdge(int from, int to) {
    edges.push_back(std::pair<int, int>(from, to));
}

int ArcGraph::VerticesCount() const {
    return vertices_count;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    std::vector<int> array;
    for (auto pair: edges) {
        if (pair.first == vertex) {
            array.push_back(pair.second);
        }
    }
    return array;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    std::vector<int> array;
    for (auto pair: edges) {
        if (pair.second == vertex) {
            array.push_back(pair.first);
        }
    }
    return array;
}