// Найдите приближенное решение метрической неориентированной задачи коммивояжера в
// полном графе (на плоскости) с помощью минимального остовного дерева.
// Вариант 2. Для построения минимального остовного дерева используйте алгоритм Прима.

#include "ListGraph.h"
#include "Pair.h"
#include "Vertex.h"

#include <iostream>
#include <set>
#include <random>
#include <cfloat>
#include <algorithm>
#include <cassert>

ListGraph get_MST(const ListGraph &graph);
std::vector<Pair> prima(const ListGraph &graph);
std::vector<int> DFS(const ListGraph &graph);
double count_H(const ListGraph &original_graph, const std::vector<int> &vertices);
double get_exact_solution(const ListGraph &graph);
double rho(double c_1, double c_2);
double arythmetic_mean(const std::vector<double> &results);
double standart_deviation(const std::vector<double> &results);
void test_DFS();
void test_solutions();
void test();
void run(std::istream &input, std::ostream &output);

int main() {
    test();
    run(std::cin, std::cout);

    return 0;
}

void run(std::istream &input, std::ostream &output) {
    std::default_random_engine generator;
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    for (int N = 2; N < 10; N++) {
        std::vector<double> results;
        for (int i = 0; i < 10; i++) {
            std::vector<Vertex> vertices;
            for (int j = 0; j < N; j++) {
                double u = dist(generator);
                double v = dist(generator);
                double s = u*u + v*v;
                if (s == 0 || s > 1) {
                    j--;
                    continue;
                }
                else {
                    double x = u * sqrt(-2 * log(s) / s);
                    double y = v * sqrt(-2 * log(s) / s);
                    Vertex vertex(x, y);
                    vertices.push_back(vertex);
                }
            }
            ListGraph graph(vertices.size());
            for (int j = 0; j < graph.VerticesCount(); j++) {
                for (int k = j + 1; k < graph.VerticesCount(); k++) {
                    double value = vertices[j].get_distance(vertices[k]);
                    graph.AddEdge(j, k, vertices[j].get_distance(vertices[k]));
                }
            }
            ListGraph mst = get_MST(graph);
            std::vector<int> vertices_order = DFS(mst);
            double approximate_distance = count_H(graph, vertices_order);
            double exact_distance = get_exact_solution(graph);
            double coef = rho(approximate_distance, exact_distance);
            output << "N = " << N << ", approx_d = " << approximate_distance << ", d = " << exact_distance << ", rho = " << coef << std::endl;
            assert(exact_distance <= approximate_distance);
            results.push_back(coef);
        }
        output << "N = " << N << ", average = " << arythmetic_mean(results) << ", standart deviation = " << standart_deviation(results) << std::endl;
    }
}

ListGraph get_MST(const ListGraph &graph) {
    std::vector<Pair> pairs = prima(graph);
    ListGraph mst(graph.VerticesCount());
    for (int i = 1; i < graph.VerticesCount(); i++) {
        mst.AddEdge(i, pairs[i].vertex, pairs[i].weight);
    }
    return mst;
}

std::vector<Pair> prima(const ListGraph &graph) {
    std::vector<int> p(graph.VerticesCount(), -1);
    std::vector<double> min_e(graph.VerticesCount(), DBL_MAX);
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
            if (min_e[next_pair.vertex] == DBL_MAX) {
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
    std::vector<Pair> result;
    for (int i = 0; i < graph.VerticesCount(); i++) {
        result.push_back({p[i], min_e[i]});
    }
    return result;
}

std::vector<int> DFS(const ListGraph &graph) {
    std::stack<Pair> stack;
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::vector<int> result;
    Pair current_pair = {0, 0};
    stack.push(current_pair);
    visited[0] = true;
    while (!stack.empty()) {
        current_pair = stack.top();
        stack.pop();
        result.push_back(current_pair.vertex);
        for (Pair next_pair: graph.GetNextPairs(current_pair.vertex)) {
            if (!visited[next_pair.vertex]) {
                visited[next_pair.vertex] = true;
                stack.push(next_pair);
            }
        }
    }
    return result;
}

double count_H(const ListGraph &original_graph, const std::vector<int> &vertices) {
    ListGraph graph(vertices.size());
    double result = 0;
    for (int i = 1; i < vertices.size(); i++) {
        int from = vertices[i - 1];
        int to = vertices[i];
        result += original_graph.GetWeight(from, to);
    }
    result += original_graph.GetWeight(vertices.size() - 1, 0);
    return result;
}

double get_exact_solution(const ListGraph &graph) {
    std::vector<int> vertices;
    for (int i = 0; i < graph.VerticesCount(); i++) {
        vertices.push_back(i);
    }
    double min_path = DBL_MAX;
    int permutations = 0;
    do {
        double current_min_path = 0;
        for (int i = 1; i < vertices.size(); i++) {
            current_min_path += graph.GetWeight(vertices[i - 1], vertices[i]);
        }
        current_min_path += graph.GetWeight(vertices[vertices.size() - 1], vertices[0]);
        if (current_min_path < min_path) {
            min_path = current_min_path;
        }
        permutations++;
    } while (std::next_permutation(vertices.begin(), vertices.end()));
    return min_path;
}

void test_DFS() {
    {
        ListGraph graph(3);
        graph.AddEdge(0, 1, 0.514);
        graph.AddEdge(0, 2, 0.514);
        std::vector<int> vertices_order = DFS(graph);
        assert(vertices_order[0] == 0);
        assert(vertices_order[1] == 2);
        assert(vertices_order[2] == 1);
    }
    std::cout << "DFS test OK" << std::endl;
}

void test_solutions() {
    {
        ListGraph graph(3);
        graph.AddEdge(0, 1, 0.514);
        graph.AddEdge(0, 2, 0.514);
        graph.AddEdge(1, 2, 1.708);
        ListGraph mst = get_MST(graph);
        std::vector<int> vertices_order = DFS(mst);
        double approximate_distance = count_H(graph, vertices_order);
        double exact_distance = get_exact_solution(graph);
        assert(exact_distance <= approximate_distance);
    }
    {
        ListGraph graph(3);
        graph.AddEdge(0, 1, 1.32);
        graph.AddEdge(0, 2, 1.32);
        graph.AddEdge(1, 2, 0.48);
        ListGraph mst = get_MST(graph);
        std::vector<int> vertices_order = DFS(mst);
        double approximate_distance = count_H(graph, vertices_order);
        double exact_distance = get_exact_solution(graph);
        assert(exact_distance == approximate_distance);
    }
    std::cout << "solutions test OK" << std::endl;
}

void test() {
    test_DFS();
    test_solutions();
}

double rho(double c_1, double c_2) {
    return std::max(c_1 / c_2, c_2 / c_1);
}

double arythmetic_mean(const std::vector<double> &results) {
    double result = 0;
    for (double value : results) {
        result += value;
    }
    return result / results.size();
}

double standart_deviation(const std::vector<double> &results) {
    double average = arythmetic_mean(results);
    double sum = 0;
    for (double value : results) {
        double dif = value - average;
        sum += dif * dif;
    }
    return sqrt(sum);
}