#ifndef TRANSFORMATION_HPP
#define TRANSFORMATION_HPP

#include <eigen3/Eigen/Eigen>

using namespace Eigen;

class Transformation
{
public:
    Transformation(float x, float y)
        : pos(Vector2f(x,y)) { }

public:
    Vector2f pos;
    float angle;
};

#endif // TRANSFORMATION_HPP
