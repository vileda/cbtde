#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <vector>

#include "Transformation.hpp"
#include "Component.hpp"

class GameApp;

class GameObject : public Transformation
{
public:
    GameObject()
        : Transformation(0,0) {}

    GameObject(float x, float y)
        : Transformation(x,y) {}

    virtual void update() {
        for(auto c : components) {
            c->update();
        }
    }
    virtual void draw() {
        for(auto *c : components) {
            c->draw();
        }
    }

    virtual void onClick(float x, float y) {
        for(auto c : components) {
            c->onClick(x, y);
        }
    }

    void addChild(GameObject *go) {
        go->parent = this;
        children.push_back(go);
    }

    void addCompontent(Component *comp) {
        comp->go = this;
        components.push_back(comp);
    }

    void addCompontent(std::function<void (void)> drawFunc) {
      auto c = new Component(drawFunc);
      c->go = this;
      components.push_back(c);
    }

public:
    GameApp *app;
    GameObject *parent;
    std::vector<GameObject*> children;
    std::vector<Component*> components;
};

#endif // GAMEOBJECT_HPP
