#include <vector>
#include <stack>
#include <iostream>

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

bool DFS(const ListGraph &graph, std::vector<bool> &visited, std::vector<bool> &grey, std::vector<int> &parent, int start);
bool main_DFS(const ListGraph &graph);

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
    std::cout << static_cast<int>(main_DFS(graph)) << std::endl;

    return 0;
}

bool main_DFS(const ListGraph &graph) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::vector<bool> grey(graph.VerticesCount(), false);
    std::vector<int> parent(graph.VerticesCount(), -1);
    bool result = DFS(graph, visited, grey, parent, 0);
    for (int i = 0; i < visited.size(); i++) {
        if (!visited[i]) {
            result = false;
            break;
        }
    }
    return result;
}

bool DFS(const ListGraph &graph, std::vector<bool> &visited, std::vector<bool> &grey, std::vector<int> &parent, int start) {
    bool result = true;
    if (grey[start]) {
        return true;
    }
    grey[start] = true;
    for (int vertex : graph.GetNextVertices(start)) {
        if (!visited[vertex] && !grey[vertex]) {
            parent[vertex] = start;
            result = DFS(graph, visited, grey, parent, vertex);
        }
        else if (grey[vertex] && parent[start] != vertex) {
            return false;
        }
    }
    grey[start] = false;
    visited[start] = true;
    return result;
}