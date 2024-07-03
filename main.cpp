#include <iostream>
#include <boomclear/create.h>
#include <boomclear/view.h>
#include <boomclear/event.h>

int main()
{
    int mode = 0;
    int ***frontier = initFrontier(mode);
    createBoom(frontier, mode);
    printFrontier(frontier, mode);
    test();
    test2();
    return 0;
}
