#include <iostream>
#include <boomclear/create.h>
#include <boomclear/view.h>
#include <boomclear/event.h>
#include <windows.h>

int main()
{
    mode = 0;
    initFrontier();
    for (auto &it : frontier)
    {
        std::cout << it.getPosition().x << " " << it.getPosition().y << " " << it.getPosition().z << std::endl;
    }
    

    system("pause");
    return 0;
}
