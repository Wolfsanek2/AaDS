#include <iostream>
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
    bool check_same() {
        std::queue<Node<T>*>  queue;
        queue.push(root);
        T value = root->data;
        while (!queue.empty()) {
            Node<T> *node = queue.front();
            queue.pop();
            if (node->data != value) return false;
            if (node->left) queue.push(node->left);
            if (node->right) queue.push(node->right);
        }
        return true;
    }
private:
    Node<T> *root = nullptr;
    Comparator cmp;
};

void run(std::istream &input, std::ostream &output);

int main() {
    run(std::cin, std::cout);

    return 0;
}

void run(std::istream &input, std::ostream &output) {
    BinaryTree<int> tree;
    int value = 0;
    while (input >> value) {
        tree.add(value);
    }
    output << tree.check_same() << std::endl;
}