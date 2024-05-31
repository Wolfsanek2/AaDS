#ifndef NODE_H
#define NODE_H

#include "Pair.h"

struct Node {
    Node() = default;
    Node(const Pair &pair)
    : pair(pair) {}
    Pair pair;
    Node *next = nullptr;
};

#endif