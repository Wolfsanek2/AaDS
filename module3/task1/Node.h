#ifndef NODE_H
#define NODE_H

struct Node {
    Node() = default;
    Node(int data)
    : data(data) {}
    int data = 0;
    Node *next = nullptr;
};

#endif