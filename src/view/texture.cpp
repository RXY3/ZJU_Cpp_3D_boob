#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <vector>
#include <view_module/event.h>
#include <view/texture.h>

// 顶点着色器
const char *vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
)";

// 片段着色器
const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, TexCoord);
}
)";

float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

// 相机参数
glm::vec3 cameraPos0 = glm::vec3(0.0f, 0.0f, 10.0f);                                // 相机位置
glm::vec3 objectPos0;                                                               // 物体位置
glm::mat4 view0 = glm::lookAt(cameraPos0, objectPos0, glm::vec3(0.0f, 1.0f, 0.0f)); // 视图矩阵，初始时相机看向物体中心

float radius = 10.0f; // 相机到物体的距离，假设为10个单位
float angle = 0.0f;   // 初始角度
float pitch = 0.0f;   // 初始俯仰角度

// 鼠标按键状态
bool mouse_button_pressed = false;

// 鼠标拖动标志位
bool mouse_dragging = false;

// 上一次鼠标位置
double last_xpos, last_ypos;

// 针对于点击的flag
int flag = 0;

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

unsigned int View::loadTexture(char const *path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
// 更新相机方向
void update_camera_direction()
{
    // 根据角度更新相机位置
    cameraPos0.x = objectPos0.x + radius * sin(glm::radians(angle)) * cos(glm::radians(pitch));
    cameraPos0.y = objectPos0.y + radius * sin(glm::radians(pitch));
    cameraPos0.z = objectPos0.z + radius * cos(glm::radians(angle)) * cos(glm::radians(pitch));

    // 更新视图矩阵，相机始终看向物体中心
    view0 = glm::lookAt(cameraPos0, objectPos0, glm::vec3(0.0f, 1.0f, 0.0f));
}

/*
 * return the mouse position in world coordinates when the left mouse button is clicked
 */
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
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
                // 触发事件,返回点击的位置
                flag = 1;
            }
        }
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
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
                flag = 2;
            }
        }
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
        angle -= xoffset * sensitivity;
        // 限制角度范围在360度内
        if (angle > 360.0f)
            angle -= 360.0f;
        if (angle < 0.0f)
            angle += 360.0f;
        // 更新垂直角度，限制范围在90度以内
        pitch -= yoffset * sensitivity;
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

    update_camera_direction(); // 更新相机位置和视角
}

void View::init(int numCubes)
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    // Create a GLFW window
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Rotating Cube", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height)
                                   { glViewport(0, 0, width, height); });

    // Register mouse callbacks
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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

    shaderProgram = glCreateProgram();
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
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Load and create textures
    unsigned int texture0 = loadTexture("../texture_src/0.jpg");
    unsigned int texture1 = loadTexture("../texture_src/1.jpg");
    unsigned int texture2 = loadTexture("../texture_src/2.jpg");
    unsigned int texture3 = loadTexture("../texture_src/3.jpg");
    unsigned int texture4 = loadTexture("../texture_src/4.jpg");
    unsigned int texture5 = loadTexture("../texture_src/5.jpg");
    unsigned int texture6 = loadTexture("../texture_src/6.jpg");
    unsigned int texture7 = loadTexture("../texture_src/7.jpg");
    unsigned int texture8 = loadTexture("../texture_src/8.jpg");
    unsigned int texture9 = loadTexture("../texture_src/9.jpg");
    unsigned int texture10 = loadTexture("../texture_src/10.jpg");
    unsigned int texture11 = loadTexture("../texture_src/11.jpg");
    unsigned int texture12 = loadTexture("../texture_src/12.jpg");
    unsigned int texture13 = loadTexture("../texture_src/13.jpg");
    unsigned int texture14 = loadTexture("../texture_src/14.jpg");
    unsigned int texture15 = loadTexture("../texture_src/15.jpg");
    unsigned int texture16 = loadTexture("../texture_src/16.jpg");
    unsigned int texture17 = loadTexture("../texture_src/17.jpg");
    unsigned int texture18 = loadTexture("../texture_src/18.jpg");
    unsigned int texture19 = loadTexture("../texture_src/19.jpg");
    unsigned int texture20 = loadTexture("../texture_src/20.jpg");
    unsigned int texture21 = loadTexture("../texture_src/21.jpg");
    unsigned int texture22 = loadTexture("../texture_src/22.jpg");
    unsigned int texture23 = loadTexture("../texture_src/23.jpg");
    unsigned int texture24 = loadTexture("../texture_src/24.jpg");
    unsigned int texture25 = loadTexture("../texture_src/25.jpg");
    unsigned int texture26 = loadTexture("../texture_src/26.jpg");

    textures = {texture0, texture1, texture2, texture3, texture4, texture5, texture6, texture7, texture8, texture9, texture10, texture11, texture12, texture13, texture14, texture15, texture16, texture17, texture18, texture19, texture20, texture21, texture22, texture23, texture24, texture25, texture26};

    // cube大小为numCubes的立方根
    int cubeSize = static_cast<int>(cbrt(numCubes));
    std::vector<glm::vec3> centers;
    for (int x = 0; x < cubeSize; ++x)
    {
        for (int y = 0; y < cubeSize; ++y)
        {
            for (int z = 0; z < cubeSize; ++z)
            {
                centers.push_back(glm::vec3(x, y, z));
            }
        }
    }

    objectPos0 = glm::vec3(cubeSize / 2, cubeSize / 2, cubeSize / 2); // 物体位置在cube grid的中心

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
}

void View::renderCubes(int numCubes)
{
    // Input
    glfwGetCursorPos(window, &xPos, &yPos);
    setView(cameraPos0, objectPos0, view0);
    processInput(window);
    update_camera_direction();

    if (flag)
    {
        check(flag);
        flag = 0;
    }
    // Render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Activate shader
    glUseProgram(shaderProgram);

    // Transformation matrices
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 2000.0f / 1500.0f, 0.1f, 100.0f);

    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);

    int n = static_cast<int>(cbrt(numCubes)); // Calculate the dimension size of the cube grid
    for (int x = 0; x < n; ++x)
    {
        for (int y = 0; y < n; ++y)
        {
            for (int z = 0; z < n; ++z)
            {
                int index = x * n * n + y * n + z;
                if ((*drawFlags)[index] == 1)
                {
                    // Select the correct texture
                    glBindTexture(GL_TEXTURE_2D, textures[(*textureCodes)[index]]);

                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(x, y, z)); // Position the cube
                    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
                    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
            }
        }
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool View::shouldClose()
{
    return glfwWindowShouldClose(window);
}

int View::getFlag()
{
    return flag;
}

void View::cleanUp()
{
    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
}