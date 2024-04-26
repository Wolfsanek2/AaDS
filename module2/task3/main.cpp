// Постройте B-дерево минимального порядка t и выведите его по слоям.
// В качестве ключа используются числа, лежащие в диапазоне 0..2^32 -1
// Требования:
// B-дерево должно быть реализовано в виде шаблонного класса.
// Решение должно поддерживать передачу функции сравнения снаружи.
// Формат входных данных
// Сначала вводится минимальный порядок дерева t.
// Затем вводятся элементы дерева.
// Формат выходных данных
// Программа должна вывести B-дерево по слоям. Каждый слой на новой строке, элементы
// должны выводится в том порядке, в котором они лежат в узлах.

#include <iostream>
#include <vector>
#include <queue>

template <typename T>
struct BTreeNode {
    BTreeNode(bool leaf)
    : leaf(leaf) {}
    ~BTreeNode() {
        if (!leaf) {
            for (BTreeNode<T> *child: children) {
                delete child;
            }
        }
    }

    bool leaf;
    std::vector<T> keys;
    std::vector<BTreeNode<T>*> children;
};

template <typename T, typename Comparator = std::less<T>>
class BTree {
public:
    BTree(size_t min_degree)
    : t(min_degree) {}
    ~BTree() {
        if (root) {
            delete root;
        }
    }

    void insert(const T &key) {
        if (!root) {
            root = new BTreeNode<T>(true);
        }
        if (is_node_full(root)) {
            BTreeNode<T> *new_root = new BTreeNode<T>(false);
            new_root->children.push_back(root);
            root = new_root;
            split_child(root, 0);
        }
        insert_non_full(root, key);
    }
    std::ostream & operator >> (std::ostream &output) {
        std::queue<BTreeNode<T>*> queue;
        BTreeNode<T> *node = root;
        queue.push(node);
        int count = 1;
        std::vector<BTreeNode<T> *> one_level_nodes_1;
        one_level_nodes_1.push_back(node);
        while (!queue.empty()) {
            // Если выведен весь ряд
            if (!count) {
                output << std::endl;
                // Обновление счетчика
                size_t size = one_level_nodes_1.size();
                for (size_t i = 0; i < size; i++) {
                    count += one_level_nodes_1[i]->children.size();
                }
                std::vector<BTreeNode<T> *> one_level_nodes_2;
                for (size_t i = 0; i < size; i++) {
                    // Добавление следующего ряда в массив
                    for (size_t j = 0; j < one_level_nodes_1[i]->children.size(); j++) {
                        one_level_nodes_2.push_back(one_level_nodes_1[i]->children[j]);
                    }
                }
                one_level_nodes_1 = one_level_nodes_2;
            }
            // Вывод очередного узла
            node = queue.front();
            for (size_t i = 0; i < node->keys.size(); i++) {
                output << node->keys[i] << " ";
            }
            // Удаление узла из очереди и добавление его потомков
            queue.pop();
            if (!node->leaf) {
                for (size_t i = 0; i < node->children.size(); i++) {
                    queue.push(node->children[i]);
                }    
            }
            count--;
        }
        return output;
    }
private:
    bool is_node_full(BTreeNode<T> *node) {
        return node->keys.size() == 2 * t - 1;
    }
    void split_child(BTreeNode<T> *node, size_t index) {
        BTreeNode<T> *new_child = new BTreeNode<T>(true);
        BTreeNode<T> *old_child = node->children[index];
        new_child->leaf = old_child->leaf;

        // Добавление медианного ключа потомка в node
        node->keys.resize(node->keys.size() + 1);
        for (size_t i = node->keys.size() - 1; i > index; i--) {
            node->keys[i] = node->keys[i - 1];
        }
        node->keys[index] = old_child->keys[t - 1];

        // Перенос ключей к новому узлу
        new_child->keys.resize(t - 1);
        for (size_t i = 0; i < t - 1; i++) {
            new_child->keys[i] = old_child->keys[i + t];
        }
        old_child->keys.resize(t - 1);

        // Добавление нового узла в потомки node
        node->children.resize(node->children.size() + 1);
        for (size_t i = node->children.size() - 2; i > index; i--) {
            node->children[i + 1] = node->children[i];
        }
        node->children[index + 1] = new_child;

        // Добавление потомков old_child в потомки new_child
        if (!node->children[index]->leaf) {
            new_child->children.resize(t);
            for (size_t i = 0; i < t; i++) {
                new_child->children[i] = old_child->children[i + t];
            }
            old_child->children.resize(t);
        }
    }
    void insert_non_full(BTreeNode<T> *node, const T &key) {
        int pos = node->keys.size() - 1;
        if (node->leaf) {
            node->keys.resize(node->keys.size() + 1);
            while (pos >= 0 && cmp(key, node->keys[pos])) {
                node->keys[pos + 1] = node->keys[pos];
                pos--;
            }
            node->keys[pos + 1] = key;
        }
        else {
            while (pos >= 0 && cmp(key, node->keys[pos])) {
                pos--;
            }
            if (is_node_full(node->children[pos + 1])) {
                split_child(node, pos + 1);
                if (cmp(node->keys[pos + 1], key)) {
                    pos++;
                }
            }
            insert_non_full(node->children[pos + 1], key);
        }
    }

    BTreeNode<T> *root = nullptr;
    size_t t;
    Comparator cmp;
};

void run(std::istream &input, std::ostream &output);

int main() {
    run(std::cin, std::cout);
    return 0;
}

void run(std::istream &input, std::ostream &output) {
    size_t t;
    input >> t;
    BTree<int> tree(t);
    int element;
    while (input >> element) {
        tree.insert(element);
    }
    tree >> output;
}