#include <unordered_set>
#include <string>
#include <iostream>

struct Node {
    Node(std::string product_name, int color, int size)
    : product_name(product_name), color(color), size(size) {}

    std::string product_name;
    int color;
    int size;
};

namespace std {
    template <>
    struct hash<Node> {
        size_t operator() (const Node &node) const {
            std::string str = node.product_name;
            size_t h = 0;
            int prime = 71;
            for (int i = 0; i < str.size(); i++) {
                h = h * prime + str[i];
            }
            return h;
        }
    };
    template <>
    struct equal_to<Node> {
        bool operator() (const Node &node_1, const Node &node_2) const {
            return (node_1.product_name == node_2.product_name) && (node_1.color == node_2.color) && (node_1.size == node_2.size);
        }
    };
};

int main() {
    std::unordered_set<Node, std::hash<Node>, std::equal_to<Node>> hash_table;
    char cmd;
    std::string name;
    int color;
    int size;
    while (std::cin >> cmd >> name >> color >> size) {
        switch (cmd) {
            case '+':
            {
                Node new_node(name, color, size);
                auto result = hash_table.insert(new_node);
                if (result.second) std::cout << "OK" << std::endl;
                else std::cout << "FAIL" << std::endl;
                break;
            }
            case '-':
            {
                Node node(name, color, size);
                auto result = hash_table.erase(node);
                if (result) std::cout << "OK" << std::endl;
                else std::cout << "FAIL" << std::endl;
                break;
            }
            case '?':
            {
                Node node(name, color, size);
                auto result = hash_table.count(node);
                if (result) std::cout << "OK" << std::endl;
                else std::cout << "FAIL" << std::endl;
                break;
            }
        }
    }

    return 0;
}