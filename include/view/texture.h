#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

extern const char *vertexShaderSource;
extern const char *fragmentShaderSource;
extern float vertices[];

extern glm::vec3 cameraPos;
extern glm::vec3 objectPos;
extern glm::mat4 view;

void processInput(GLFWwindow *window);
unsigned int loadTexture(char const *path);
void renderCubes(int numCubes, std::vector<int> &drawFlags, std::vector<int> &textureCodes);

#endif