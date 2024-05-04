#include <iostream>
#include <cassert>

template <typename Comparator = std::less<int>>
class AvlTree {
private:
    struct Node {
        Node(const int &data)
        : data(data) {}

        void update_parameters() {
            count = 1;
            height = 0;
            if (left) {
                count += left->count;
                height = std::max(height, left->height);
            }
            if (right) {
                count += right->count;
                height = std::max(height, right->height);
            }
            height += 1;
        }
        int get_balance() {
            size_t l = 0, r = 0;
            if (left) l = left->height;
            if (right) r = right->height;
            return l - r;
        }

        int data;
        size_t height = 1;
        size_t count = 1;
        Node *left = nullptr;
        Node *right = nullptr;
    };
public:
    AvlTree() = default;
    ~AvlTree() {
        if (root) destroy_tree(root);
    }
    size_t add(const int &data) {
        root = add_internal(root, data);
        // assert(check(root));
        return get_kth_statistics(root, data);
    }
    void remove(size_t kth_statistics) {
        Node *node = get_node_by_k(root, kth_statistics);
        root = remove_internal(root, node->data);
        // assert(check(root));
    }
    bool has(const int &data) {
        return has_internal(root, data);
    }
    int next_x(const int &data) {
        return next_x_internal(root, data);
    }
    void print() {
        print_internal(root);
        std::cout << std::endl;
    }
private:
    Node *add_internal(Node *node, const int &data) {
        if (!node) {
            node = new Node(data);
            return node;
        }
        if (cmp(data, node->data)) {
            node->left = add_internal(node->left, data);
        }
        else {
            node->right = add_internal(node->right, data);
        }
        return do_balance(node);
    }
    size_t get_kth_statistics(Node *node, const int &data) {
        if (!node) return 0;
        if (data == node->data) {
            if (node->left) {
                return node->left->count;
            }
            return 0;
        }
        if (cmp(data, node->data)) {
            return get_kth_statistics(node->left, data);
        }
        else {
            if (!node->right) return node->count;
            return get_kth_statistics(node->right, data) + node->count - node->right->count;
        }
    }
    Node *do_balance(Node *node) {
        node->update_parameters();
        int balance = node->get_balance();
        switch (balance) {
            case 2:
                return rotate_right(node);
                break;
            case -2:
                return rotate_left(node);
                break;
        }
        return node;
    }
    Node *rotate_left(Node *node) {
        if (node->right->get_balance() > 0) {
            node->right = rotate_right(node->right);
        }
        Node *tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;
        node->update_parameters();
        tmp->update_parameters();
        return tmp;
    }
    Node *rotate_right(Node *node) {
        if (node->left->get_balance() < 0) {
            node->left = rotate_left(node->left);
        }
        Node *tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;
        node->update_parameters();
        tmp->update_parameters();
        return tmp;
    }
    Node *remove_internal(Node *node, const int &data) {
        if (data == node->data) {
            if (node->left && node->right) {
                int tmp = get_minmax(node->right)->data;
                node->data = tmp;
                node->right = remove_internal(node->right, tmp);
                return do_balance(node);
            }
            else if (node->left) {
                Node *tmp = node->left;
                delete node;
                return tmp;
            }
            else if (node->right) {
                Node *tmp = node->right;
                delete node;
                return tmp;
            }
            else {
                delete node;
                return nullptr;
            }
        }
        else if (cmp(data, node->data)) {
            node->left = remove_internal(node->left, data);
        }
        else {
            node->right = remove_internal(node->right, data);
        }
        return do_balance(node);
    }
    Node *get_node_by_k(Node *node, size_t k) {
        size_t tmp = k;
        if (node->left && k < node->left->count) {
            return get_node_by_k(node->left, k);
        }
        if (node->right && k > node->count - node->right->count - 1) {
            return get_node_by_k(node->right, k - (node->count - node->right->count));
        }
        return node;
    }
    Node *get_minmax(Node *node) {
        if (node->left) return get_minmax(node->left);
        return node;
    }
    void destroy_tree(Node *node) {
        if (node->left) {
            destroy_tree(node->left);
        }
        if (node->right) {
            destroy_tree(node->right);
        }
        delete node;
    }
    bool check(Node *node) {
        if (!root) return true;
        bool check_left = true;
        bool check_right = true;
        bool check_current = true;
        if (node->left) {
            if (!cmp(node->left->data, node->data)) return false;
            if (!this->check_left(node->left, node->data)) return false;
            check_left = check(node->left);
        }
        if (node->right) {
            if (!cmp(node->data, node->right->data)) return false;
            if (!this->check_right(node->right, node->data)) return false;
            check_right = check(node->right);
        }
        return check_left && check_right;
    }
    bool check_left(Node *node, const int &data) {
        if (!cmp(node->data, data)) return false;
        bool result = true;
        if (node->left) result = check_left(node->left, data);
        if (node->right) result = check_left(node->right, data);
        return result;
    }
    bool check_right(Node *node, const int &data) {
        if (!cmp(data, node->data)) return false;
        bool result = true;
        if (node->left && result) result = check_right(node->left, data);
        if (node->right && result) result = check_right(node->right, data);
        return result;
    }
    void print_internal(Node *node) {
        if (!node) return;
        if (node->left) print_internal(node->left);
        std::cout << node->data << " ";
        if (node->right) print_internal(node->right);
    }
    bool has_internal(Node *node, const int &data) {
        if (node->data == data) {
            return true;
        }
        bool result = false;
        if (node->left) result = has_internal(root->left, data);
        if (node->right && !result) result = has_internal(root->right, data);
        return result;
    }
    int next_x_internal(Node *node, const int &data) {
        if (cmp(data, node->data)) {
            if (node->left) {
                
            }
        }
        return -1;
    }

    Node *root = nullptr;
    Comparator cmp;
};

int main() {
    AvlTree<int> tree;
    std::string cmd;
    int value = 0;
    while (std::cin >> cmd >> value) {
        if (cmd == "insert") {
            tree.add(value);
        }
        else if (cmd == "delete") {
            tree.remove(value);
        }
        else if (cmd == "exists") {
            std::cout << tree.has(value) << std::endl;
        }
        else if (cmd == "next") {

        }
        else if (cmd == "prev") {

        }
    }
    return 0;
}