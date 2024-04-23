// Дано число N < 10^6 и последовательность целых чисел из [-2^31..2^31] длиной N.
// Требуется построить бинарное дерево, заданное наивным порядком вставки.
// Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то
// узел K добавляется в правое поддерево root; иначе в левое поддерево root.
// 2_4. Выведите элементы в порядке level-order (по слоям, “в ширину”).

#include <iostream>
#include <sstream>
#include <cassert>
#include <vector>
#include <queue>

template <typename T>
struct Node {
    Node(const T &data) 
    : data(data) {}

    T data;
    Node *left = nullptr;
    Node *right = nullptr;
};

template <typename T, typename Comparator = std::less<T>>
class BinaryTree {
public:
    BinaryTree() = default;
    ~BinaryTree() {
        std::queue<Node<T>*> queue;
        queue.push(root);
        while (!queue.empty()) {
            Node<T> *node = queue.front();
            queue.pop();
            if (node->left) queue.push(node->left);
            if (node->right) queue.push(node->right);
            delete node;
        }        
    }

    void add(const T &key) {
        Node<T> *new_node = new Node<T>(key);
        if (!root) {
            root = new_node;
            return;
        }
        Node<T> *current_node = root;
        while (current_node) {
            if (cmp(key, current_node->data)) {
                if (current_node->left) {
                    current_node = current_node->left;
                }
                else {
                    current_node->left = new_node;
                    return;
                }
            }
            else {
                if (current_node->right) {
                    current_node = current_node->right;
                }
                else {
                    current_node->right = new_node;
                    return;
                }
            }
        }
    }
    void to_vector(std::vector<T> &output) {
        std::queue<Node<T>*> queue;
        queue.push(root);
        while (!queue.empty()) {
            Node<T> *node = queue.front();
            queue.pop();
            output.push_back(node->data);
            if (node->left) queue.push(node->left);
            if (node->right) queue.push(node->right);
        }
    }
private:
    Node<T> *root = nullptr;
    Comparator cmp;
};

void run(std::istream &input, std::ostream &output);
void test_BinaryTree();
void test_run();
void test();

int main() {

    // test();
    run(std::cin, std::cout);

    return 0;
}

void run(std::istream &input, std::ostream &output) {
    BinaryTree<int> tree;
    int number;
    input >> number;
    for (int i = 0; i < number; i++) {
        int element;
        input >> element;
        tree.add(element);
    }
    std::vector<int> array;
    tree.to_vector(array);
    for (int i = 0; i < array.size(); i++) {
        output << array[i] << " ";
    }
}

void test_BinaryTree() {
    {
        BinaryTree<int> tree;
        tree.add(1);
        tree.add(2);
        tree.add(3);
        std::vector<int> array;
        tree.to_vector(array);
        assert(array[0] == 1);
        assert(array[1] == 2);
        assert(array[2] == 3);
    }
    std::cout << "BinaryTest OK" << std::endl;
}

void test_run() {
    {
        std::stringstream input, output;
        input << "3" << std::endl;
        input << "2 1 3" << std::endl;
        run(input, output);
        assert(output.str() == "2 1 3 ");
    }
    {
        std::stringstream input, output;
        input << "3" << std::endl;
        input << "1 2 3" << std::endl;
        run(input, output);
        assert(output.str() == "1 2 3 ");
    }
    {
        std::stringstream input, output;
        input << "3" << std::endl;
        input << "3 1 2" << std::endl;
        run(input, output);
        assert(output.str() == "3 1 2 ");
    }
    {
        std::stringstream input, output;
        input << "4" << std::endl;
        input << "3 1 4 2" << std::endl;
        run(input, output);
        assert(output.str() == "3 1 4 2 ");
    }
    std::cout << "run test OK" << std::endl;
}

void test() {
    test_BinaryTree();
    test_run();
}