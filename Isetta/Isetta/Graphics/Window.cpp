/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/Window.h"
#include "Core/Config/Config.h"

namespace Isetta {
void Isetta::WindowModule::StartUp() {
  glfwInit();
  InitRenderConfig();
  InitWindow();
}
void WindowModule::Update(float deltaTime) { glfwSwapBuffers(winHandle); }

void WindowModule::ShutDown() {
  glfwDestroyWindow(winHandle);
  winHandle = nullptr;
  glfwTerminate();
}

void WindowModule::InitWindow() {  // Create OpenGL window
  glfwWindowHint(GLFW_RED_BITS, 8);
  glfwWindowHint(GLFW_GREEN_BITS, 8);
  glfwWindowHint(GLFW_BLUE_BITS, 8);
  glfwWindowHint(GLFW_ALPHA_BITS, 8);
  glfwWindowHint(GLFW_DEPTH_BITS, 24);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  if (winFullScreen) {
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    winHandle = glfwCreateWindow(mode->width, mode->height, winTitle.c_str(),
                                 glfwGetPrimaryMonitor(), nullptr);
  } else {
    winHandle = glfwCreateWindow(initWinWidth, initWinHeight, winTitle.c_str(),
                                 nullptr, nullptr);
  }

  if (winHandle == nullptr) {
    // Fake message box
    glfwDestroyWindow(winHandle);

    winHandle = glfwCreateWindow(800, 50,
                                 "Unable to initialize engine - Make sure you "
                                 "have an OpenGL 2.0 compatible graphics card",
                                 nullptr, nullptr);
    double startTime = glfwGetTime();
    while (glfwGetTime() - startTime < 5.0) { /* Sleep */
    }

    throw new std::exception(
        "Render::InitWindow: Unable to initialize window. Make sure you have "
        "an OpenGL compatible graphics card.");
  }

  glfwSetWindowUserPointer(winHandle, this);
  glfwMakeContextCurrent(winHandle);
  // TODO(Chaojie): Probally remove this line in the future and limit the max
  // fps to 60fps
  glfwSwapInterval(0);

  glfwSetInputMode(winHandle, GLFW_CURSOR,
                   winShowCursor ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}
void WindowModule::InitRenderConfig() {
  winTitle = Config::Instance().windowConfig.windowTitle.GetVal();
  initWinWidth = Config::Instance().windowConfig.windowWidth.GetVal();
  initWinHeight = Config::Instance().windowConfig.windowHeight.GetVal();
  winFullScreen = Config::Instance().windowConfig.windowFullScreen.GetVal();
  winShowCursor = Config::Instance().windowConfig.windowShowCursor.GetVal();
}
}  // namespace Isetta
