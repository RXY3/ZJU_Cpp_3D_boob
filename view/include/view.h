/* CubeRenderer.h

一个简单的OpenGL渲染器，用于渲染立方体

用法示例：
int main() {
    glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 13.0f);
    std::vector<int> drawFlags = {
        1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1
    }; // All cubes drawn

    std::vector<int> textureCodes = {
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0
    }; // Alternating textures

    CubeRenderer renderer(camPos, drawFlags, textureCodes);

    // 另一个线程或外部代码可以通过以下方式更新摄像机位置
    std::thread updateCameraThread([&renderer]() {
        // 模拟摄像机位置更新
        glm::vec3 newCamPos = glm::vec3(0.0f, 0.0f, 15.0f);
        renderer.updateCameraPos(newCamPos);
        std::this_thread::sleep_for(std::chrono::seconds(1)); //停止一秒
        newCamPos = glm::vec3(0.0f, 15.0f, 15.0f);
        renderer.updateCameraPos(newCamPos);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    });

    renderer.render();

    updateCameraThread.join();
    return 0;
}
*/

#ifndef CUBE_RENDERER_H
#define CUBE_RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

class CubeRenderer {
public:
    CubeRenderer(glm::vec3 camPos, const std::vector<int>& drawFlags, const std::vector<int>& textureCodes);
    ~CubeRenderer();

    void render();
    void updateCameraPos(const glm::vec3& newCameraPos);

private:
    GLFWwindow* window;
    glm::vec3 cameraPos;
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    std::vector<int> drawFlags;
    std::vector<int> textureCodes;

    unsigned int VBO, VAO, shaderProgram;
    unsigned int texture1, texture2;
    std::vector<unsigned int> textures;

    void init();
    void cleanup();
    void processInput(GLFWwindow* window);
    unsigned int loadTexture(char const* path);
};

#endif // CUBE_RENDERER_H
