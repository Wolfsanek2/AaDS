#define private public
#include "main.cpp"
#undef private

#include <sstream>
#include <cassert>
#include <unordered_set>

void test_AvlTree();
void test_random();
void test_run();
void test();

int main() {
    test();
    return 0;
}

void test_AvlTree() {
    {
        AvlTree<int> tree;
        tree.add(0);
        tree.add(1);
        tree.add(2);
        tree.add(3);
        tree.add(4);
        tree.add(5);
        tree.add(6);
        tree.add(7);
        tree.add(8);
        tree.add(9);
        assert(tree.root->data == 3);
        assert(tree.root->left->data == 1);
        assert(tree.root->left->left->data == 0);
        assert(tree.root->left->right->data == 2);
        assert(tree.root->right->data == 7);
        assert(tree.root->right->left->data == 5);
        assert(tree.root->right->left->left->data == 4);
        assert(tree.root->right->left->right->data == 6);
        assert(tree.root->right->right->data == 8);
        assert(tree.root->right->right->right->data == 9);
    }
    {
        AvlTree<int, std::greater<int>> tree;
        tree.add(0);
        tree.add(1);
        tree.add(2);
        tree.add(3);
        tree.add(4);
        tree.add(5);
        tree.add(6);
        tree.add(7);
        tree.add(8);
        tree.add(9);
        assert(tree.root->data == 3);
        assert(tree.root->left->data == 7);
        assert(tree.root->left->left->data == 8);
        assert(tree.root->left->left->left->data == 9);
        assert(tree.root->left->right->data == 5);
        assert(tree.root->left->right->left->data == 6);
        assert(tree.root->left->right->right->data == 4);
        assert(tree.root->right->data == 1);
        assert(tree.root->right->left->data == 2);
        assert(tree.root->right->right->data == 0);
        tree.remove(5);
        assert(!tree.root->left->right->right);
        assert(tree.root->left->right->data == 5);
        assert(tree.root->left->data == 7);
        assert(tree.root->data == 3);
        tree.add(4);
        assert(tree.root->left->right->right->data == 4);
        tree.remove(6);
        assert(tree.root->data == 2);
        assert(!tree.root->right->left);
    }
    {
        AvlTree<int> tree;
        tree.add(3);
        tree.add(1);
        tree.add(0);
        tree.add(2);
        tree.add(5);
        tree.add(4);
        tree.add(7);
        assert(tree.get_kth_statistics(tree.root, 6) == 6);
        assert(tree.get_kth_statistics(tree.root, 7) == 6);
        assert(tree.get_kth_statistics(tree.root, 10) == 7);
    }
    {
        AvlTree<size_t> tree;
        tree.add(50);
        tree.add(40);
        tree.add(60);
        tree.add(70);
        tree.add(30);
        tree.add(45);
        tree.add(42);
        tree.add(47);
        tree.remove(7);
        assert(tree.root->data == 45);
        assert(tree.root->left->data == 40);
        assert(tree.root->left->left->data == 30);
        assert(tree.root->left->right->data == 42);
        assert(tree.root->right->data == 50);
        assert(tree.root->right->left->data == 47);
        assert(tree.root->right->right->data == 60);
    }
    {
        AvlTree<size_t> tree;
        tree.add(50);
        tree.add(40);
        tree.add(60);
        tree.add(30);
        tree.add(45);
        tree.add(42);
        assert(tree.root->data == 42);
        assert(tree.root->left->data == 40);
        assert(tree.root->left->left->data == 30);
        assert(tree.root->right->data == 50);
        assert(tree.root->right->left->data == 45);
        assert(tree.root->right->right->data == 60);
    }
    {
        AvlTree<size_t, std::greater<size_t>> tree;
        tree.add(50);
        tree.add(40);
        tree.add(60);
        tree.add(30);
        tree.add(45);
        tree.add(42);
        assert(tree.root->data == 42);
        assert(tree.root->right->data == 40);
        assert(tree.root->right->right->data == 30);
        assert(tree.root->left->data == 50);
        assert(tree.root->left->right->data == 45);
        assert(tree.root->left->left->data == 60);

        assert(tree.get_kth_statistics(tree.root, 60) == 0);
        assert(tree.get_kth_statistics(tree.root, 50) == 1);
        assert(tree.get_kth_statistics(tree.root, 45) == 2);
        assert(tree.get_kth_statistics(tree.root, 42) == 3);
        assert(tree.get_kth_statistics(tree.root, 40) == 4);
        assert(tree.get_kth_statistics(tree.root, 30) == 5);
    }
    {
        AvlTree<size_t, std::greater<size_t>> tree;
        tree.add(2);
        tree.add(4);
        tree.add(1);
        tree.add(3);
        tree.remove(2);
        assert(tree.root->data == 3);
        assert(tree.root->left->data == 4);
        assert(tree.root->right->data == 1);
    }
    {
        AvlTree<size_t, std::greater<size_t>> tree;
        tree.add(6);
        tree.add(9);
        tree.add(2);
        tree.add(7);
        tree.add(8);
        tree.add(5);
        tree.add(1);
        tree.add(4);
        tree.add(3);
        tree.remove(3);
    }
    std::cout << "AvlTree test OK" << std::endl;
}

