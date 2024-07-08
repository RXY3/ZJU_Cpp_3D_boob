#ifndef _JUDGE_H_
#define _JUDGE_H_

#include <GLFW/glfw3.h>
#include <boomclear/create.h>

void bfsBoomZero(Block &click_pos);
void boomClear(Block &boom_pos);
void boomDel();
void resultLeft(GLFWwindow *window, int button, int action, int mods);
void resultRight(GLFWwindow *window, int button, int action, int mods);

#endif