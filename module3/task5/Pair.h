#ifndef PAIR_H
#define PAIR_H

struct Pair {
    Pair(int vertex, double weight);
    bool operator==(const Pair &other) const;
    int vertex;
    double weight;
};

class PairComparator {
public:
    bool operator()(const Pair &pair_1, const Pair &pair_2) const;
};

#endif