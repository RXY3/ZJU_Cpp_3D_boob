#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <boomclear/view.h>
#include <iostream>

/*
 * TODO: roller change the sight and the size of the camera(maybe we can use the left bottom too)
        right click to stand a flag
 */

float radius = 3.0f; // 相机到物体的距离，假设为3个单位
float angle = 0.0f;  // 初始角度
float pitch = 0.0f;  // 初始俯仰角度

// 投影矩阵
glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

// 鼠标按键状态
bool mouse_button_pressed = false;

// 鼠标拖动标志位
bool mouse_dragging = false;

// 上一次鼠标位置
double last_xpos, last_ypos;

// 更新相机方向
void update_camera_direction()
{
        // 根据角度更新相机位置
        cameraPos.x = objectPos.x + radius * sin(glm::radians(angle)) * cos(glm::radians(pitch));
        cameraPos.y = objectPos.y + radius * sin(glm::radians(pitch));
        cameraPos.z = objectPos.z + radius * cos(glm::radians(angle)) * cos(glm::radians(pitch));

        // 更新视图矩阵，相机始终看向物体中心
        view = glm::lookAt(cameraPos, objectPos, glm::vec3(0.0f, 1.0f, 0.0f));
}

/*
 * return the mouse position in world coordinates when the left mouse button is clicked
 */
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods, double *click_pos_3d)
{
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
                if (action == GLFW_PRESS)
                {
                        mouse_button_pressed = true;
                        mouse_dragging = false;
                        glfwGetCursorPos(window, &last_xpos, &last_ypos);
                }
                else if (action == GLFW_RELEASE)
                {
                        mouse_button_pressed = false;
                        if (!mouse_dragging)
                        {
                                double xpos, ypos;
                                glfwGetCursorPos(window, &xpos, &ypos);

                                // 将屏幕坐标转换为归一化设备坐标
                                float x = (2.0f * xpos) / SCREEN_WIDTH - 1.0f;
                                float y = 1.0f - (2.0f * ypos) / SCREEN_HEIGHT;
                                float z = 1.0f;

                                glm::vec3 ray_nds = glm::vec3(x, y, z);

                                // 从归一化设备坐标转换到齐次裁剪坐标
                                glm::vec4 ray_clip = glm::vec4(ray_nds, 1.0f);

                                // 从齐次裁剪坐标转换到相机坐标
                                glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
                                ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);

                                // 从相机坐标转换到世界坐标
                                glm::vec3 ray_wor = glm::vec3(glm::inverse(view) * ray_eye);
                                ray_wor = glm::normalize(ray_wor);

                                click_pos_3d[0] = ray_wor.x;
                                click_pos_3d[1] = ray_wor.y;
                                click_pos_3d[2] = ray_wor.z;
                                std::cout << ray_wor.x << " " << ray_wor.y << " " << ray_wor.z << std::endl;
                        }
                }
        }
        /*TODO:rightclick
         */
        else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
        }
}
/*
 *drag
 */
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
        if (mouse_button_pressed)
        {
                mouse_dragging = true;

                // 计算鼠标偏移量
                double xoffset = xpos - last_xpos;
                double yoffset = last_ypos - ypos; // 注意这里与之前的正负相反，因为y坐标从下到上增加

                // 设置灵敏度
                float sensitivity = 0.1f;

                // 更新水平角度
                angle += xoffset * sensitivity;
                // 限制角度范围在360度内
                if (angle > 360.0f)
                        angle -= 360.0f;
                if (angle < 0.0f)
                        angle += 360.0f;
                // 更新垂直角度，限制范围在90度以内
                pitch += yoffset * sensitivity;
                if (pitch > 89.0f)
                        pitch = 89.0f;
                if (pitch < -89.0f)
                        pitch = -89.0f;

                // 更新相机方向
                update_camera_direction();

                // 更新上一次的鼠标位置
                last_xpos = xpos;
                last_ypos = ypos;
        }
}
/*
 *use scroll to change the radius
 */
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
        radius -= yoffset * 0.1f; // 根据滚轮的y偏移量调整radius，可以根据需要调整缩放速度
        if (radius < 1.0f)
                radius = 1.0f; // 确保radius不小于最小值，可以根据需要调整最小值

        std::cout << radius << std::endl;

        update_camera_direction(); // 更新相机位置和视角
}
