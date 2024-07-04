#include <iostream>
#include <boomclear/create.h>
#include <boomclear/view.h>
#include <boomclear/event.h>

int main()
{
    int mode = 0;
    std::vector<glm::vec4> frontier = initFrontier(mode);
    std::vector<bool> clickArr = initialClickArr(mode);
    createBoom(frontier, mode);
    test();
    test2();
    return 0;
}
