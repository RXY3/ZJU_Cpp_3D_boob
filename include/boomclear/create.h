#ifndef _CREATE_H_
#define _CREATE_H_

#include <vector>
#include <glm/glm.hpp>
#include <iostream>

extern int SizeByMode[3];
extern int BoomByMode[3];

class Block
{
private:
    glm::vec3 position;
    bool isBoom;
    int boomCount;
    bool is_Clicked;

public:
    Block(glm::vec3 pos, bool boom = false, int count = 0, bool clicked = false)
    {
        position = pos;
        isBoom = boom;
        boomCount = count;
        is_Clicked = clicked;
    }
    ~Block() {}

    glm::vec3 getPosition()
    {
        return position;
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
};

std::vector<Block> initFrontier(int mode);
void createBoom(std::vector<Block> frontier, int mode);

#endif
