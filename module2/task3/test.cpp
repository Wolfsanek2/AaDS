#include <cassert>
#include <sstream>

#define private public

#include "main.cpp"

void test_is_node_full();
void test_split_child();
void test_run();
void test();

int main() {
    test();
    return 0;
}

void test_is_node_full() {
    {

    }
    std::cout << "is_node_full test OK" << std::endl;
}

void test_split_child() {
    {
        BTree<int> tree(2);
        auto root = new BTreeNode<int> (false);
        auto child_1 = new BTreeNode<int> (true);
        auto child_2 = new BTreeNode<int> (true);
        root->children.resize(2);
        root->children[0] = child_1;
        root->children[1] = child_2;
        root->keys.resize(1);
        root->keys[0] = 4;
        child_1->keys.resize(3);
        child_1->keys[0] = 1;
        child_1->keys[1] = 2;
        child_1->keys[2] = 3;
        child_2->keys.resize(1);
        child_2->keys[0] = 5;
        tree.root = root;
        tree.split_child(root, 0);
        assert(root->keys.size() == 2);
        assert(root->keys[0] == 2);
        assert(root->keys[1] == 4);
        assert(root->children.size() == 3);
        assert(child_1->keys.size() == 1);
        assert(child_1->keys[0] == 1);
        assert(child_2->keys.size() == 1);
        assert(child_2->keys[0] == 5);
        assert(root->children[1]->keys.size() == 1);
        assert(root->children[1]->keys[0] == 3);
    }
    std::cout << "split_child test OK" << std::endl;
}

void test_run() {
    {
        std::stringstream input, output;
        input << "2" << std::endl;
        input << "0 1 2 3 4 5 6 7 8 9";
        run(input, output);
        std::string output_str = output.str();
        assert(output_str == "3 \n1 5 7 \n0 2 4 6 8 9 ");
    }
    std::cout << "run test OK" << std::endl;
}

void test() {
    test_is_node_full();
    test_split_child();
    test_run();
}