/**
 * @file game_areah
 * @brief 定义俄罗斯方块的游戏区域 (GameArea) 类
 * @details 此文件定义了 GameArea 类, 负责管理游戏区域网格、
 * 碰撞检测、锁定方块以及清除满行
 * @author LuoShu
 * @version 10
 * @date 2025-11-08
 */
#pragma once

#include "block.h"
#include <array>

/**
 * @brief 包含与游戏区域相关的常量
 */
namespace gamearea {

    inline constexpr int WIDTH { 12 };  ///< 游戏区域的宽度 (单位: 格)
    inline constexpr int HEIGHT { 18 }; ///< 游戏区域的高度 (单位: 格)

};

/**
 * @brief 管理俄罗斯方块的游戏区域 (网格)
 * @details 
 * 负责存储已固定的方块 (m_grid),
 * 检测新方块是否与边界或已固定方块发生碰撞 (isCollision),
 * 将方块固定到网格 (lockBlock), 以及处理行消除 (clearFullLines)
 */
class GameArea { 
public:

    /**
     * @brief 构造函数
     * @details 初始化游戏区域, 调用 clear() 将网格清零
     */
    GameArea();

    /**
     * @brief 检查指定方块是否与边界或已固定的方块发生碰撞
     * @details 遍历方块的 4x4 内部网格,
     * 将其转换为游戏区域坐标, 然后检查是否越界或 m_grid 中对应位置已被占用
     * @param[in] block 要检测的方块 (包含其当前位置、类型和旋转状态)
     * @return bool 如果发生碰撞 (或即将越界), 返回 true; 否则返回 false
     */
    bool isCollision(const Block& block) const;

    /**
     * @brief 将一个方块 "锁定" (固定) 到游戏区域网格上
     * @details 遍历方块的 4x4 内部网格,
     * 将方块的实体部分 (非 0 值) 复制到 m_grid 的对应位置
     * @param[in] block 要锁定的方块
     */
    void lockBlock(const Block& block);

    /**
     * @brief 检查并清除所有已满的行,并返回消除的行数
     * @details 从下到上扫描网格, 如果发现某行已满则跳过 (不复制),
     * 将未满的行向下移动以覆盖已满的行, 最后将顶部的空行清零
     */
    int clearFullLines();

    /**
     * @brief 清空 (重置) 整个游戏区域网格, 全部填充为 0
     */
    void clear();

    /**
     * @brief 获取游戏区域网格中指定坐标的值
     * @param[in] x 要查询的 x 坐标
     * @param[in] y 要查询的 y 坐标
     * @return int 该位置的值 (0 表示空白, >0 表示方块类型) 如果坐标越界, 返回 0
     */
    int getValue(int x, int y) const;

private:

    /**
     * @brief 检查坐标 (x, y) 是否在游戏区域的有效范围内
     * @param[in] x x 坐标
     * @param[in] y y 坐标
     * @return bool 如果 (0 <= x < WIDTH) 且 (0 <= y < HEIGHT), 返回 true
     */
    bool isInSide(int x, int y) const;

    /**
     * @brief 检查游戏区域中的指定坐标是否已被占用 (非 0)
     * @param[in] x x 坐标
     * @param[in] y y 坐标
     * @return bool 如果 m_grid[y][x] != 0, 返回 true
     */
    bool isOccupied(int x, int y) const;

    /**
     * @brief 检查指定行 (y) 是否已满 (所有格子都不为 0)
     * @param[in] y 要检查的行索引
     * @return bool 如果该行所有格子都不为 0, 返回 true
     */
    bool isLineFull(int y) const;

    /**
     * @brief 游戏区域的二维网格
     * @details 存储所有已锁定的方块 0 代表空白,
     * 大于 0 的值代表对应 Block::Type 的方块
     */
    std::array<std::array<int, gamearea::WIDTH>, gamearea::HEIGHT> m_grid {};

};