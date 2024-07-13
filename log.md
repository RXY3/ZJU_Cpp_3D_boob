# 更新日志

## 2024.7.9
替换了点击部分(event.cpp:mouse_button_callback_left)部分的点击，返回最近的中心
函数初始化部分为texture的253行，并在299行使用\


## 2024.7.11
undo:
1. texture与鼠标检测函数传递参数的函数（需要传递cameraPos，view等+xyPos
2. create&judge部分删除全局变量，加上一个&vector《Block》传递参数
3. mouse检测到后的函数需要完成，位置在event.h 35行
4. 


## 2024.7.12
基本完成所有模块
剩余：
1. 贴图
2. model层逻辑检查
3. 结束程序机制以及刚开始的美化