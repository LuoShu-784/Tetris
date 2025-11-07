#pragma once

#include <array>
#include <string_view>


class Block {
public:

    enum class Type {
        I = 1,
        J,
        L,
        O,
        S,
        T,
        Z,
        MAX_NUM
    };

    enum class RotationState {
        Rotation_0,
        Rotation_90,
        Rotation_180,
        Rotation_270,
        MAX_NUM
    };

    Block(Type type, int x, int y, RotationState rotationState);

    void move(int dx, int dy);
    void rotate();

    int getValue(int x, int y) const;
    int getX() const;
    int getY() const;

    Type getType() const;

private:
    Type m_type;
    int m_x;
    int m_y;
    RotationState m_rotationState;
};


namespace block
{

inline constexpr int WIDTH { 4 };
inline constexpr int HEIGHT { 4 };
inline constexpr int ROTATION_STATES_NUM { static_cast<int>(Block::RotationState::MAX_NUM) };
inline constexpr int TYPE_NUM { static_cast<int>(Block::Type::MAX_NUM) - 1};

using Shape = std::array<std::wstring_view, block::HEIGHT>;

/**
 * @brief 存储所有方块类型 (Type) 的所有旋转状态 (RotationState) 的形状 (Shape)
 * * 访问方式: Shapes[Type][RotationState][y][x]
 * * 注意：
 * 1. Type 索引 = static_cast<int>(Block::Type) - 1
 * 2. RotationState 索引 = static_cast<int>(Block::RotationState)
 * 3. y 索引 = 0-3 (行)
 * 4. x 索引 = 0-3 (列)
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
        { L"XXX." , L"...X" , L"...." , L"...." }   // Rotation_270
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
        { L"..X." , L".XX." , L".X.." , L"...." },  // Rotation_990
        { L"XX.." , L".XX." , L"...." , L"...." },  // Rotation_180
        { L"..X." , L".XX." , L".X.." , L"...." }   // Rotation_270
    }}
}};

}