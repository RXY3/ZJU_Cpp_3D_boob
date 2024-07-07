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

std::vector<Block> frontier;
int mode;
int SizeByMode[3] = {3, 5, 10};
int BoomByMode[3] = {2, 10, 74};
float BlockLength[3] = {0.6, 0.4, 0.1};

inline float absFloat(float x)
{
    return x > 0 ? x : -x;
}

/*
 * Create a frontier with size*size*size
 * mode: 0 for 3*3*3, 1 for 5*5*5, 2 for 10*10*10
 * Index =  i * size * size + j * size + k
 */
void initFrontier()
{
    int size = SizeByMode[mode];
    int length = BlockLength[mode];
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            for (int k = 0; k < size; k++)
            {
                frontier.push_back(Block(glm::vec3(i * length + length / 2 - 1, j * length + length / 2 - 1, k * length + length / 2 - 1), false, 0, false));
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
            int origin_x = frontier[index].getPosition().x;
            int origin_y = frontier[index].getPosition().y;
            int origin_z = frontier[index].getPosition().z;
            for (auto &it : frontier)
            {

                int x = it.getPosition().x;
                int y = it.getPosition().y;
                int z = it.getPosition().z;
                if ((x == origin_x || absFloat(x - origin_x) == BlockLength[mode]) &&
                    (y == origin_y || absFloat(y - origin_y) == BlockLength[mode]) &&
                    (z == origin_z || absFloat(z - origin_z) == BlockLength[mode]))
                    it.boomCountPlus();
            }
        }
        else
            i--;
    }
}