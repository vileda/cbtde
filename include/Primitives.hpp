#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP

#include <cmath>

#include "Component.hpp"

class Circle : public Component
{
public:
    Circle(float r, GLfloat *color)
        : r(r), color(color) { }

    void draw() {
        float theta = 2 * 3.1415926 / float(segments);
        float c = cosf(theta); //precalculate the sine and cosine
        float s = sinf(theta);
        float t;

        float x = r; //we start at angle = 0
        float y = 0;

        glBegin(GL_TRIANGLE_FAN);
        for(int ii = 0; ii < segments; ii++)
        {
            glColor3fv(color);
            glVertex2f(x + go->pos.x(), y + go->pos.y());

            t = x;
            x = c * x - s * y;
            y = s * t + c * y;
        }
        glEnd();
    }

    void update() {}

    void onClick(float x, float y) {
      float dx = pow(x-go->pos.x(),2);
      float dy = pow(y-go->pos.y(),2);
      float dr = dx+dy;
      float rq = pow(r,2);

      if(dr < rq) {
          std::cout << "hit" << std::endl;
      }

      std::cout << "r:" << rq << " dr:" << dr << std::endl;

      std::cout << "click " << x << "," << y << std::endl;
    }

private:
    float r;
    unsigned int segments = 360;

public:
    GLfloat *color;
};

class Triangle : public Component
{
public:
    Triangle() {}
};


#endif // PRIMITIVES_HPP
