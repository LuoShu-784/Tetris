/**
 * @file main.cpp
 * @brief 俄罗斯方块游戏的主入口
 * @author LuoShu
 * @version 1.0
 * @date 2025-11-09
 */
#include "tetris.h"
#include <iostream>
#include <stdexcept>

/**
 * @brief 程序主函数 (入口)
 */
int main()
{
    // Screen() 构造函数在失败时会抛出 std::runtime_error
    try 
    {
        Tetris game;
        game.run();
    }
    catch (const std::exception& e)
    {
        // 如果控制台创建失败, 优雅地退出
        std::wcerr << L"Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}