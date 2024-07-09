#include "view.h"
#include <glm/glm.hpp>
#include <vector>
#include <thread>

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