// Дан невзвешенный неориентированный граф. В графе может быть несколько кратчайших путей
// между какими-то вершинами. Найдите количество различных кратчайших путей между
// заданными вершинами.

#include <iostream>
#include <vector>
#include <queue>
#include <sstream>
#include <cassert>
#include <climits>
#include <stack>

struct Node {
    Node() = default;
    Node(int data)
    : data(data) {}
    int data = 0;
    Node *next = nullptr;
};

class ListGraph {
public:
    ListGraph(int size)
    : adjacency_lists(size) {}
    ~ListGraph() {
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

    void AddEdge(int from, int to) {
        AddEdgePrivate(from, to);
        AddEdgePrivate(to, from);
    }
    int VerticesCount() const {
        return adjacency_lists.size();
    }
    std::vector<int> GetNextVertices(int vertex) const {
        std::vector<int> next_vertices;
        Node *node = adjacency_lists[vertex];
        while (node) {
            next_vertices.push_back(node->data);
            node = node->next;
        }
        return next_vertices;
    }
    std::vector<int> GetPrevVertices(int vertex) const {
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
    int CountShortestWays(int from, int to) {
        std::vector<int> d(VerticesCount(), INT_MAX);
        std::vector<int> k(VerticesCount(), 0);
        std::vector<bool> visited(VerticesCount(), false);
        d[from] = 0;
        k[from] = 1;
        visited[from] = true;
        std::queue<int> queue;
        queue.push(from);
        while (!queue.empty()) {
            int vertex = queue.front();
            queue.pop();
            for (int next_vertex: GetNextVertices(vertex)) {
                if (!visited[next_vertex]) {
                    queue.push(next_vertex);
                    visited[next_vertex] = true;
                }
                if (d[vertex] + 1 < d[next_vertex]) {                    
                    d[next_vertex] = d[vertex] + 1;
                    k[next_vertex] = k[vertex];
                }
                else if (d[vertex] + 1 == d[next_vertex]) {
                    k[next_vertex] += k[vertex];
                }
            }
        }
        return k[to];
    }
private:
    void AddEdgePrivate(int from, int to) {
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
    std::vector<Node *> adjacency_lists;
};

void run(std::istream &input, std::ostream &output);
void test_run();
void test();

int main() {

    // test();
    run(std::cin, std::cout);

    return 0;
}

void run(std::istream &input, std::ostream &output) {
    int v = 0;
    input >> v;
    ListGraph graph(v);
    int n = 0;
    input >> n;
    for (int i = 0; i < n; i++) {
        int u, w = 0;
        input >> u >> w;
        graph.AddEdge(u, w);
    }
    int u, w = 0;
    input >> u >> w;
    output << graph.CountShortestWays(u, w) << std::endl;
}

void test_run() {
    {
        std::stringstream input, output;
        input << "4" << std::endl;
        input << "5" << std::endl;
        input << "0 1" << std::endl;
        input << "0 2" << std::endl;
        input << "1 2" << std::endl;
        input << "1 3" << std::endl;
        input << "2 3" << std::endl;
        input << "0 3" << std::endl;
        run(input, output);
        std::string output_string = output.str();
        assert(output_string == "2\n");
    }
    {
        std::stringstream input, output;
        input << "8" << std::endl;
        input << "9" << std::endl;
        input << "0 1" << std::endl;
        input << "0 2" << std::endl;
        input << "1 3" << std::endl;
        input << "2 3" << std::endl;
        input << "3 4" << std::endl;
        input << "4 5" << std::endl;
        input << "4 6" << std::endl;
        input << "5 7" << std::endl;
        input << "6 7" << std::endl;
        input << "0 7" << std::endl;
        run(input, output);
        std::string output_string = output.str();
        assert(output_string == "4\n");
    }
    {
        std::stringstream input, output;
        input << "6" << std::endl;
        input << "8" << std::endl;
        input << "0 1" << std::endl;
        input << "0 2" << std::endl;
        input << "0 3" << std::endl;
        input << "0 4" << std::endl;
        input << "1 5" << std::endl;
        input << "2 5" << std::endl;
        input << "3 5" << std::endl;
        input << "4 5" << std::endl;
        input << "0 5" << std::endl;
        run(input, output);
        std::string output_string = output.str();
        assert(output_string == "4\n");
    }
    std::cout << "run test OK" << std::endl;
}

void test() {
    test_run();
}