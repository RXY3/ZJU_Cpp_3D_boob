#include <iostream>

#include <module/create.h>
#include <view_module/event.h>
#include <view/view.h>
#include <view/texture.h>
#include <windows.h>

int main()
{
    mode = 0;
    initFrontier();
    for (auto &it : frontier)
    {
        std::cout << it.getX() << " " << it.getY() << " " << it.getZ() << std::endl;
    }
    int numCubes = 27; // 3x3x3 cubes
    std::vector<int> drawFlags = {
        1, 1, 1, 0, 0, 0, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1}; // All cubes drawn

    std::vector<int> textureCodes = {
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0}; // Alternating textures

    renderCubes(numCubes, drawFlags, textureCodes);

    return 0;
}
