// 4_1. Солдаты. В одной военной части решили построить в одну шеренгу по росту. Т.к.
// часть была далеко не образцовая, то солдаты часто приходили не вовремя, а то их и
// вовсе приходилось выгонять из шеренги за плохо начищенные сапоги. Однако солдаты в
// процессе прихода и ухода должны были всегда быть выстроены по росту – сначала самые
// высокие, а в конце – самые низкие. За расстановку солдат отвечал прапорщик, который
// заметил интересную особенность – все солдаты в части разного роста. Ваша задача
// состоит в том, чтобы помочь прапорщику правильно расставлять солдат, а именно для
// каждого приходящего солдата указывать, перед каким солдатом в строе он должен
// становится.
// Требования: скорость выполнения команды - O(log n).
// Формат входных данных.
// Первая строка содержит число N – количество команд (1 ≤ N ≤ 30 000). В каждой
// следующей строке содержится описание команды: число 1 и X если солдат приходит в
// строй (X – рост солдата, натуральное число до 100 000 включительно) и число 2 и Y если
// солдата, стоящим в строе на месте Y надо удалить из строя. Солдаты в строе нумеруются
// с нуля.
// Формат выходных данных.
// На каждую команду 1 (добавление в строй) вы должны выводить число K – номер
// позиции, на которую должен встать этот солдат (все стоящие за ним двигаются назад).

#include <iostream>
#include <cassert>

template <typename T, typename Comparator = std::less<T>>
class AvlTree {
private:
    struct Node {
        Node(const T &data)
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

        T data;
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
    size_t add(const T &data) {
        root = add_internal(root, data);
        // assert(check(root));
        return get_kth_statistics(root, data);
    }
    void remove(size_t kth_statistics) {
        Node *node = get_node_by_k(root, kth_statistics);
        root = remove_internal(root, node->data);
        // assert(check(root));
    }
    void print() {
        print_internal(root);
        std::cout << std::endl;
    }
private:
    Node *add_internal(Node *node, const T &data) {
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
    size_t get_kth_statistics(Node *node, const T &data) {
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
    Node *remove_internal(Node *node, const T &data) {
        if (data == node->data) {
            if (node->left && node->right) {
                T tmp = get_minmax(node->right)->data;
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
    bool check_left(Node *node, const T &data) {
        if (!cmp(node->data, data)) return false;
        bool result = true;
        if (node->left) result = check_left(node->left, data);
        if (node->right) result = check_left(node->right, data);
        return result;
    }
    bool check_right(Node *node, const T &data) {
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

    Node *root = nullptr;
    Comparator cmp;
};

void run(std::istream &input, std::ostream &output);

int main() {
    run(std::cin, std::cout);
    return 0;
}

void run(std::istream &input, std::ostream &output) {
    size_t number;
    input >> number;
    AvlTree<size_t, std::greater<size_t>> tree;
    size_t cmd, value;
    for (size_t i = 0; i < number; i++) {
        input >> cmd >> value;
        switch (cmd) {
            case 1:
                size_t k;
                k = tree.add(value);
                output << k << std::endl;
                break;
            case 2:
                tree.remove(value);
                break;
        }
    }
}