#include <iostream>

#include <ctime>
#include <cstdlib>
#include <vector>

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include <boomclear/create.h>
/*
 * there are three arrays in total:
 * 1. Frontier: a 3D array to store the boom status of each block
 * 2. ClickArr: a 3D array to store the click status of each block
 * 3. Position: a 3D array to store the left-bottom of each block
 */

std::vector<Block> frontier;
int mode;
int SizeByMode[3] = {3, 5, 10};
int BoomByMode[3] = {2, 10, 74};

/*
 * Create a frontier with size*size*size
 * mode: 0 for 3*3*3, 1 for 5*5*5, 2 for 10*10*10
 * Index =  i * size * size + j * size + k
 */
void initFrontier()
{
    int size = SizeByMode[mode];
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            for (int k = 0; k < size; k++)
            {
                frontier.push_back(Block(glm::vec3(float(i), float(j), float(k)), false, 0, false));
            }
        }
    }
}

/*
 * Create a clickArr which is used to store the click status of each block
 * 0 stands for unclicked, 1 stands for clicked
 * later we will draw the blocks according to the click status and the boom status
 */
void createBoom(int mode)
{
    int boomCount = BoomByMode[mode];
    srand(time(NULL));
    for (int i = 0; i < boomCount; i++)
    {
        int index = rand() % frontier.size();
        if (frontier[index].getIsBoom() == false)
        {
            frontier[index].setIsBoom(true);
            frontier[index].setBoomCount(200); // MAX_COUNT
            int x1 = frontier[index].getPosition().x;
            int y1 = frontier[index].getPosition().y;
            int z1 = frontier[index].getPosition().z;
            for (auto &it : frontier)
            {

                int x = it.getPosition().x;
                int y = it.getPosition().y;
                int z = it.getPosition().z;
                if (absFloat(x1 - x) <= 1 && absFloat(y1 - y) <= 1 && absFloat(z1 - z) <= 1)
                    it.boomCountPlus();
            }
        }
        else
            i--;
    }
}