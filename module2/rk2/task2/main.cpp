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
    int get_min_distance() {
        std::queue<Node<T>*>  queue;
        queue.push(root);
        while (!queue.empty()) {
            Node<T> *node = queue.front();
            queue.pop();
            if (!node->left && !node->right) return count_height(node->data);
            if (node->left) queue.push(node->left);
            if (node->right) queue.push(node->right);
        }
        return 0;
    }
private:
    int count_height(const T &value) {
        int height = 0;
        Node<T> *current_node = root;
        while (current_node) {
            if (cmp(value, current_node->data)) {
                current_node = current_node->left;
                height++;
            }
            else {
                current_node = current_node->right;
                height++;
            }
        }
        return height;
    }

    Node<T> *root = nullptr;
    Comparator cmp;
};

void run(std::istream &input, std::ostream &output);
void test();

int main() {
    // test();
    run(std::cin, std::cout);

    return 0;
}

void run(std::istream &input, std::ostream &output) {
    BinaryTree<int> tree;
    int value = 0;
    while (input >> value) {
        tree.add(value);
    }
    output << tree.get_min_distance() << std::endl;
}

void test() {
    {
        BinaryTree<int> tree;
        tree.add(3);
        tree.add(2);
        tree.add(1);
        tree.add(4);
        std::cout << tree.get_min_distance() << std::endl;
    }
}