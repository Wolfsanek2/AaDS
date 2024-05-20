#include "ListGraph.h"

#include <stack>
#include <functional>
#include <iostream>

ListGraph::ListGraph(int size)
: adjacency_lists(size) {}

ListGraph::ListGraph(const IGraph &graph) {
    adjacency_lists.resize(graph.VerticesCount());
    for (int i = 0; i < adjacency_lists.size(); i++) {
        std::vector<int> next_vertices = graph.GetNextVertices(i);
        for (int j = 0; j < next_vertices.size(); j++) {
            AddEdge(i, j);
        }
    }
}

ListGraph::~ListGraph() {
    for (int i = 0; i < adjacency_lists.size(); i++) {
        Node *node = adjacency_lists[i];
        if (!node) continue;
        std::stack<Node *> stack;
        while (node->next) {
            stack.push(node);
            node = node->next;
        }
        stack.push(node);
        while (!stack.empty()) {
            node = stack.top();
            stack.pop();
            delete node;
        }
    }
}

void ListGraph::AddEdge(int from, int to) {
    Node *node = adjacency_lists[from];
    Node *new_node = new Node(to);
    if (!node) {
        adjacency_lists[from] = new_node;
        return;
    }
    while (node->next) {
        node = node->next;
    }
    node->next = new_node;
}

int ListGraph::VerticesCount() const {
    return adjacency_lists.size();
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    std::vector<int> next_vertices;
    Node *node = adjacency_lists[vertex];
    if (!node) {
        return std::vector<int> ();
    }
    next_vertices.push_back(node->data);
    while (node->next) {
        node = node->next;
        next_vertices.push_back(node->data);
    }
    return next_vertices;
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
    std::vector<int> prev_vertices;
    for (int i = 0; i < VerticesCount(); i++) {
        Node *node = adjacency_lists[i];
        while (node) {
            if (node->data == vertex) {
                prev_vertices.push_back(i);
            }
            node = node->next;
        }
    }
    return prev_vertices;
}