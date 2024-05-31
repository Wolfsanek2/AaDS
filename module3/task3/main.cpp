// 3. Требуется отыскать самый выгодный маршрут между городами.
// Требования: время работы O((N+M)logN), где N-количество городов, M-известных дорог между
// ними.
// Формат входных данных.
// Первая строка содержит число N – количество городов.
// Вторая строка содержит число M - количество дорог.
// Каждая следующая строка содержит описание дороги (откуда, куда, время в пути).
// Последняя строка содержит маршрут (откуда и куда нужно доехать).
// Формат выходных данных.
// Вывести длину самого выгодного маршрута.

#include <iostream>
#include <vector>
#include <stack>
#include <climits>
#include <set>
#include <sstream>
#include <cassert>
#include <unordered_set>

class PairComparator {
public:
    bool operator()(const std::pair<int, int> pair_1, const std::pair<int, int> pair_2) const {
        if (pair_1.second < pair_2.second) return true;
        if (pair_1.second > pair_2.second) return false;
        return pair_1.first < pair_2.first;
    }
};

class PairHasher {
public:
    size_t operator()(const std::pair<int, int> &pair) const {
        return pair.first * 51 + pair.second * 73;
    }
};

class SetGraph {
public:
    SetGraph(int size);
    ~SetGraph();
    void AddEdge(int from, int to, int weight);
    int VerticesCount() const;
    std::vector<Pair> GetNextPairs(int vertex) const;
private:
    std::vector<std::unordered_set<std::pair<int, int>, PairHasher>> hash_tables;
};

SetGraph::SetGraph(int size)
: hash_tables(size) {}

SetGraph::~SetGraph() = default;

void SetGraph::AddEdge(int from, int to, int weight) {
    hash_tables[from].insert({to, weight});
    hash_tables[to].insert({from, weight});
}

int SetGraph::VerticesCount() const {
    return hash_tables.size();
}

std::vector<std::pair<int, int>> SetGraph::GetNextPairs(int vertex) const {
    std::vector<std::pair<int, int>> array;
    for (auto current_pair: hash_tables[vertex]) {
        array.push_back(current_pair);
    }
    return array;
}

int dijkstra(const SetGraph &graph, int start, int stop);
void run(std::istream &input, std::ostream &output);
void test_run();
void test_random();
void test();

int main() {
    // test();
    run(std::cin, std::cout);

    return 0;
}

void run(std::istream &input, std::ostream &output) {
    int N = 0;
    input >> N;
    int M = 0;
    input >> M;
    SetGraph graph(N);
    for (int i = 0; i < M; i++) {
        int from = 0;
        input >> from;
        int to = 0;
        input >> to;
        int weight = 0;
        input >> weight;
        graph.AddEdge(from, to, weight);
    }
    int start = 0;
    input >> start;
    int stop = 0;
    input >> stop;
    output << dijkstra(graph, start, stop) << std::endl;
}

int dijkstra(const SetGraph &graph, int start, int stop) {
    std::vector<int> d(graph.VerticesCount(), INT_MAX);
    d[start] = 0;
    std::set<std::pair<int, int>, PairComparator> priority_queue;
    priority_queue.insert(std::pair<int, int>(start, 0));
    while (!priority_queue.empty()) {
        std::pair<int, int> current_pair = *priority_queue.begin();
        priority_queue.erase(current_pair);
        int current_vertex = current_pair.first;
        for (std::pair<int, int> next_pair: graph.GetNextPairs(current_pair.first)) {
            int next_vertex = next_pair.first;
            int weight = next_pair.second;
            if (d[next_vertex] == INT_MAX) {
                d[next_vertex] = d[current_vertex] + weight;
                priority_queue.insert(std::pair<int, int>(next_vertex, d[current_vertex] + weight));
            }
            else if (d[current_vertex] + weight < d[next_vertex]) {
                priority_queue.erase({next_vertex, d[next_vertex]});
                d[next_vertex] = d[current_vertex] + weight;
                priority_queue.insert({next_vertex, d[next_vertex]});
            }
        }
    }
    return d[stop];
}

