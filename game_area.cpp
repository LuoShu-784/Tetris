/**
 * @file game_area.cpp
 * @brief GameArea 类的实现
 * @author LuoShu
 * @version 1.0
 * @date 2025-11-08
 */
#include "game_area.h"
#include <algorithm>

/**
 * @brief 构造函数, 调用 clear() 初始化网格
 */
GameArea::GameArea()
{
    clear();
}

/**
 * @brief 碰撞检测
 */
bool GameArea::isCollision(const Block& block) const
{
    for (int y = 0; y < block::HEIGHT; y++)
    {
        for (int x = 0; x < block::WIDTH; x++)
        {
            if (!block.getValue(x, y)) {
                continue;
            }

            int AreaX = block.getX() + x;
            int AreaY = block.getY() + y;

            if (AreaY < 0) {
                continue; // 允许在顶部边界之上
            }

            if (!isInSide(AreaX, AreaY)) {
                return true; // 碰到了左右或下边界
            }

            if (isOccupied(AreaX, AreaY)) {
                return true; // 碰到了已固定的方块
            }
        }
    }
    return false;
}

/**
 * @brief 锁定方块
 */
void GameArea::lockBlock(const Block& block)
{
    for (int y = 0; y < block::HEIGHT; y++)
    {
        for (int x = 0; x < block::WIDTH; x++)
        {
            int value = block.getValue(x, y);
            if (!value) {
                continue;
            }

            int AreaX { block.getX() + x };
            int AreaY { block.getY() + y };

            if (isInSide(AreaX, AreaY)) {
                m_grid[AreaY][AreaX] = value;
            }
        }
    }
}

/**
 * @brief 消除满行,并返回消除的行数
 */
int GameArea::clearFullLines()
{
    int linesCleared { 0 };
    int py { gamearea::HEIGHT - 1 }; // 目标行 (写入)
    for (int y = gamearea::HEIGHT - 1; y >= 0; y--) // 原始行 (读取)
    {
        if (isLineFull(y)) {
            linesCleared++;
            continue; // 如果 y 行满了, 跳过 (即删除)
        }

        // 如果 y 行未满, 且 y 和 py 不在同一行
        if (y != py) {
            m_grid[py] = m_grid[y]; // 将 y 行复制到 py 行
        }

        --py; // 目标行上移
    }

    // 将剩余的顶部行 (py 及以上) 清零
    while (py >= 0) {
        std::fill(m_grid[py].begin(), m_grid[py].end(), 0);
        --py;
    }
    return linesCleared;
}

/**
 * @brief 清空网格
 */
void GameArea::clear()
{
    for (auto &py : m_grid) {
        std::fill(py.begin(), py.end(), 0);
    }
}

/**
 * @brief 获取网格值
 */
int GameArea::getValue(int x, int y) const
{
    if (!isInSide(x, y)) {
        return 0;
    }
    return m_grid[y][x];
}

/**
 * @brief 边界检查
 */
bool GameArea::isInSide(int x, int y) const
{
    return x >= 0 && x < gamearea::WIDTH && y >= 0 && y < gamearea::HEIGHT;
}

/**
 * @brief 占用检查
 */
bool GameArea::isOccupied(int x, int y) const
{
    return m_grid[y][x] != 0;
}

/**
 * @brief 满行检查
 */
bool GameArea::isLineFull(int y) const
{
    const auto &py { m_grid[y] };
    return std::all_of(py.begin(), py.end(), [](int i) { return i != 0; });
}