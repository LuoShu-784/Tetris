#include "block.h"

Block::Block(Type type, int x, int y, RotationState rotationState)
    : m_type(type), m_x(x), m_y(y), m_rotationState(rotationState) {}

void Block::move(int dx, int dy) 
{
    m_x += dx;
    m_y += dy;
}

void Block::rotate()
{
    int rotationState = static_cast<int>(m_rotationState);
    m_rotationState = static_cast<RotationState>((rotationState + 1) % block::ROTATION_STATES_NUM);
}

Block::Type Block::getType() const
{
    return m_type;
}

int Block::getValue(int x, int y) const
{

    if (x < 0 || x >= block::WIDTH || y < 0 || y >= block::HEIGHT) {
        return 0;
    }

    int typeIndex = static_cast<int>(m_type) - 1;
    int rotationStateIndex = static_cast<int>(m_rotationState);
    return block::Shapes[typeIndex][rotationStateIndex][y][x] == L'X' ? static_cast<int>(m_type) : 0;
}

int Block::getX() const
{
    return m_x;
}

int Block::getY() const
{
    return m_y;
}