#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <module/create.h>
#include <view_module/event.h>
#include <view/view.h>
#include <module/judge.h>
#include <view/texture.h>

/*
 *stay these parameters here
 */

// 窗口尺寸
// int SCREEN_WIDTH = 800;
// int SCREEN_HEIGHT = 600;

// // 相机参数
// glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);                               // 相机位置
// glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);                               // 物体位置，假设为中心点
// glm::mat4 view = glm::lookAt(cameraPos, objectPos, glm::vec3(0.0f, 1.0f, 0.0f)); // 视图矩阵，初始时相机看向物体中心

int closestVertexIndex = -1; // 最近的顶点索引
glm::vec3 closestVertex;     // 最近的顶点
glm::vec3 mousePoint;        // 鼠标点

int closestCenterIndex = -1;
// 假设 CenTerses 是存储所有立方体中心点的数组
glm::vec3 closestCenter;

struct AABB
{
    glm::vec3 min;
    glm::vec3 max;
};

void getClosestCenter(Block &click_pos)
{
    click_pos.setX(closestCenter.x);
    click_pos.setY(closestCenter.y);
    click_pos.setZ(closestCenter.z);
}

// 用于检测射线与 AABB 相交
glm::vec3 intersectRayAABB(const glm::vec3 &rayOrigin, const glm::vec3 &rayDir, const AABB &aabb)
{
    float tMin = (aabb.min.x - rayOrigin.x) / rayDir.x;
    float tMax = (aabb.max.x - rayOrigin.x) / rayDir.x;
    if (tMin > tMax)
        std::swap(tMin, tMax);

    float t1 = (aabb.min.y - rayOrigin.y) / rayDir.y;
    float t2 = (aabb.max.y - rayOrigin.y) / rayDir.y;
    if (t1 > t2)
        std::swap(t1, t2);

    if ((tMin > t2) || (tMax < t1))
        return glm::vec3(-1.0f); // 射线与 AABB 无交点

    float t3 = (aabb.min.z - rayOrigin.z) / rayDir.z;
    float t4 = (aabb.max.z - rayOrigin.z) / rayDir.z;
    if (t3 > t4)
        std::swap(t3, t4);

    float tNear = std::max(t1, std::max(tMin, t3));
    float tFar = std::min(t2, std::min(tMax, t4));

    if (tNear < tFar && tNear >= 0)
    {
        // 计算交点
        glm::vec3 intersectionPoint = rayOrigin + rayDir * tNear;
        return intersectionPoint;
    }
    else
    {
        // 没有有效的交点
        return glm::vec3(-1.0f);
    }
}

glm::vec3 screenToWorldCoords(double xpos, double ypos, int width, int height, glm::mat4 view, glm::mat4 projection)
{
    float x = (2.0f * xpos) / width - 1.0f;
    float y = 1.0f - (2.0f * ypos) / height;
    float z = 1.0f;
    glm::vec3 ray_nds = glm::vec3(x, y, z); // 归一化设备坐标

    glm::vec4 ray_clip = glm::vec4(ray_nds, 1.0); // 齐次裁剪坐标

    glm::vec4 ray_eye = glm::inverse(projection) * ray_clip; // 视图坐标
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);    // 齐次裁剪坐标

    glm::vec3 ray_wor = glm::vec3(glm::inverse(view) * ray_eye); // 世界坐标
    ray_wor = glm::normalize(ray_wor);                           // 标准化

    return ray_wor;
}
// 用于检测射线与立方体相交，返回交点
void findClosestCube(GLFWwindow *window, glm::mat4 view, glm::mat4 projection, std::vector<glm::vec3> &CenTerses, std::vector<int> &drawFlags)
{
    double xpos, ypos; // 获取鼠标水平和垂直位置
    glfwGetCursorPos(window, &xpos, &ypos);

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    // 将屏幕坐标转换为世界坐标
    glm::vec3 ray_wor = screenToWorldCoords(xpos, ypos, width, height, view, projection);

    float minDist = std::numeric_limits<float>::max(); // 最小距离初始化为无穷大
    closestCenter = glm::vec3(-1.0f);                   // 初始化为-1向量
    closestCenterIndex = -1;

    glm::vec3 rayOrigin = cameraPos;                  // 射线原点是相机位置
    glm::vec3 rayDirection = glm::normalize(ray_wor); // 标准化射线方向

    for (int i = 0; i < CenTerses.size(); ++i)
    {
        if (drawFlags[i] == 0)
        {
            continue; // 跳过那些没有绘制的立方体
        }

        AABB aabb = AABB{CenTerses[i] - glm::vec3(0.5f, 0.5f, 0.5f), CenTerses[i] + glm::vec3(0.5f, 0.5f, 0.5f)}; // 初始化AABB
        glm::vec3 mouse = intersectRayAABB(rayOrigin, rayDirection, aabb);                                        // 射线与AABB相交，得到第一个交点
        if (mouse == glm::vec3(-1.0f))                                                                            // 没有交点，跳过
            continue;
        float dist = glm::length(mouse - rayOrigin); // 计算射线原点到交点的距离，选取最近的
        if (dist < minDist)
        {
            minDist = dist;               // 更新最小距离
            closestCenter = CenTerses[i]; // 更新最近的立方体中心点
            closestCenterIndex = i;       // 更新最近的立方体中心点索引
            mousePoint = mouse;           // 更新鼠标点
        }
    }

    if (closestCenterIndex == -1)
    {
        mousePoint = glm::vec3(-1.0f, -1.0f, -1.0f); // 鼠标点击空白部分时返回-1, -1, -1
    }
}
