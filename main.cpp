#include <iostream>

#include <boomclear/create.h>
#include <boomclear/event.h>
#include <boomclear/view.h>
#include <boomclear/texture.h>

int main()
{
    mode = 0;
    initFrontier();
    for (auto &it : frontier)
    {
        std::cout << it.getPosition().x << " " << it.getPosition().y << " " << it.getPosition().z << std::endl;
    }
    int numCubes = 27; // 3x3x3 cubes
    std::vector<int> drawFlags = {
        1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1}; // All cubes drawn

    std::vector<int> textureCodes = {
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0}; // Alternating textures

    renderCubes(numCubes, drawFlags, textureCodes);

    return 0;
}
