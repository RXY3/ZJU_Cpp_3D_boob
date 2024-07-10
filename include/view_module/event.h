#ifndef EVENT_H
#define EVENT_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <module/create.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void update_camera_direction();
void mouse_button_callback_left(GLFWwindow *window, int button, int action, int mods);
void mouse_button_callback_right(GLFWwindow *window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

#endif
