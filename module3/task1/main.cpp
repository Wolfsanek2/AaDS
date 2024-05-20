#include <iostream>
#include <functional>
#include <vector>
#include <queue>
#include <cassert>

#include "ListGraph.h"
#include "SetGraph.h"
#include "MatrixGraph.h"
#include "ArcGraph.h"

void mainBFS(const IGraph &graph, const std::function<void(int)> &func);
void BFS(const IGraph &graph, const std::function<void(int)> &func, std::vector<bool> &visited, int vertex);
void mainDFS(const IGraph &graph, const std::function<void(int)> &func);
void DFS(const IGraph &graph, const std::function<void(int)> &func, std::vector<bool> &visited, int vertex);
void test_graph(IGraph &graph);
void test();

int main() {
    test();

    ListGraph list_graph(10);
    list_graph.AddEdge(0, 1);
    list_graph.AddEdge(0, 2);
    list_graph.AddEdge(0, 5);
    list_graph.AddEdge(0, 7);
    list_graph.AddEdge(1, 2);
    list_graph.AddEdge(2, 0);
    list_graph.AddEdge(2, 3);
    list_graph.AddEdge(3, 1);
    list_graph.AddEdge(3, 4);
    list_graph.AddEdge(4, 2);
    list_graph.AddEdge(4, 5);
    list_graph.AddEdge(5, 3);
    list_graph.AddEdge(5, 6);
    list_graph.AddEdge(6, 4);
    list_graph.AddEdge(6, 7);
    list_graph.AddEdge(7, 5);
    list_graph.AddEdge(8, 9);
    list_graph.AddEdge(9, 8);

    std::cout << "list graph:" << std::endl;
    mainBFS(list_graph, [](int vertex){std::cout << vertex << " ";});
    std::cout << std::endl;
    mainDFS(list_graph, [](int vertex){std::cout << vertex << " ";});
    std::cout << std::endl;

    std::cout << "set graph:" << std::endl;
    SetGraph set_graph(list_graph);
    mainBFS(set_graph, [](int vertex){std::cout << vertex << " ";});
    std::cout << std::endl;
    mainDFS(set_graph, [](int vertex){std::cout << vertex << " ";});
    std::cout << std::endl;

    std::cout << "matrix graph:" << std::endl;
    MatrixGraph matrix_graph(set_graph);
    mainBFS(matrix_graph, [](int vertex){std::cout << vertex << " ";});
    std::cout << std::endl;
    mainDFS(matrix_graph, [](int vertex){std::cout << vertex << " ";});
    std::cout << std::endl;

    std::cout << "arc graph:" << std::endl;
    ArcGraph arc_graph(matrix_graph);
    mainBFS(arc_graph, [](int vertex){std::cout << vertex << " ";});
    std::cout << std::endl;
    mainDFS(arc_graph, [](int vertex){std::cout << vertex << " ";});
    std::cout << std::endl;

    std::cout << "list graph:" << std::endl;
    ListGraph list_graph_2(arc_graph);
    mainBFS(list_graph, [](int vertex){std::cout << vertex << " ";});
    std::cout << std::endl;
    mainDFS(list_graph, [](int vertex){std::cout << vertex << " ";});
    std::cout << std::endl;

    return 0;
}

void mainBFS(const IGraph &graph, const std::function<void(int)> &func) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    for (int i = 0; i < graph.VerticesCount(); i++) {
        if (!visited[i]) {
            BFS(graph, func, visited, i);
        }
    }
}

void BFS(const IGraph &graph, const std::function<void(int)> &func, std::vector<bool> &visited, int vertex) {
    visited[vertex] = true;
    std::queue<int> queue;
    queue.push(vertex);
    while (!queue.empty()) {
        int current_vertex = queue.front();
        queue.pop();
        func(current_vertex);
        for (int next_vertex: graph.GetNextVertices(current_vertex)) {
            if (!visited[next_vertex]) {
                queue.push(next_vertex);
                visited[next_vertex] = true;
            }
        }
    }
}

void mainDFS(const IGraph &graph, const std::function<void(int)> &func) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    for (int i = 0; i < graph.VerticesCount(); i++) {
        if (!visited[i]) {
            DFS(graph, func, visited, i);
        }
    }
}

void DFS(const IGraph &graph, const std::function<void(int)> &func, std::vector<bool> &visited, int vertex) {
    visited[vertex] = true;
    func(vertex);
    for (int next_vertex: graph.GetNextVertices(vertex)) {
        if (!visited[next_vertex]) {
            DFS(graph, func, visited, next_vertex);
        }
    }
}

void test_graph(IGraph &graph) {
    {
        assert(graph.VerticesCount() == 5);
        graph.AddEdge(0, 1);
        graph.AddEdge(0, 3);
        graph.AddEdge(0, 4);
        graph.AddEdge(2, 1);
        graph.AddEdge(2, 4);
        graph.AddEdge(2, 3);
        std::vector<int> next_vertices = graph.GetNextVertices(0);
        for (int value: next_vertices) {
            switch (value) {
                case 1:
                    break;
                case 3:
                    break;
                case 4:
                    break;
                default:
                    assert(false);
            }
        }
        next_vertices = graph.GetNextVertices(2);
        for (int value: next_vertices) {
            switch (value) {
                case 1:
                    break;
                case 3:
                    break;
                case 4:
                    break;
                default:
                    assert(false);
            }
        }
        std::vector<int> prev_vertices = graph.GetPrevVertices(1);
        for (int value: prev_vertices) {
            switch (value) {
                case 0:
                    break;
                case 2:
                    break;
                default:
                    assert(false);
            }
        }
        prev_vertices = graph.GetPrevVertices(3);
        for (int value: prev_vertices) {
            switch (value) {
                case 0:
                    break;
                case 2:
                    break;
                default:
                    assert(false);
            }
        }
        prev_vertices = graph.GetPrevVertices(4);
        for (int value: prev_vertices) {
            switch (value) {
                case 0:
                    break;
                case 2:
                    break;
                default:
                    assert(false);
            }
        }
    }
}

void test() {
    ListGraph list_graph(5);
    test_graph(list_graph);
    SetGraph set_graph(5);
    test_graph(set_graph);
    ArcGraph arc_graph(5);
    test_graph(arc_graph);
    MatrixGraph matrix_graph(5);
    test_graph(matrix_graph);
    
    std::cout << "test OK" << std::endl;
}