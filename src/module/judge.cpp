#include <iostream>

#include <module/create.h>
#include <module/judge.h>

std::vector<Block> zeroCount;

void bfsBoomZero(Block &click_pos, std::vector<Block> &frontier) // bfs to clear all the zero boom count blocks
{
    zeroCount.push_back(click_pos);
    while (!zeroCount.empty())
    {
        Block now = zeroCount.back();
        zeroCount.pop_back();
        int x = now.getX();
        int y = now.getY();
        int z = now.getZ();

        for (auto it = frontier.begin(); it != frontier.end();)
        {
            int x1 = it->getX();
            int y1 = it->getY();
            int z1 = it->getZ();

            if (abs(x1 - x) <= 1 && abs(y1 - y) <= 1 && abs(z1 - z) <= 1)
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

void boomClear(Block &boom_pos, std::vector<Block> &frontier) // delete all the Blocks near boom_pos from frontier
{
    int x = boom_pos.getX();
    int y = boom_pos.getY();
    int z = boom_pos.getZ();
    for (auto &it : frontier)
    {
        float x1 = it.getX();
        float y1 = it.getY();
        float z1 = it.getZ();
        if (abs(x1 - x) <= 1 && abs(y1 - y) <= 1 && abs(z1 - z) <= 1)
        {
            it.boomCountMinus();
        }
    }
    bfsBoomZero(boom_pos, frontier);
}

void boomDel(std::vector<Block> &frontier)
{
    if (frontier.empty())
    {
        return;
    }
    for (auto it = frontier.begin(); it != frontier.end();)
    {
        if (it->getIsBoom())
        {
            bool hasUnclickedNonBoom = false;
            int x = it->getX();
            int y = it->getY();
            int z = it->getZ();

            // 遍历周围的块
            for (auto &neighbor : frontier)
            {
                int x1 = neighbor.getX();
                int y1 = neighbor.getY();
                int z1 = neighbor.getZ();

                if (abs(x1 - x) <= 1 && abs(y1 - y) <= 1 && abs(z1 - z) <= 1)
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
                Block temp = *it;
                it = frontier.erase(it); // 删除元素并更新迭代器
                boomClear(temp, frontier);
                boomDel(frontier);
            }
            else
            {
                ++it; // 如果找到了未点击的非boom块，继续查找下一个boom块
            }

            if (frontier.empty())
                break;
        }
        else
        {
            ++it; // 如果当前块不是boom块，继续查找下一个块
        }
    }
}

bool resultLeft(Block &click_pos, std::vector<Block> &frontier)
{
    for (auto it = frontier.begin(); it != frontier.end(); ++it)
    {
        if (it->getX() == click_pos.getX() && it->getY() == click_pos.getY() && it->getZ() == click_pos.getZ())
        {
            it->setIsClicked(true);
            if (it->getIsBoom())
            {
                std::cout << "Boom! You lose!" << std::endl;
                return true;
            }
            else
            {
                if (it->getBoomCount() == 0)
                {
                    Block temp = *it;        // 复制要删除的块
                    it = frontier.erase(it); // 删除块
                    bfsBoomZero(temp, frontier);
                    boomDel(frontier);
                }
                else
                {
                    boomDel(frontier);
                }
            }
            break; // 一旦找到并处理目标块，跳出循环
        }
    }
    return false;
}

bool resultRight(Block &click_pos, std::vector<Block> &frontier)
{
    for (auto it = frontier.begin(); it != frontier.end();)
    {
        if (it->getX() == click_pos.getX() && it->getY() == click_pos.getY() && it->getZ() == click_pos.getZ())
        {
            if (it->getIsBoom())
            {
                it->setIsClicked(true);
                Block temp = *it;
                it = frontier.erase(it); // 删除元素并更新迭代器
                boomClear(temp, frontier);
                boomDel(frontier);
                break;
            }
            else
            {
                std::cout << "Not a boom! You lose!" << std::endl;
                return true; // 一旦找到并处理目标块，跳出循环
            }
        }
        else
        {
            ++it; // 仅在未删除元素时递增迭代器
        }
    }
    return false;
}
