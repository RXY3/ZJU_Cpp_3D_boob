#ifndef EVENT_H
#define EVENT_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

void update_camera_direction();
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods, double *click_pos_3d);
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

#endif
