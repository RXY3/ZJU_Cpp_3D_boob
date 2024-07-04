#ifndef _CREATE_H_
#define _CREATE_H_

extern int SizeByMode[3];
extern int BoomByMode[3];

#include <vector>
#include <glm/glm.hpp>
#include <iostream>

std::vector<glm::vec4> initFrontier(int mode);
std::vector<bool> initialClickArr(int mode);
void createBoom(std::vector<glm::vec4> frontier, int mode);
void destroyFrontier(std::vector<glm::vec4> frontier);
void destroyClickArr(std::vector<bool> clickArr);

#endif
