#include <boomclear/create.h>
#include <iostream>
#include <ctime>
#include <cstdlib>

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
int ***initFrontier(int mode)
{
    int size = SizeByMode[mode];
    int ***frontier = new int **[size];
    for (int i = 0; i < size; i++)
    {
        frontier[i] = new int *[size];
        for (int j = 0; j < size; j++)
        {
            frontier[i][j] = new int[size];
            for (int k = 0; k < size; k++)
            {
                frontier[i][j][k] = 0;
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
int ***initialClickArr(int mode)
{
    int size = SizeByMode[mode];
    int ***clickArr = new int **[size];
    for (int i = 0; i < size; i++)
    {
        clickArr[i] = new int *[size];
        for (int j = 0; j < size; j++)
        {
            clickArr[i][j] = new int[size];
            for (int k = 0; k < size; k++)
            {
                clickArr[i][j][k] = 0;
            }
        }
    }
    return clickArr;
}
/*
 * Create a 3D array to store the left_bottom position of each block
 */
double ***initPosition(double ***block_pos, int mode)
{
    int size = SizeByMode[mode];
    block_pos = new double **[size];
    for (int i = 0; i < size; i++)
    {
        block_pos[i] = new double *[size];
        for (int j = 0; j < size; j++)
        {
            block_pos[i][j] = new double[size];
            for (int k = 0; k < size; k++)
            {
                block_pos[i][j][k] = 0;
            }
        }
    }
    return block_pos;
}

void createBoom(int ***frontier, int mode)
{
    int size = SizeByMode[mode];
    int boomCount = BoomByMode[mode];
    srand(time(NULL));
    while (boomCount > 0)
    {
        int i = rand() % size;
        int j = rand() % size;
        int k = rand() % size;
        if (frontier[i][j][k] != -1)
        {
            frontier[i][j][k] = -1;
            boomCount--;
        }
    }
}

void printFrontier(int ***frontier, int mode)
{
    int size = SizeByMode[mode];
    for (int i = 0; i < size; i++)
    {
        std::cout << "layer: " << i << std::endl;
        for (int j = 0; j < size; j++)
        {
            for (int k = 0; k < size; k++)
                std::cout << frontier[i][j][k] << " ";
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

void destroyFrontier(int ***frontier, int mode)
{
    if (frontier == NULL)
        return;
    int size = SizeByMode[mode];
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            delete[] frontier[i][j];
        }
        delete[] frontier[i];
    }
    delete[] frontier;
}

void destroyClickArr(int ***clickArr, int mode)
{
    if (clickArr == NULL)
        return;
    int size = SizeByMode[mode];
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            delete[] clickArr[i][j];
        }
        delete[] clickArr[i];
    }
    delete[] clickArr;
}

void destroyPosition(double ***block_pos, int mode)
{
    if (block_pos == NULL)
        return;
    int size = SizeByMode[mode];
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            delete[] block_pos[i][j];
        }
        delete[] block_pos[i];
    }
    delete[] block_pos;
}