void test_run() {
    {
        std::stringstream input, output;
        input << "6" << std::endl;
        input << "9" << std::endl;
        input << "0 3 1" << std::endl;
        input << "0 4 2" << std::endl;
        input << "1 2 7" << std::endl;
        input << "1 3 2" << std::endl;
        input << "1 4 3" << std::endl;
        input << "1 5 3" << std::endl;
        input << "2 5 3" << std::endl;
        input << "3 4 4" << std::endl;
        input << "3 5 6" << std::endl;
        input << "0 2" << std::endl;
        run(input, output);
        std::string output_string = output.str();
        assert(output_string == "9\n");
    }
    {
        std::stringstream input, output;
        input << "2" << std::endl;
        input << "2" << std::endl;
        input << "0 1 2" << std::endl;
        input << "1 0 1" << std::endl;
        input << "0 1" << std::endl;
        run(input, output);
        std::string output_string = output.str();
        assert(output_string == "1\n");
    }
    {
        std::stringstream input, output;
        input << "9" << std::endl;
        input << "16" << std::endl;
        input << "0 1 9" << std::endl;
        input << "0 2 2" << std::endl;
        input << "0 3 6" << std::endl;
        input << "1 6 4" << std::endl;
        input << "2 3 3" << std::endl;
        input << "2 4 1" << std::endl;
        input << "3 1 2" << std::endl;
        input << "3 5 9" << std::endl;
        input << "3 6 7" << std::endl;
        input << "4 3 1" << std::endl;
        input << "4 7 6" << std::endl;
        input << "5 7 5" << std::endl;
        input << "5 8 1" << std::endl;
        input << "6 5 1" << std::endl;
        input << "6 8 5" << std::endl;
        input << "7 8 5" << std::endl;
        input << "0 8" << std::endl;
        run(input, output);
        std::string output_string = output.str();
        assert(output_string == "12\n");
    }
    {
        std::stringstream input, output;
        input << "9" << std::endl;
        input << "16" << std::endl;
        input << "0 1 9" << std::endl;
        input << "0 2 2" << std::endl;
        input << "0 3 6" << std::endl;
        input << "1 6 4" << std::endl;
        input << "2 3 3" << std::endl;
        input << "2 4 1" << std::endl;
        input << "3 1 2" << std::endl;
        input << "3 5 9" << std::endl;
        input << "3 6 7" << std::endl;
        input << "4 3 1" << std::endl;
        input << "4 7 6" << std::endl;
        input << "5 7 5" << std::endl;
        input << "5 8 1" << std::endl;
        input << "6 5 1" << std::endl;
        input << "6 8 5" << std::endl;
        input << "7 8 5" << std::endl;
        input << "8 0" << std::endl;
        run(input, output);
        std::string output_string = output.str();
        assert(output_string == "12\n");
    }
    {
        std::stringstream input, output;
        input << "3" << std::endl;
        input << "1" << std::endl;
        input << "0 1 2" << std::endl;
        input << "0 2" << std::endl;
        run(input, output);
        std::string output_string = output.str();
        assert(output_string == "2147483647\n");
    }
    {
        std::stringstream input, output;
        input << "3" << std::endl;
        input << "3" << std::endl;
        input << "0 1 1" << std::endl;
        input << "1 2 2" << std::endl;
        input << "0 2 4" << std::endl;
        input << "0 2" << std::endl;
        run(input, output);
        std::string output_string = output.str();
        assert(output_string == "3\n");
    }
    {
        std::stringstream input, output;
        input << "3" << std::endl;
        input << "3" << std::endl;
        input << "0 1 1" << std::endl;
        input << "1 2 2" << std::endl;
        input << "0 2 4" << std::endl;
        input << "0 2" << std::endl;
        run(input, output);
        std::string output_string = output.str();
        assert(output_string == "3\n");
    }
    {
        std::stringstream input, output;
        input << "6" << std::endl;
        input << "4" << std::endl;
        input << "1 2 7" << std::endl;
        input << "2 4 8" << std::endl;
        input << "4 5 1" << std::endl;
        input << "4 3 100" << std::endl;
        input << "3 1" << std::endl;
        run(input, output);
        std::string output_string = output.str();
        assert(output_string == "115\n");
    }
    {
        std::stringstream input, output;
        input << "3" << std::endl;
        input << "3" << std::endl;
        input << "0 1 1" << std::endl;
        input << "1 2 2" << std::endl;
        input << "0 2 4" << std::endl;
        input << "0 0" << std::endl;
        run(input, output);
        std::string output_string = output.str();
        assert(output_string == "0\n");
    }
    {
        std::stringstream input, output;
        input << "4" << std::endl;
        input << "7" << std::endl;
        input << "0 1 0" << std::endl;
        input << "0 2 5" << std::endl;
        input << "1 2 9" << std::endl;
        input << "1 2 9" << std::endl;
        input << "1 3 9" << std::endl;
        input << "1 3 7" << std::endl;
        input << "3 3 6" << std::endl;
        input << "1 0" << std::endl;
        run(input, output);
        std::string output_string = output.str();
        assert(output_string == "0\n");
    }
    std::cout << "run test OK" << std::endl;
}

void test_random() {
    int N = 100;
    for (int i = 0; i < N; i++) {
        int V = (rand() % 100) + 1;
        int E = (rand() % (V * V)) + 1;
        SetGraph graph(V);
        for (int j = 0; j < E; j++) {
            int from = rand() % V;
            int to = rand() % V;
            int weight = rand() % 10;
            graph.AddEdge(from, to, weight);
        }
        std::cout << i << "V = " << V << " E = " << E << std::endl;
        for (int j = 0; j < V; j++) {
            std::cout << i << " " << j << std::endl;
            for (int k = 0; k < V; k++) {
                dijkstra(graph, j, k);
            }
        }
    }
    std::cout << "random test OK" << std::endl;
}

void test() {
    test_run();
    test_random();
}