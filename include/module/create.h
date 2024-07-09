#ifndef _CREATE_H_
#define _CREATE_H_

#include <iostream>
#include <vector>

extern int mode;
extern int SizeByMode[3];
extern int BoomByMode[3];

class Block
{
private:
    int x, y, z;
    bool isBoom;
    int boomCount;
    bool is_Clicked;

public:
    Block() {}

    Block(int pos_x, int pos_y, int pos_z, bool boom = false, int count = 0, bool clicked = false)
    {
        x = pos_x;
        y = pos_y;
        z = pos_z;
        isBoom = boom;
        boomCount = count;
        is_Clicked = clicked;
    }
    ~Block() {}

    void setPosition(int pos_x, int pos_y, int pos_z)
    {
        x = pos_x;
        y = pos_y;
        z = pos_z;
    }
    void setX(int pos_x)
    {
        x = pos_x;
    }
    void setY(int pos_y)
    {
        y = pos_y;
    }
    void setZ(int pos_z)
    {
        z = pos_z;
    }
    int getX()
    {
        return x;
    }
    int getY()
    {
        return y;
    }
    int getZ()
    {
        return z;
    }

    bool getIsBoom()
    {
        return isBoom;
    }

    int getBoomCount()
    {
        return boomCount;
    }

    bool getIsClicked()
    {
        return is_Clicked;
    }

    void setIsBoom(bool boom)
    {
        isBoom = boom;
    }

    void setBoomCount(int count)
    {
        boomCount = count;
    }

    void setIsClicked(bool clicked)
    {
        is_Clicked = clicked;
    }

    void boomCountPlus()
    {
        boomCount++;
    }

    void boomCountMinus()
    {
        boomCount--;
    }
};

extern std::vector<Block> frontier;

void initFrontier();
void createBoom(int mode);

#endif
