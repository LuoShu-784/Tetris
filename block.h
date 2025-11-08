/**
 * @file block.h
 * @brief 俄罗斯方块中 'Block' (方块) 类的定义.
 * @details 此文件定义了 Block 类及其相关枚举 (Type, RotationState)
 * 和用于定义方块形状的命名空间 'block'。
 * @author LuoShu
 * @version 1.0
 * @date 2025-11-08
 */
#pragma once

#include <array>
#include <string_view>


/**
 * @brief 代表一个俄罗斯方块的基本单元 (Block).
 * * 封装了方块的类型、位置 (x, y)、旋转状态以及相关的操作 (移动, 旋转, 获取形状数据).
 */
class Block {
public:

    /**
     * @brief 方块的类型 (I, J, L, O, S, T, Z).
     * @note 枚举值从 1 开始, 以便 '0' 可以用于表示游戏区域中的空白.
     */
    enum class Type {
        I = 1,      ///< I 型方块
        J,          ///< J 型方块
        L,          ///< L 型方块
        O,          ///< O 型方块
        S,          ///< S 型方块
        T,          ///< T 型方块
        Z,          ///< Z 型方块
        MAX_NUM     ///< 枚举成员的总数 (用于计数)
    };

    /**
     * @brief 方块的旋转状态 (0, 90, 180, 270 度).
     */
    enum class RotationState {
        Rotation_0,     ///< 0 度旋转
        Rotation_90,    ///< 90 度旋转
        Rotation_180,   ///< 180 度旋转
        Rotation_270,   ///< 270 度旋转
        MAX_NUM         ///< 旋转状态的总数 (用于计数)
    };

    /**
     * @brief Block 类的构造函数.
     * @param[in] type 方块的初始类型.
     * @param[in] x 方块在游戏区域的初始 x 坐标 (通常指 4x4 网格的左上角).
     * @param[in] y 方块在游戏区域的初始 y 坐标.
     * @param[in] rotationState 方块的初始旋转状态.
     */
    Block(Type type, int x, int y, RotationState rotationState);

    /**
     * @brief 移动方块.
     * @param[in] dx x 方向的移动偏移量 (正为右, 负为左).
     * @param[in] dy y 方向的移动偏移量 (正为下).
     */
    void move(int dx, int dy);

    /**
     * @brief 顺时针旋转方块.
     * * 循环更新 m_rotationState.
     */
    void rotate();

    /**
     * @brief 获取方块 4x4 内部网格中指定坐标的值.
     * @param[in] x 内部网格的 x 坐标 (0-3).
     * @param[in] y 内部网格的 y 坐标 (0-3).
     * @return int 如果该位置有方块实体, 返回 (int)m_type; 否则返回 0.
     */
    int getValue(int x, int y) const;

    /**
     * @brief 获取方块当前在游戏区域的 x 坐标 (锚点).
     * @return int x 坐标.
     */
    int getX() const;

    /**
     * @brief 获取方块当前在游戏区域的 y 坐标 (锚点).
     * @return int y 坐标.
     */
    int getY() const;

    /**
     * @brief 获取方块的类型.
     * @return Type 方块的类型 (I, J, L, ...).
     */
    Type getType() const;

private:
    Type m_type;                ///< 方块类型
    int m_x;                    ///< 方块在游戏板上的 x 坐标 (锚点)
    int m_y;                    ///< 方块在游戏板上的 y 坐标 (锚点)
    RotationState m_rotationState; ///< 当前的旋转状态
};


/**
 * @brief 包含与 Block 相关的常量和形状定义.
 */
namespace block
{

inline constexpr int WIDTH { 4 };                 ///< 方块内部网格的宽度
inline constexpr int HEIGHT { 4 };                ///< 方块内部网格的高度
inline constexpr int ROTATION_STATES_NUM { static_cast<int>(Block::RotationState::MAX_NUM) }; ///< 旋转状态的总数 (4)
inline constexpr int TYPE_NUM { static_cast<int>(Block::Type::MAX_NUM) - 1}; ///< 方块类型的总数 (7)

/**
 * @brief 定义一个 4x4 的方块形状 (4 行的 wstring_view).
 */
using Shape = std::array<std::wstring_view, block::HEIGHT>;

/**
 * @brief 存储所有方块类型 (TYPE_NUM) 在所有旋转状态下 (ROTATION_STATES_NUM) 的 4x4 形状定义.
 * * 使用 'X' 表示实体, '.' 表示空白.
 * 访问方式: Shapes[Type索引][Rotation索引][y][x]
 */
inline constexpr std::array<std::array<Shape, ROTATION_STATES_NUM>, TYPE_NUM> Shapes = {{
    // [0] I 方块 (I-Block)
    {{
        { L"..X." , L"..X." , L"..X." , L"..X." },  // Rotation_0
        { L"...." , L"XXXX" , L"...." , L"...." },  // Rotation_90
        { L"..X." , L"..X." , L"..X." , L"..X." },  // Rotation_180
        { L"...." , L"XXXX" , L"...." , L"...." }   // Rotation_270
    }},
    
    // [1] J 方块 (J-Block)
    {{
        { L"..X." , L"..X." , L".XX." , L"...." },  // Rotation_0
        { L"X..." , L"XXX." , L"...." , L"...." },  // Rotation_90
        { L".XX." , L".X.." , L".X.." , L"...." },  // Rotation_180
        { L"XXX." , L"..X." , L"...." , L"...." }   // Rotation_270
    }},
    
    // [2] L 方块 (L-Block) - (已修正 R0 状态)
    {{
        { L".X.." , L".X.." , L".XX." , L"...." },  // Rotation_0
        { L"...." , L"XXX." , L"X..." , L"...." },  // Rotation_90
        { L"XX.." , L".X.." , L".X.." , L"...." },  // Rotation_180
        { L"..X." , L"XXX." , L"...." , L"...." }   // Rotation_270
    }},
    
    // [3] O 方块 (O-Block)
    {{
        { L".XX." , L".XX." , L"...." , L"...." },  // Rotation_0
        { L".XX." , L".XX." , L"...." , L"...." },  // Rotation_90
        { L".XX." , L".XX." , L"...." , L"...." },  // Rotation_180
        { L".XX." , L".XX." , L"...." , L"...." }   // Rotation_270
    }},
    
    // [4] S 方块 (S-Block)
    {{
        { L".XX." , L"XX.." , L"...." , L"...." },  // Rotation_0
        { L".X.." , L".XX." , L"...X" , L"...." },  // Rotation_90
        { L".XX." , L"XX.." , L"...." , L"...." },  // Rotation_180
        { L".X.." , L".XX." , L"...X" , L"...." }   // Rotation_270
    }},
    
    // [5] T 方块 (T-Block)
    {{
        { L".X.." , L"XXX." , L"...." , L"...." },  // Rotation_0
        { L".X.." , L".XX." , L".X.." , L"...." },  // Rotation_90
        { L"...." , L"XXX." , L".X.." , L"...." },  // Rotation_180
        { L".X.." , L"XX.." , L".X.." , L"...." }   // Rotation_270
    }},
    
    // [6] Z 方块 (Z-Block)
    {{
        { L"XX.." , L".XX." , L"...." , L"...." },  // Rotation_0
        { L"..X." , L".XX." , L".X.." , L"...." },  // Rotation_990 (应为 Rotation_90)
        { L"XX.." , L".XX." , L"...." , L"...." },  // Rotation_180
        { L"..X." , L".XX." , L".X.." , L"...." }   // Rotation_270
    }}
}};

}