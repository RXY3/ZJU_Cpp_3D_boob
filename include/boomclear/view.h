#ifndef _VIEW_H_
#define _VIEW_H_

#include <glm/glm.hpp>

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

extern glm::vec3 cameraPos;
extern glm::vec3 objectPos;
extern glm::mat4 view;

/*
TODO: Draw the blocks according to the mode of the game
        and the click status and the boom status.
*/
void test();
int test2();

#endif