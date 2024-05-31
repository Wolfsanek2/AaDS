#include "Pair.h"

Pair::Pair(int vertex, double weight)
: vertex(vertex), weight(weight) {}

bool Pair::operator==(const Pair &other) const {
    return (vertex == other.vertex) && (weight == other.weight);
}

bool PairComparator::operator()(const Pair &pair_1, const Pair &pair_2) const {
        if (pair_1.weight < pair_2.weight) return true;
        if (pair_1.weight > pair_2.weight) return false;
        return pair_1.vertex < pair_2.vertex;
    }