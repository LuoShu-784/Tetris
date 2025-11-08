/**
 * @file block.cpp
 * @brief 俄罗斯方块 'Block' (方块) 类的实现
 * @details 实现了 Block 类中定义的所有成员函数
 * @author LuoShu
 * @version 1.0
 * @date 2025-11-08
 */
#include "block.h"

/**
 * @brief 构造函数实现
 */
Block::Block(Type type, int x, int y, RotationState rotationState)
    : m_type(type), m_x(x), m_y(y), m_rotationState(rotationState) {}

/**
 * @brief 更新方块的位置坐标
 */
void Block::move(int dx, int dy) 
{
    m_x += dx;
    m_y += dy;
}

/**
 * @brief 顺时针旋转方块 (循环切换到下一个旋转状态)
 */
void Block::rotate()
{
    int rotationState = static_cast<int>(m_rotationState);
    m_rotationState = static_cast<RotationState>((rotationState + 1) % block::ROTATION_STATES_NUM);
}

/**
 * @brief 获取方块的类型
 */
Block::Type Block::getType() const
{
    return m_type;
}

/**
 * @brief 获取方块在其 4x4 内部网格中指定位置的值
 */
int Block::getValue(int x, int y) const
{

    if (x < 0 || x >= block::WIDTH || y < 0 || y >= block::HEIGHT) {
        return 0; // 越界检查
    }

    // 类型枚举值从 1 开始 (Type::I = 1), 数组索引从 0 开始
    int typeIndex = static_cast<int>(m_type) - 1;
    int rotationStateIndex = static_cast<int>(m_rotationState);
    
    // 检查 'block::Shapes' 查找表
    return block::Shapes[typeIndex][rotationStateIndex][y][x] == L'X' ? static_cast<int>(m_type) : 0;
}

/**
 * @brief 获取方块在游戏区域的 x 坐标
 */
int Block::getX() const
{
    return m_x;
}

/**
 * @brief 获取方块在游戏区域的 y 坐标
 */
int Block::getY() const
{
    return m_y;
}