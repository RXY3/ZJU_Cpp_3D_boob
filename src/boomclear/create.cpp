#include <boomclear/create.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

/*
 * there are three arrays in total:
 * 1. Frontier: a 3D array to store the boom status of each block
 * 2. ClickArr: a 3D array to store the click status of each block
 * 3. Position: a 3D array to store the left-bottom of each block
 */

int SizeByMode[3] = {3, 5, 10};
int BoomByMode[3] = {2, 10, 74};

/*
 * Create a frontier with size*size*size
 * mode: 0 for 3*3*3, 1 for 5*5*5, 2 for 10*10*10
 */
std::vector<glm::vec4> initFrontier(int mode)
{
    int size = SizeByMode[mode];
    std::vector<glm::vec4> frontier;
    for (int i = (-size) / 2; i < (size + 1) / 2; i++)
    {
        for (int j = (-size) / 2; j < (size + 1) / 2; j++)
        {
            for (int k = (-size) / 2; k < (size + 1) / 2; k++)
            {
                frontier.push_back(glm::vec4(i, j, k, 0));
            }
        }
    }
    return frontier;
}

/*
 * Create a clickArr which is used to store the click status of each block
 * 0 stands for unclicked, 1 stands for clicked
 * later we will draw the blocks according to the click status and the boom status
 */
std::vector<bool> initialClickArr(int mode)
{
    int size = SizeByMode[mode];
    std::vector<bool> clickArr;
    for (int i = 0; i < size * size * size; i++)
    {
        clickArr.push_back(false);
    }
    return clickArr;
}

void createBoom(std::vector<glm::vec4> frontier, int mode)
{
    int boomCount = BoomByMode[mode];
    srand(time(NULL));
    for (int i = 0; i < boomCount; i++)
    {
        int index = rand() % frontier.size();
        if (!frontier[index].w)
            frontier[index].w = 1;
        else
            i--;
    }
}

void destroyFrontier(std::vector<glm::vec4> frontier)
{
    frontier.clear();
}

void destroyClickArr(std::vector<bool> clickArr)
{
    clickArr.clear();
}