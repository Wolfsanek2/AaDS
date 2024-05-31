// Вариант 2. Для построения минимального остовного дерева используйте алгоритм Прима.
/*
#include <iostream>
#include <vector>
#include <stack>
#include <cassert>
#include <unordered_set>
#include <climits>
#include <set>
#include <sstream>

struct Pair {
    Pair(int vertex, int weight)
    : vertex(vertex), weight(weight) {}
    int vertex;
    int weight;
};

class PairComparator {
public:
    bool operator()(const Pair &pair_1, const Pair &pair_2) const {
        if (pair_1.weight < pair_2.weight) return true;
        if (pair_1.weight > pair_2.weight) return false;
        return pair_1.vertex < pair_2.vertex;
    }
};

struct Node {
    Node() = default;
    Node(const Pair &pair)
    : pair(pair) {}
    Pair pair;
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

    void AddEdge(int from, int to, int weight) {
        AddEdgePrivate(from, to, weight);
        AddEdgePrivate(to, from, weight);
    }
    int VerticesCount() const {
        return adjacency_lists.size();
    }
    std::vector<Pair> GetNextPairs(int vertex) const {
        std::vector<Pair> next_pairs;
        Node *node = adjacency_lists[vertex];
        while (node) {
            next_pairs.push_back(node->pair);
            node = node->next;
        }
        return next_pairs;
    }
private:
    void AddEdgePrivate(int from, int to, int weight) {
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
    std::vector<Node *> adjacency_lists;
};




int prima(const ListGraph &graph);
int get_tree_weight(const std::vector<int> &min_e);
void run(std::istream &input, std::ostream &output);
void test_run();
void test();

int main() {
    // test();
    run(std::cin, std::cout);

    return 0;
}

void run(std::istream &input, std::ostream &output) {
    int V = 0;
    input >> V;
    ListGraph graph(V);
    int E = 0;
    input >> E;
    for (int i = 0; i < E; i++) {
        int from = 0;
        input >> from;
        int to = 0;
        input >> to;
        int weight = 0;
        input >> weight;
        graph.AddEdge(from - 1, to - 1, weight);
    }
    output << prima(graph) << std::endl;
}

int prima(const ListGraph &graph) {
    std::vector<int> p(graph.VerticesCount(), -1);
    std::vector<int> min_e(graph.VerticesCount(), INT_MAX);
    std::vector<bool> visited(graph.VerticesCount(), false);
    min_e[0] = 0;
    std::set<Pair, PairComparator> priority_queue;
    priority_queue.insert({0, min_e[0]});
    while (!priority_queue.empty()) {
        Pair current_pair = *priority_queue.begin();
        priority_queue.erase(current_pair);
        visited[current_pair.vertex] = true;
        for (Pair next_pair: graph.GetNextPairs(current_pair.vertex)) {
            if (visited[next_pair.vertex]) continue;
            if (min_e[next_pair.vertex] == INT_MAX) {
                min_e[next_pair.vertex] = next_pair.weight;
                p[next_pair.vertex] = current_pair.vertex;
                priority_queue.insert(next_pair);
            }
            else if (next_pair.weight < min_e[next_pair.vertex]) {
                priority_queue.erase({next_pair.vertex, min_e[next_pair.vertex]});
                min_e[next_pair.vertex] = next_pair.weight;
                priority_queue.insert({next_pair.vertex, min_e[next_pair.vertex]});
                p[next_pair.vertex] = current_pair.vertex;
            }
        }
    }
    return get_tree_weight(min_e);
}

int get_tree_weight(const std::vector<int> &min_e) {
    int result = 0;
    for (int i = 0; i < min_e.size(); i++) {
        result += min_e[i];
    }
    return result;
}

void test_run() {
    {
        std::stringstream input, output;
        input << "4 4" << std::endl;
        input << "1 2 1" << std::endl;
        input << "2 3 2" << std::endl;
        input << "3 4 5" << std::endl;
        input << "4 1 4" << std::endl;
        run(input, output);
        std::string output_string = output.str();
        assert(output_string == "7\n");
    }
    {
        std::stringstream input, output;
        input << "5 10" << std::endl;
        input << "4 3 3046" << std::endl;
        input << "4 5 90110" << std::endl;
        input << "5 1 57786" << std::endl;
        input << "3 2 28280" << std::endl;
        input << "4 3 18010" << std::endl;
        input << "4 5 61367" << std::endl;
        input << "4 1 18811" << std::endl;
        input << "4 2 69898" << std::endl;
        input << "3 5 72518" << std::endl;
        input << "3 1 85838" << std::endl;
        run(input, output);
        std::string output_string = output.str();
        assert(output_string == "107923\n");
    }
    std::cout << "run test OK" << std::endl;
}

void test() {
    test_run();
}
*/