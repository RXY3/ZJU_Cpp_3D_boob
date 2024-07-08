#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <boomclear/view.h>
#include <boomclear/event.h>
#include <boomclear/judge.h>

/*
 *stay these parameters here
 */

// 窗口尺寸
int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

// 相机参数
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);                               // 相机位置
glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);                               // 物体位置，假设为中心点
glm::mat4 view = glm::lookAt(cameraPos, objectPos, glm::vec3(0.0f, 1.0f, 0.0f)); // 视图矩阵，初始时相机看向物体中心

int closestVertexIndex = -1;//最近的顶点索引
glm::vec3 closestVertex;//最近的顶点
glm::vec3 mousePoint;//鼠标点

int closestCenterIndex = -1;
// 假设 CenTerses 是存储所有立方体中心点的数组
glm::vec3 closestCenter;

// Vertex Shader
const char *vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

// Fragment Shader
const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

uniform int renderMode; // 0: Solid color, 1: Wireframe

void main()
{
    if (renderMode == 0)
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); // Orange color for faces
    else
        FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f); // Black color for wireframe
}
)";

float vertices[] = {
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,

    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,

    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,

    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, -0.5f};

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
};
 
AABB aabb[100];

// 用于检测射线与 AABB 相交
glm::vec3 intersectRayAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const AABB& aabb) {
    float tMin = (aabb.min.x - rayOrigin.x) / rayDir.x;
    float tMax = (aabb.max.x - rayOrigin.x) / rayDir.x;
    if (tMin > tMax) std::swap(tMin, tMax);

    float t1 = (aabb.min.y - rayOrigin.y) / rayDir.y;
    float t2 = (aabb.max.y - rayOrigin.y) / rayDir.y;
    if (t1 > t2) std::swap(t1, t2);

    if ((tMin > t2) || (tMax < t1)) return glm::vec3(-1.0f); // 射线与 AABB 无交点

    float t3 = (aabb.min.z - rayOrigin.z) / rayDir.z;
    float t4 = (aabb.max.z - rayOrigin.z) / rayDir.z;
    if (t3 > t4) std::swap(t3, t4);

    float tNear = std::max(t1, std::max(tMin, t3));
    float tFar = std::min(t2, std::min(tMax, t4));

    if (tNear < tFar && tNear >= 0) {
        // 计算交点
        glm::vec3 intersectionPoint = rayOrigin + rayDir * tNear;
        return intersectionPoint;
    } else {
        // 没有有效的交点
        return glm::vec3(-1.0f);
    }
}

glm::vec3 screenToWorldCoords(double xpos, double ypos, int width, int height, glm::mat4 view, glm::mat4 projection)
{
    float x = (2.0f * xpos) / width - 1.0f;
    float y = 1.0f - (2.0f * ypos) / height;
    float z = 1.0f;
    glm::vec3 ray_nds = glm::vec3(x, y, z);//归一化设备坐标

    glm::vec4 ray_clip = glm::vec4(ray_nds, 1.0);//齐次裁剪坐标

    glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;//视图坐标
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);//齐次裁剪坐标

    glm::vec3 ray_wor = glm::vec3(glm::inverse(view) * ray_eye);//世界坐标
    ray_wor = glm::normalize(ray_wor);//标准化
 
    return ray_wor;
}
//用于检测射线与立方体相交，返回交点
glm::vec3 findClosestCube(GLFWwindow* window, glm::mat4 view, glm::mat4 projection, std::vector<glm::vec3>& CenTerses, glm::vec3& closestCenter)
{
    double xpos, ypos;//获取鼠标水平和垂直位置
    glfwGetCursorPos(window, &xpos, &ypos);

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    // 将屏幕坐标转换为世界坐标
    glm::vec3 ray_wor = screenToWorldCoords(xpos, ypos, width, height, view, projection);

    float minDist = std::numeric_limits<float>::max();//最小距离初始化为无穷大
    closestCenter = glm::vec3(0.0f); // 初始化为零向量
    closestCenterIndex = -1;

    glm::vec3 rayOrigin = cameraPos; // 射线原点是相机位置
    glm::vec3 rayDirection = glm::normalize(ray_wor); // 标准化射线方向   

    for(int i=0;i<CenTerses.size();++i)
    {
        glm::vec3 mouse = intersectRayAABB(rayOrigin, rayDirection, aabb[i]);//射线与AABB相交，得到第一个交点
        if(mouse==glm::vec3(-1.0f))//没有交点，跳过
            continue;
        float dist = glm::length(mouse - rayOrigin);//计算射线原点到交点的距离，选取最近的
        if(dist < minDist)
        {
            minDist = dist;//更新最小距离
            closestCenter = CenTerses[i];//更新最近的立方体中心点
            closestCenterIndex = i;//更新最近的立方体中心点索引
            mousePoint = mouse;//更新鼠标点
        }
    }
    return closestCenter;
    
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void test()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    // Create a GLFW window
    GLFWwindow *window = glfwCreateWindow(2000, 1500, "Rotating Cube", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height)
                                   { glViewport(0, 0, width, height); });
    glfwSetMouseButtonCallback(window, resultLeft);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // Build and compile shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Set up vertex data and buffers
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Initialize camera and object position
    glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f); // Center of the object
    float radius = 3.0f;                               // Radius of the orbit
    float angle = 0.0f;                                // Initial angle for rotation

    for(int i=0;i<CenTerses.size();++i)
        {
            aabb[i].min = CenTerses[i] - glm::vec3(0.5f, 0.5f, 0.5f);
            aabb[i].max = CenTerses[i] + glm::vec3(0.5f, 0.5f, 0.5f);
        }

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Input
        processInput(window);

        // Update camera position and view matrix
        update_camera_direction();


        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate shader
        glUseProgram(shaderProgram);

        // Transformation matrices
        glm::mat4 model = glm::mat4(1.0f);
        // model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Render the cube
        glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);                         // Render faces as filled polygons
        glUniform1i(glGetUniformLocation(shaderProgram, "renderMode"), 0); // Set render mode to faces
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Render wireframe
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);                         // Render only the lines
        glUniform1i(glGetUniformLocation(shaderProgram, "renderMode"), 1); // Set render mode to wireframe
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //find the closest cube
        findClosestCube(window, view, glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f), CenTerses, closestCenter);

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Update angle for next frame
        angle += 0.5f;
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
}

int test2()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Mouse Click to World Coordinates", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // 设置鼠标回调函数
    glfwSetMouseButtonCallback(window, resultLeft);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}