#ifndef GAMEAPP_HPP
#define GAMEAPP_HPP

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cstdio>

#include "GameObject.hpp"
#include "Eventable.hpp"

class GameApp :
  public Eventable
{
  public:
    static GameApp& getInstance()
    {
      static GameApp instance; // Guaranteed to be destroyed.
      // Instantiated on first use.
      return instance;
    }


  public:
    static void error_callback(int error, const char* description)
    {
      fputs(description, stderr);
    }

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
      KeyEvent *evt = new KeyEvent();
      switch (key) {
        case GLFW_KEY_ESCAPE:
          evt->key = KeyEvent::Key::ESC;
          break;
      }
      
      switch (action) {
        case GLFW_PRESS:
          evt->type = EventType::PRESS;
          break;
      }

      GameApp::getInstance().onKeyEvent(evt);
    }

    static void mouse_callback(GLFWwindow* window, int button, int action, int mods)
    {
      if (button == GLFW_MOUSE_BUTTON_1 && GameApp::getInstance().last_mouse_action == GLFW_PRESS && action == GLFW_RELEASE) {
        double x, y;
        glfwGetCursorPos(GameApp::getInstance().window, &x, &y);

        float ww = GameApp::getInstance().width;
        float wh = GameApp::getInstance().height;
        float rax = GameApp::getInstance().ratio;
        float ray = 1.f;
        float rx = ww/(ww/rax);
        float ry = wh/(wh/1.f);

        y = (y-wh)*(-1);

        float wpx = (x/(ww/(rax*2.f))) - (rx);
        float wpy = (y/(wh/(ray*2.f))) - (ry);

        //GameApp::getInstance().onClick(wpx,wpy);
        std::cout << "screen click: " << x << "," << y << std::endl;
        std::cout << "world click: " << wpx << "," << wpy << std::endl;
      }
      GameApp::getInstance().last_mouse_action = action;
      MouseEvent *evt = new MouseEvent();
      GameApp::getInstance().onMouseEvent(evt);
    }

    void onMouseEvent(MouseEvent *evt) {
      assert(_rootGO != nullptr);
      _rootGO->onMouseEvent(evt);
    }

    void onKeyEvent(KeyEvent *evt) {
      assert(_rootGO != nullptr);
      _rootGO->onKeyEvent(evt);
      if (evt->key == KeyEvent::Key::ESC)
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    void render() {
      _rootGO->draw();
    }

    void init() {
      glfwSetErrorCallback(error_callback);

      if (!glfwInit())
        exit(EXIT_FAILURE);

      window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
      if (!window)
      {
        glfwTerminate();
        exit(EXIT_FAILURE);
      }

      glfwMakeContextCurrent(window);
    }

    int shouldQuit() {
      return glfwWindowShouldClose(window);
    }

    void setRootGO(GameObject *rootGO) {
      _rootGO = rootGO;
      glfwSetKeyCallback(window, key_callback);
      glfwSetMouseButtonCallback(window, mouse_callback);
    }

    GameObject *rootGO() {
      return _rootGO;
    }

    void update() {
      glfwGetFramebufferSize(window, &width, &height);
      ratio = width / (float) height;

      glViewport(0, 0, width, height);

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(-ratio, ratio, -1.f, 1.1f, 1.f, -1.f);
      glMatrixMode(GL_MODELVIEW);

      /*glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POINT_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);*/

      glLoadIdentity();
      glClear(GL_COLOR_BUFFER_BIT);


      _rootGO->update();
    }

    void run() {
      assert(_rootGO != nullptr);
      while (!shouldQuit())
      {
        update();

        glBegin(GL_QUADS);
        glColor3f(1,1,1);
        glVertex2d(-1.f,-1.f);
        glVertex2d(1.f,-1.f);
        glVertex2d(1.f,1.f);
        glVertex2d(-1.f,1.f);
        glEnd();

        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
      }

      glfwDestroyWindow(window);

      glfwTerminate();
      exit(EXIT_SUCCESS);
    }

  private:
    GameApp() {
      init();
    }
    GameApp(GameApp const&);
    void operator=(GameApp const&);

  public:
    int width;
    int height;
    GLFWwindow* window;
    int last_mouse_action;
    float ratio;

  private:
    GameObject *_rootGO;
};

#endif // GAMEAPP_HPP
