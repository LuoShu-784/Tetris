/**
 * @file tetris.h
 * @brief 定义俄罗斯方块 (Tetris) 游戏的主逻辑类
 * @details 此文件包含 Tetris 类的定义, 该类封装了整个游戏的状态、逻辑、渲染 (render) 以及所有游戏组件 
 * @author LuoShu
 * @version 1.0
 * @date 2025-11-09
 */
#pragma once

#include "game_area.h"
#include "screen.h"
#include "block.h"

#include <chrono>
#include <random>
#include <array>

/**
 * @brief 包含与 Tetris 游戏逻辑相关的常量.
 */
namespace tetris { 

inline constexpr int AREA_OFFSET_X { 2 };                                               ///< 游戏区域在屏幕上的 X 偏移量
inline constexpr int AREA_OFFSET_Y { 1 };                                               ///< 游戏区域在屏幕上的 Y 偏移量
inline constexpr int UI_OFFSET_X { gamearea::WIDTH * 2 + AREA_OFFSET_X + 3 };           ///< UI 区域在屏幕上的 X 偏移量
inline constexpr int UI_OFFSET_Y { AREA_OFFSET_Y };                                     ///< UI 区域在屏幕上的 Y 偏移量
inline constexpr int BLOCK_GENERATE_X { (gamearea::WIDTH / 2) - (block::WIDTH / 2) };   ///< 新方块生成的 X 坐标
inline constexpr int BLOCK_GENERATE_Y { 0 };                                            ///< 新方块生成的 Y 坐标

inline constexpr int VIRTUAL_KEY_COUNT { 256 };                                         ///< 虚拟键码的最大数量
inline constexpr SHORT KEY_PRESSED_BIT { static_cast<SHORT>(0x8000) };                    ///< 用于检测按键是否按下的位掩码

};

/**
 * @brief 俄罗斯方块游戏的主类
 * @details
 * 封装了游戏循环、状态更新、输入处理和屏幕渲染, 管理游戏区域、屏幕和当前的方块
 */
class Tetris { 
public:
    /**
     * @brief 构造函数
     * @details 初始化屏幕、游戏区域、随机数生成器等
     */
    Tetris();

    /**
     * @brief 启动并运行游戏主循环.
     * @details 在循环中处理按键、更新游戏状态 (基于下落速度) 和渲染屏幕
     * 游戏结束后, 显示 "GAME OVER" 界面并等待 ESC 退出
     */
    void run();

private:
    /**
     * @brief 处理用户输入.
     * @details 检查按键 (左/右/上/下/空格/ESC),并调用相应的动作
     */
    void processKey();

    /**
     * @brief 检查 Windows 虚拟键码 (vKey) 当前是否被按下
     * @param[in] vKey 要检查的虚拟键码 (例如 VK_LEFT)
     * @return bool 如果键被按下, 返回 true
     */
    bool isKeyPressed(int vKey) const;

    /**
     * @brief 更新游戏状态 (处理方块自动下落)
     * @details 尝试向下移动一格, 如果发生碰撞:
     * 1. 锁定方块 
     * 2. 检查并清除满行, 并更新分数
     * 3. 生成新方块
     */
    void update();

    /**
     * @brief 生成一个新的方块
     */
    void generateBlock();

    /**
     * @brief (尝试) 移动当前方块
     * @param[in] dx X 方向的偏移量
     * @param[in] dy Y 方向的偏移量
     */
    void moveBlock(int dx, int dy);

    /**
     * @brief (尝试) 旋转当前方块
     */
    void rotateBlock();

    /**
     * @brief "硬"下落当前方块
     * @details 立即将方块移动到它能下落到的最低位置,
     * 然后锁定方块、清行、生成新方块
     */
    void hardDropBlock();

    /**
     * @brief 根据消除的行数更新分数、总行数和等级
     * @param[in] linesCleared 本次消除的行数
     */
    void updateScore(int linesCleared);

    /**
     * @brief 随机生成一个方块类型
     * @return Block::Type 随机的方块类型 (I, J, L, ... Z)
     */
    Block::Type getRandomBlockType();

    /**
     * @brief 根据当前等级计算方块的自动下落速度
     * @return std::chrono::milliseconds 下落一步所需的时间 (毫秒)
     */
    std::chrono::milliseconds getFallSpeed() const;

    /**
     * @brief 渲染所有游戏元素到后台缓冲区
     */
    void render();

    /**
     * @brief 获取方块类型的 Windows COLORREF
     * @param[in] blockType 方块类型
     * @return COLORREF (Windows GDI 颜色)
     */
    COLORREF getBlockColor(Block::Type blockType) const;

    /**
     * @brief 在屏幕上绘制一个方块
     * @param[in] block 要绘制的方块
     * @param[in] xOffset 方块绘制的 X 屏幕偏移量
     * @param[in] yOffset 方块绘制的 Y 屏幕偏移量
     */
    void drawBlock(const Block& block, int xOffset, int yOffset);

    /**
     * @brief 绘制游戏区域的边框、已锁定的方块以及当前正在下落的方块
     */
    void drawGameArea();

    /**
     * @brief 绘制右侧的用户界面 (分数、等级、行数、下一个方块、控制说明)
     */
    void drawUI();

    Screen m_screen;                                ///< 屏幕(控制台)渲染器
    GameArea m_gameArea;                            ///< 游戏区域(网格)逻辑

    std::mt19937 m_rng;                             ///< 随机数生成器
    std::uniform_int_distribution<int> m_blockDist; ///< 方块类型均匀分布器

    Block m_currentBlock;           ///< 当前正在操作的方块
    Block m_nextBlock;              ///< "下一个" 区域中显示的方块

    bool m_isGameOver { false };    ///< 游戏是否结束的标志
    int m_score { 0 };              ///< 当前得分
    int m_level { 1 };              ///< 当前等级
    int m_linesCleared { 0 };       ///< 累计消除的总行数

    using clock = std::chrono::steady_clock;                    ///< 定义时钟类型别名
    std::chrono::time_point<clock> m_lastFallTime;              ///< 上次方块自动下落的时间点

    std::array<bool, tetris::VIRTUAL_KEY_COUNT> m_keyState {};  ///< 按键状态数组 (用于检测按键"按下"的瞬间)
};