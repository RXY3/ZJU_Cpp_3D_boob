#include <boomclear/judge.h>
#include <GLFW/glfw3.h>
#include <boomclear/event.h>
#include <boomclear/create.h>
#include <glm/glm.hpp>
#include <iostream>

std::vector<Block> zeroCount;

void bfsBoomZero(Block &click_pos) // delete all the Blocks whose boomcount == 0 and near click_pos from frontier
{
    zeroCount.push_back(click_pos);
    while (!zeroCount.empty())
    {
        Block now = zeroCount.back();
        zeroCount.pop_back();
        int x = now.getPosition().x;
        int y = now.getPosition().y;
        int z = now.getPosition().z;

        for (auto it = frontier.begin(); it != frontier.end();)
        {
            int x1 = it->getPosition().x;
            int y1 = it->getPosition().y;
            int z1 = it->getPosition().z;

            if ((x1 == x || absFloat(x1 - x) == BlockLength[mode]) &&
                (y1 == y || absFloat(y1 - y) == BlockLength[mode]) &&
                (z1 == z || absFloat(z1 - z) == BlockLength[mode]))
            {
                if (it->getBoomCount() == 0)
                {
                    zeroCount.push_back(*it);
                    it = frontier.erase(it); // 删除元素并更新迭代器
                }
                else
                {
                    if (!it->getIsClicked() && !it->getIsBoom())
                    {
                        it->setIsClicked(true);
                    }
                    ++it; // 仅在未删除元素时递增迭代器
                }
            }
            else
            {
                ++it; // 仅在未删除元素时递增迭代器
            }
        }
    }
    return;
}

void resultLeft(GLFWwindow *window, int button, int action, int mods)
{
    Block click_pos;
    mouse_button_callback(window, button, action, mods, &click_pos);
    // std::cout << click_pos.getPosition().x << " " << click_pos.getPosition().y << " " << click_pos.getPosition().z << std::endl;
    for (auto &it : frontier)
    {
        if (it.getPosition() == click_pos.getPosition())
        {
            it.setIsClicked(true);
            if (it.getIsBoom())
            {
                std::cout << "Boom! You lose!" << std::endl;
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
            else
            {
                if (it.getBoomCount() == 0)
                {
                    bfsBoomZero(it);
                    break;
                }
                else
                    break;
            }
        }
    }
    return;
}

void resultRight(GLFWwindow *window, int button, int action, int mods)
{
    Block click_pos;
    mouse_button_callback(window, button, action, mods, &click_pos);
    // std::cout << click_pos.getPosition().x << " " << click_pos.getPosition().y << " " << click_pos.getPosition().z << std::endl;
    for (auto &it : frontier)
    {
        if (it.getPosition() == click_pos.getPosition())
        {
            it.setIsClicked(true);
            if (it.getIsBoom())
            {
                std::cout << "Boom! You lose!" << std::endl;
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
            else
            {
                if (it.getBoomCount() == 0)
                {
                    bfsBoomZero(it);
                    break;
                }
                else
                    break;
            }
        }
    }
    return;
}