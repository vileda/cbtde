#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <functional>

class GameObject;

class Component
{
public:
    Component(std::function<void (void)> drawFunc)
        : _draw_func(drawFunc) {
    }
    Component()
        : Component(nullptr) { }
    virtual void update() {}
    virtual void draw() {
        if(_draw_func) {
            _draw_func();
        }
    }
    virtual void onClick(float x, float y) {
        if(_onclick_func) {
            _onclick_func(x, y);
        }
    }

    void onClick(std::function<void (float x, float y)> func) {
        _onclick_func = func;
    }

public:
    GameObject *go;

private:
    std::function<void (void)> _draw_func;
    std::function<void (float x, float y)> _onclick_func;
};

#endif // COMPONENT_HPP
