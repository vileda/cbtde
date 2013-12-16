#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <vector>

#include "Transformation.hpp"
#include "Component.hpp"
#include "Eventable.hpp"

class GameApp;

class GameObject :
  public Transformation,
  public Eventable
{
public:
    GameObject()
        : Transformation(0,0) {}

    GameObject(float x, float y)
        : Transformation(x,y) {}

    virtual void update() {
      updateComponents(components);
      for(auto go : children) {
        updateComponents(components);
      }
    }

    virtual void draw() {
      drawComponents(components);
      for(auto go : children) {
        drawComponents(go->components);
      }
    }

    virtual void onClick(float x, float y) {
      onClickComponents(components, x, y);
      for(auto go : children) {
        onClickComponents(go->components, x, y);
      }
    }

    void onKeyEvent(KeyEvent *evt) {
      std::cout << "onKeyEvent" << std::endl;
    }

    void onMouseEvent(MouseEvent *evt) {
      std::cout << "onMouseEvent" << std::endl;
    }

    void addChild(GameObject *go) {
        go->parent = this;
        children.push_back(go);
    }

    void addComponent(Component *comp) {
        comp->go = this;
        components.push_back(comp);
    }

    void addComponent(std::function<void (void)> drawFunc) {
      auto c = new Component(drawFunc);
      c->go = this;
      components.push_back(c);
    }

private:
    void drawComponents(std::vector<Component*> comps) {
      for(auto c : comps) {
        c->draw();
      }
    }

    void updateComponents(std::vector<Component*> comps) {
      for(auto c : comps) {
        c->update();
      }
    }

    void onClickComponents(std::vector<Component*> comps, float x, float y) {
      for(auto c : comps) {
        c->onClick(x, y);
      }
    }

public:
    GameApp *app;
    GameObject *parent;
    std::vector<Component*> components;
    std::vector<GameObject*> children;
};

#endif // GAMEOBJECT_HPP
