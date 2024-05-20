#include "SetGraph.h"

SetGraph::SetGraph(int size)
: hash_tables(size) {}

SetGraph::SetGraph(const IGraph &graph)
: hash_tables(graph.VerticesCount()){
    for (int i = 0; i < hash_tables.size(); i++) {
        for (int vertex: graph.GetNextVertices(i)) {
            hash_tables[i].insert(vertex);
        }
    }
}

SetGraph::~SetGraph() = default;

void SetGraph::AddEdge(int from, int to) {
    hash_tables[from].insert(to);
}

int SetGraph::VerticesCount() const {
    return hash_tables.size();
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    std::vector<int> array;
    for (int current_vertex: hash_tables[vertex]) {
        array.push_back(current_vertex);
    }
    return array;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    std::vector<int> array;
    for (int i = 0; i < VerticesCount(); i++) {
        if (hash_tables[i].contains(vertex)) {
            array.push_back(i);
        }
    }
    return array;
}