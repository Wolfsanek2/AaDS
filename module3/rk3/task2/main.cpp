#include <vector>
#include <stack>
#include <iostream>
#include <queue>

struct Node {
    Node() = default;
    Node(int data)
    : data(data) {}
    int data = 0;
    Node *next = nullptr;
};

class ListGraph {
public:
    ListGraph(int size);
    ~ListGraph();
    void AddEdge(int from, int to);
    int VerticesCount() const;
    std::vector<int> GetNextVertices(int vertex) const;
    std::vector<int> GetPrevVertices(int vertex) const;
private:
    void AddEdgeInternal(int from, int to);
    std::vector<Node *> adjacency_lists;
};

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

void ListGraph::AddEdge(int from, int to) {
    AddEdgeInternal(from, to);
    AddEdgeInternal(to, from);
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

void ListGraph::AddEdgeInternal(int from, int to) {
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

bool BFS(const ListGraph &graph);

int main() {

    int V = 0;
    std::cin >> V;
    int E = 0;
    std::cin >> E;
    ListGraph graph(V);
    for (int i = 0; i < E; i++) {
        int from = 0;
        std::cin >> from;
        int to = 0;
        std::cin >> to;
        graph.AddEdge(from, to);
    }
    std::cout << static_cast<int>(BFS(graph)) << std::endl;

    return 0;
}

bool BFS(const ListGraph &graph) {
    std::queue<int> queue;
    std::vector<bool> visited(graph.VerticesCount(), false);
    visited[0] = true;
    queue.push(0);
    while (!queue.empty()) {
        int vertex = queue.front();
        if (graph.GetNextVertices(vertex).size() % 2) {
            return false;
        }
        queue.pop();
        for (int next_vertex : graph.GetNextVertices(vertex)) {
            if (!visited[next_vertex]) {
                queue.push(next_vertex);
                visited[next_vertex] = true;
            }
        }
    }
    return true;
}