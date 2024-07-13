
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <CommandBus.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void update_camera_direction();
void mouse_button_callback_left(GLFWwindow *window, int button, int action, int mods);
void mouse_button_callback_right(GLFWwindow *window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

class View
{
private:
    CommandBus &commandBus;
    GLFWwindow *window;
    std::vector<unsigned int> textures;
    unsigned int VBO, VAO, shaderProgram;

public:
    glm::vec3 closestCenter;
    View();
    ~View(){};
    double xPos, yPos;

    std::vector<int> *drawFlags;
    std::vector<int> *textureCodes;
    glm::vec3 cameraPos;
    glm::vec3 objectPos;
    glm::mat4 view;

    /*TODO*/

    unsigned int loadTexture(char const *path);
    void renderCubes(int numCubes);
    void setView(glm::vec3 cameraPos, glm::vec3 objectPos, glm::mat4 view)
    {
        this->cameraPos = cameraPos;
        this->objectPos = objectPos;
        this->view = view;
    }

    View(std::vector<int> *df, std::vector<int> *tc, CommandBus &commandBus)
        : drawFlags(df), textureCodes(tc), commandBus(commandBus)
    {
        commandBus.Publish("CheckForLeft", [this]()
                           {
        // 检查是否有炸弹
        std::cout << "view" << std::endl; });
        commandBus.Publish("CheckForRight", [this]()
                           {
        // 检查是否有炸弹
        std::cout << "view" << std::endl; });
    }
    void check(int x)
    {
        if (x == 1)
        {
            commandBus.Execute("CheckForLeft");
        }
        else if (x == 2)
        {
            commandBus.Execute("CheckForRight");
        }
    };

    bool shouldClose();
    int getFlag();
    void cleanUp();
    void init(int numCubes);
};

#endif