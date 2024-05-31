#ifndef VERTEX_H
#define VERTEX_H

class Vertex {
public:
    Vertex(double x, double y);
    double get_distance(const Vertex &other) const;
private:
    double x, y;
};

#endif