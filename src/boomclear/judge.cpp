#include <boomclear/judge.h>
#include <GLFW/glfw3.h>
#include <boomclear/event.h>
#include <glm/glm.hpp>
#include <iostream>

void resultLeft(GLFWwindow *window, int button, int action, int mods)
{
    double click_pos_3d[3] = {0, 0, 0};
    mouse_button_callback(window, button, action, mods, click_pos_3d);
}