void test_run() {
    {
        std::stringstream input, output;
        input << "5" << std::endl;
        input << "1 100" << std::endl;
        input << "1 200" << std::endl;
        input << "1 50" << std::endl;
        input << "2 1" << std::endl;
        input << "1 150" << std::endl;
        run(input, output);
        std::string output_string = output.str();
        assert(output_string == "0\n0\n2\n1\n");
    }
    {
        std::stringstream input, output;
        input << "17" << std::endl;
        input << "1 700" << std::endl;
        input << "1 400" << std::endl;
        input << "1 900" << std::endl;
        input << "1 200" << std::endl;
        input << "1 800" << std::endl;
        input << "1 100" << std::endl;
        input << "1 500" << std::endl;
        input << "1 300" << std::endl;
        input << "1 600" << std::endl;
        input << "2 0" << std::endl;
        input << "2 0" << std::endl;
        input << "2 2" << std::endl;
        input << "2 4" << std::endl;
        input << "1 900" << std::endl;
        input << "1 500" << std::endl;
        input << "1 800" << std::endl;
        input << "1 200" << std::endl;
        run(input, output);
        std::string output_string = output.str();
        assert(output_string == "0\n1\n0\n3\n1\n5\n3\n5\n3\n0\n3\n1\n7\n");
    }
    {
        std::stringstream input, output;
        input << "11" << std::endl;
        input << "1 711" << std::endl;
        input << "1 868" << std::endl;
        input << "1 644" << std::endl;
        input << "2 0" << std::endl;
        input << "1 859" << std::endl;
        input << "1 741" << std::endl;
        input << "1 778" << std::endl;
        input << "2 0" << std::endl;
        input << "2 2" << std::endl;
        input << "2 1" << std::endl;
        input << "1 711" << std::endl;
        run(input, output);
        std::string output_string = output.str();
        assert(output_string == "0\n0\n2\n0\n1\n1\n1\n");
    }
    std::cout << "run test OK" << std::endl;
}

void test_random() {
    {
        size_t n = 100;
        for (size_t i = 0; i < n; i++) {
            AvlTree<size_t, std::greater<size_t>> tree;
            size_t m = rand() % 40;
            std::unordered_set<size_t> set;
            for (size_t j = 0; j < m; j++) {
                if (rand() % 2) {
                    size_t value = rand() % 1000;
                    if (set.count(value)) {
                        continue;
                    }
                    set.insert(value);
                    std::cout << "+" << value << " ";
                    tree.add(value);
                }
                else if (tree.root) {
                    size_t k = rand() % tree.root->count;
                    AvlTree<size_t, std::greater<size_t>>::Node *node = tree.get_node_by_k(tree.root, k);
                    set.erase(node->data);
                    std::cout << "-" << k << " ";
                    tree.remove(k);
                }
            }
            std::cout << std::endl;
            tree.print();
        }
    }
}

void test() {
    test_AvlTree();
    test_run();
    // test_random();
}