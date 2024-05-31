#include "Vertex.h"

#include <cmath>

Vertex::Vertex(double x, double y) 
: x(x), y(y) {}

double Vertex::get_distance(const Vertex &other) const {
    return sqrt(x*x + y*y);
}