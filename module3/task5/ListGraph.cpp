#include "ListGraph.h"

ListGraph::ListGraph(int size)
: adjacency_lists(size) {}


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

void ListGraph::AddEdge(int from, int to, double weight) {
    AddEdgePrivate(from, to, weight);
    AddEdgePrivate(to, from, weight);
}

int ListGraph::VerticesCount() const {
    return adjacency_lists.size();
}

std::vector<Pair> ListGraph::GetNextPairs(int vertex) const {
    std::vector<Pair> next_pairs;
    Node *node = adjacency_lists[vertex];
    while (node) {
        next_pairs.push_back(node->pair);
        node = node->next;
    }
    return next_pairs;
}

void ListGraph::AddEdgePrivate(int from, int to, double weight) {
    Node *node = adjacency_lists[from];
    Node *new_node = new Node({to, weight});
    if (!node) {
        adjacency_lists[from] = new_node;
        return;
    }
    while (node->next) {
        node = node->next;
    }
    node->next = new_node;
}

double ListGraph::GetWeight(int from, int to) const {
    Node *current_node = adjacency_lists[from];
    while (current_node && current_node->pair.vertex != to) {
        current_node = current_node->next;
    }
    return current_node->pair.weight;
}