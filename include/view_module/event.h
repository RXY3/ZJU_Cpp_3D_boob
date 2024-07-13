#ifndef _EVENT_H_
#define _EVENT_H_

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <module/create.h>
#include <module/judge.h>
#include <view_module/event.h>
#include <CommandBus.h>

class ViewModel
{
private:
    CommandBus &commandBus;
    int SizeByMode[3] = {3, 5, 10};

public:
    bool isGameOver = false;
    ViewModel(int mode, CommandBus &bus, std::vector<int> *df, std::vector<int> *tc) : commandBus(bus), drawFlags(df), textureCodes(tc)
    {
        commandBus.Subscribe("CheckForLeft", [this]() // 点击到左键
                             {
        
            //检查是否有炸弹
            Block click_pos(0, 0, 0, false, 0, false);
            setCenTerses();
            findClosestCube(view, *drawFlags);
            // std::cout << closestCenter.x << " " << closestCenter.y << " " << closestCenter.z << std::endl;
            getClosestCenter(click_pos);
            bool judge_close = resultLeft(click_pos, frontier);
            FlagUpdate();
            if (judge_close) {
                isGameOver = true;
                std::cout << "game over" << std::endl; 
            }
            else {
                if (frontier.empty()) {
                    std::cout << "You win" << std::endl;
                    isGameOver = true;
                }
            }
        });

        commandBus.Subscribe("CheckForRight", [this]() // 点击到右键
                             {
            Block click_pos(0, 0, 0, false, 0, false);
            setCenTerses();
            findClosestCube(view,  *drawFlags);
            getClosestCenter(click_pos);
            bool judge_close = resultRight(click_pos, frontier);
            FlagUpdate(); 
            if (judge_close) {
                isGameOver = true;
                std::cout << "game over" << std::endl; 
            }
            else {
                if (frontier.empty()) {
                    std::cout << "You win" << std::endl;
                    isGameOver = true;
                }
            }
            });
        // initial:FRONTIER

        // 初始化frontier大小为100

        initFrontier(mode, frontier);
        createBoom(mode, frontier);
        FlagInit(mode);
        size = SizeByMode[mode];
        // 输出frontier的大小
        FlagUpdate();
    }
    ~ViewModel(){};
    void getClosestCenter(Block &click_pos)
    {
        click_pos = Block(closestCenter.x, closestCenter.y, closestCenter.z, false, 0, false);
    }

    double xPos, yPos;

    void findClosestCube(glm::mat4 view, std::vector<int> &drawFlags);

    glm::vec3 closestCenter;
    glm::vec3 cameraPos;
    glm::vec3 objectPos;
    glm::mat4 view;
    int size;

    std::vector<int> *drawFlags;
    std::vector<int> *textureCodes;

    void cameraSync(glm::vec3 cameraPos, glm::vec3 objectPos, glm::mat4 view, double xPos, double yPos)
    {
        this->cameraPos = cameraPos;
        this->objectPos = objectPos;
        this->view = view;
        this->xPos = xPos;
        this->yPos = yPos;
    }

    std::vector<Block> frontier;

    std::vector<glm::vec3> CenTerses;

    void setCenTerses()
    {
        // 根据frontier设置CenTerses
        for (auto &it : frontier)
        {
            CenTerses.push_back(glm::vec3(it.getX(), it.getY(), it.getZ()));
        }
    }

    void FlagUpdate()
    {
        // 更新drawFlags
        for (size_t i = 0; i < drawFlags->size(); ++i)
        {
            drawFlags->at(i) = 0;
        }
        for (auto &it : frontier)
        {
            drawFlags->at(it.getX() * size * size + it.getY() * size + it.getZ()) = 1;
            if (!it.getIsBoom() && it.getIsClicked())
                textureCodes->at(it.getX() * size * size + it.getY() * size + it.getZ()) = it.getBoomCount();
            else
                textureCodes->at(it.getX() * size * size + it.getY() * size + it.getZ()) = 0;
        }
    }

    void FlagInit(int mode)
    {
        // 初始化drawFlags
        for (int i = 0; i < SizeByMode[mode] * SizeByMode[mode] * SizeByMode[mode]; i++)
        {
            drawFlags->push_back(1);
            textureCodes->push_back(0);
        }
    }
};

#endif
