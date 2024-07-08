#ifndef _VIEW_H_
#define _VIEW_H_

#include <glm/glm.hpp>

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

extern glm::vec3 cameraPos;
extern glm::vec3 objectPos;
extern glm::mat4 view;
extern std::vector<glm::vec3> CenTerses;
/*
TODO: Initialize the Center of each block
*/

/*
TODO: Draw the blocks according to the mode of the game
        and the click status and the boom status.
*/
glm::vec3 findClosestCube(GLFWwindow* window, glm::mat4 view, glm::mat4 projection, std::vector<glm::vec3>& CenTerses, glm::vec3& closestCenter);


#endif