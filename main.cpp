#include <iostream>
#include <module/create.h>
#include <view_module/event.h>
#include <view/texture.h>
#include <windows.h>
#include <CommandBus.h>

int main()
{
    // mode = 0;
    // initFrontier();
    // for (auto &it : frontier)
    // {
    //     std::cout << it.getX() << " " << it.getY() << " " << it.getZ() << std::endl;
    // }
    
    std::vector<int> drawFlagsTmp; // All cubes drawn

    std::vector<int> textureCodesTmp; // Alternating textures

    int mode;
    std :: cin >> mode;
    int SizeByMode[3] = {3, 5, 10};
    int numCubes = SizeByMode[mode] * SizeByMode[mode] * SizeByMode[mode];
    
    //std::cout << "check";
    /*TODO
    1.vector绑定
    2. while实现
    3. view内部函数封装以及实现
    4. 点击事件处理函数封装
    5. 点击事件传递
    6. …………
    */
    CommandBus commandBus;
    //初始化顺寻因为是要庸view控制viewmodel，需要先初始化view
    
    std::vector<int>* drawFlagsPtr = &drawFlagsTmp;
    
    std::vector<int>* textureCodesPtr = &textureCodesTmp;
    //std::cout << "check";
    class View view(drawFlagsPtr, textureCodesPtr, commandBus);
   // std::cout << "check";

    class ViewModel vm(mode,commandBus,drawFlagsPtr,textureCodesPtr);
    // view.init();

    // while()
    // {
    //     view.beforeclick();//点击前的函数
    //     if(view.flag)//flag代表点击
    //     {
    //         vm.click(view.closestVertexIndex);
    //         view.flag = false;
    //     }
    //     view.after();

    // }
    view.init(numCubes);
    while (!view.shouldClose() && !vm.isGameOver){
        view.renderCubes(numCubes);
        if (view.getFlag())
        {
            // std:: cout << "view";
            // std::cout << view.cameraPos.x << " " << view.cameraPos.y << " " << view.cameraPos.z << std::endl;
            // std::cout << view.xPos << " " << view.yPos << std::endl;
            vm.cameraSync(view.cameraPos, view.objectPos, view.view, view.xPos, view.yPos);
        }
    }
    view.cleanUp();
    return 0;
}
