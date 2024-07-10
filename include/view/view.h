#ifndef _VIEW_H_
#define _VIEW_H_

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <module/create.h>

extern glm::vec3 cameraPos;
extern glm::vec3 objectPos;
extern glm::mat4 view;

/*
TODO: Initialize the Center of each block
*/

/*
TODO: Draw the blocks according to the mode of the game
        and the click status and the boom status.
*/
void findClosestCube(GLFWwindow *window, glm::mat4 view, glm::mat4 projection, std::vector<glm::vec3> &CenTerses, std::vector<int> &drawFlags);
void getClosestCenter(Block &click_pos);

#endif