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

            if (absFloat(x1 - x) <= 1 && absFloat(y1 - y) <= 1 && absFloat(z1 - z) <= 1)
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

void boomClear(Block &boom_pos)
{
    float x = boom_pos.getPosition().x;
    float y = boom_pos.getPosition().y;
    float z = boom_pos.getPosition().z;
    for (auto &it : frontier)
    {
        float x1 = it.getPosition().x;
        float y1 = it.getPosition().y;
        float z1 = it.getPosition().z;
        if (absFloat(x1 - x) <= 1 && absFloat(y1 - y) <= 1 && absFloat(z1 - z) <= 1)
        {
            it.boomCountMinus();
        }
    }
    bfsBoomZero(boom_pos);
}

void boomDel()
{
    for (auto it = frontier.begin(); it != frontier.end();)
    {
        if (it->getIsBoom())
        {
            bool hasUnclickedNonBoom = false;
            int x = it->getPosition().x;
            int y = it->getPosition().y;
            int z = it->getPosition().z;

            // 遍历周围的块
            for (auto &neighbor : frontier)
            {
                int x1 = neighbor.getPosition().x;
                int y1 = neighbor.getPosition().y;
                int z1 = neighbor.getPosition().z;

                if (absFloat(x1 - x) <= 1 && absFloat(y1 - y) <= 1 && absFloat(z1 - z) <= 1)
                {
                    if (!neighbor.getIsClicked() && !neighbor.getIsBoom())
                    {
                        hasUnclickedNonBoom = true;
                        break; // 只要找到一个未点击的非boom块，就跳出循环
                    }
                }
            }

            if (!hasUnclickedNonBoom)
            {
                boomClear(*it);
                it = frontier.erase(it); // 如果没有未点击的非boom块，删除当前boom块
            }
            else
            {
                ++it; // 如果找到了未点击的非boom块，继续查找下一个boom块
            }
        }
        else
        {
            ++it; // 如果当前块不是boom块，继续查找下一个块
        }
    }
}

void resultLeft(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        Block click_pos;
        mouse_button_callback_left(window, button, action, mods, &click_pos);
        // std::cout << click_pos.getPosition().x << " " << click_pos.getPosition().y << " " << click_pos.getPosition().z << std::endl;
        for (auto it = frontier.begin(); it != frontier.end(); ++it)
        {
            if (it->getPosition() == click_pos.getPosition())
            {
                it->setIsClicked(true);
                if (it->getIsBoom())
                {
                    std::cout << "Boom! You lose!" << std::endl;
                    glfwSetWindowShouldClose(window, GLFW_TRUE);
                }
                else
                {
                    if (it->getBoomCount() == 0)
                    {
                        Block temp = *it;        // 复制要删除的块
                        it = frontier.erase(it); // 删除块
                        bfsBoomZero(temp);       // 使用复制的块调用 bfsBoomZero
                    }
                    else
                    {
                        ++it; // 增加迭代器
                    }
                }
                break; // 一旦找到并处理目标块，跳出循环
            }
        }
        return;
    }
}

void resultRight(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        Block click_pos;
        mouse_button_callback_right(window, button, action, mods, &click_pos);
        for (auto it = frontier.begin(); it != frontier.end();)
        {
            if (it->getPosition() == click_pos.getPosition())
            {
                it->setIsClicked(true);
                if (it->getIsBoom())
                {
                    boomClear(*it);
                    it = frontier.erase(it); // 删除元素并更新迭代器
                }
                else
                {
                    std::cout << "Not a boom! You lose!" << std::endl;
                    glfwSetWindowShouldClose(window, GLFW_TRUE);
                    break; // 一旦找到并处理目标块，跳出循环
                }
            }
            else
            {
                ++it; // 仅在未删除元素时递增迭代器
            }
        }
        return;
    }
}
