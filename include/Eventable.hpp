#ifndef EVENTABLE_HPP
#define EVENTABLE_HPP

enum class EventType {
  UP,
  DOWN,
  PRESS
};

class MouseEvent {
  public:
    Vector2f pos;
    EventType type;
    enum class Button {
      LEFT = GLFW_MOUSE_BUTTON_1
    };
};

class KeyEvent {
  public:
    EventType type;
    enum class Key {
      ESC = GLFW_KEY_ESCAPE
    };
    Key key;
};

class Eventable {
  public:
    virtual void onMouseEvent(MouseEvent *evt) = 0;

    virtual void onKeyEvent(KeyEvent *evt) = 0;
    
};

#endif

