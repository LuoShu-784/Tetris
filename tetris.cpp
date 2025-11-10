/**
 * @file tetris.cpp
 * @brief Tetris 类的实现
 * @details 提供了 Tetris 类中声明的成员函数的具体实现
 * @author LuoShu
 * @version 1.0
 * @date 2025-11-09
 */
#include "tetris.h"
#include <windows.h>
#include <string>
#include <algorithm>
#include <thread>

/**
 * @brief 构造函数
 */
Tetris::Tetris()
    : m_screen(),
      m_gameArea(),
      m_rng(std::random_device{}()),
      m_blockDist(static_cast<int>(Block::Type::I), static_cast<int>(Block::Type::Z)),
      m_currentBlock(
        getRandomBlockType(), 
        tetris::BLOCK_GENERATE_X, 
        tetris::BLOCK_GENERATE_Y,
        Block::RotationState::Rotation_0
      ),
      m_nextBlock(getRandomBlockType(), 0, 0, Block::RotationState::Rotation_0),
      m_lastFallTime(clock::now()) {};

/**
 * @brief 游戏主循环
 */
void Tetris::run() 
{
    using namespace screen;
    using milliseconds = std::chrono::milliseconds;
    
    while (!m_isGameOver)
    {
        processKey();

        auto now { clock::now()};
        auto elapsed { std::chrono::duration_cast<milliseconds>(now - m_lastFallTime) };

        if (elapsed >= getFallSpeed()) {
            update();
        }

        render();

        std::this_thread::sleep_for(milliseconds(16)); // 模拟约 60 FPS
    }

    // --- 游戏结束逻辑 ---
    std::wstring_view text { L"GAME OVER" };

    m_screen.clearBuffer(color::BLACK);
    m_screen.draw(
        WIDTH / 2 - text.length() / 2, 
        HEIGHT / 2, 
        text, 
        color::RED,
        color::BLACK
    );

    text = L"Press ESC to exit";
    m_screen.draw(
        WIDTH / 2 - text.length() / 2, 
        HEIGHT / 2 + 2, 
        text, 
        color::RED,
        color::BLACK
    );

    m_screen.refresh();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 等待ESC键退出
    while (!isKeyPressed(VK_ESCAPE))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
};

/**
 * @brief 处理按键输入实现
 */
void Tetris::processKey()
{
    auto checkKey = [&](int vKey, auto action)
    {
        bool isPressed { isKeyPressed(vKey)};
        if (isPressed && !m_keyState[vKey]) { // 仅在按下的第一帧触发
            action();
        }
        m_keyState[vKey] = isPressed;
    };

    checkKey(VK_LEFT, [&]() { moveBlock(-1, 0); });
    checkKey(VK_RIGHT, [&]() { moveBlock(1, 0); });
    checkKey(VK_UP, [&]() { rotateBlock(); });
    checkKey(VK_DOWN, [&]() { moveBlock(0, 1); });
    checkKey(VK_SPACE, [&]() { hardDropBlock(); });
    checkKey(VK_ESCAPE, [&]() { m_isGameOver = true; });
}

/**
 * @brief 检查按键是否被按下
 */
bool Tetris::isKeyPressed(int vKey) const
{
    return (GetAsyncKeyState(vKey) & tetris::KEY_PRESSED_BIT);
}

/**
 * @brief 游戏逻辑更新
 */
void Tetris::update()
{
    if (m_isGameOver) {
        return;
    }

    Block tempBlock { m_currentBlock };
    tempBlock.move(0, 1);

    if (!m_gameArea.isCollision(tempBlock)) 
    {
        m_currentBlock.move(0, 1);
    } 
    else 
    {
        // 发生碰撞, 锁定方块
        m_gameArea.lockBlock(m_currentBlock);

        // 清除满行并计分
        int lines { m_gameArea.clearFullLines() };
        if (lines > 0) {
            updateScore(lines);
        }

        // 生成下一个方块
        generateBlock();
    }

    m_lastFallTime = clock::now(); // 重置下落计时器
}

/**
 * @brief 生成新方块
 */
void Tetris::generateBlock()
{ 
    m_currentBlock = Block(
                            m_nextBlock.getType(), 
                            tetris::BLOCK_GENERATE_X, 
                            tetris::BLOCK_GENERATE_Y,
                            Block::RotationState::Rotation_0
                    );
                    
    m_nextBlock = Block(
                        getRandomBlockType(), 
                        0, 
                        0,
                        Block::RotationState::Rotation_0
                );

    if (m_gameArea.isCollision(m_currentBlock)) {
        m_isGameOver = true;
    }
}

/**
 * @brief 移动方块
 */
void Tetris::moveBlock(int dx, int dy)
{
    if (m_isGameOver) {
        return;
    } 

    Block tempBlock { m_currentBlock };
    tempBlock.move(dx, dy);
    if (!m_gameArea.isCollision(tempBlock)) {
        m_currentBlock = tempBlock;
    }
}

/**
 * @brief 旋转方块
 */
void Tetris::rotateBlock()
{
    if (m_isGameOver) {
        return;
    }
    
    Block tempBlock { m_currentBlock };
    tempBlock.rotate();
    if (!m_gameArea.isCollision(tempBlock)) {
        m_currentBlock = tempBlock;
    }
}

/**
 * @brief "硬"下落
 */
void Tetris::hardDropBlock()
{
    if (m_isGameOver) {
        return;
    }

    Block tempBlock { m_currentBlock };
    while (true)
    {
        tempBlock.move(0, 1);
        if (m_gameArea.isCollision(tempBlock)) 
        {
            tempBlock.move(0, -1); // 回退一步 (到碰撞前的最后位置)
            m_currentBlock = tempBlock;
            break;
        }
    }

    // 锁定, 清行, 生成
    m_gameArea.lockBlock(m_currentBlock);

    int lines { m_gameArea.clearFullLines() };
    if (lines > 0) {
        updateScore(lines);
    }

    generateBlock();
}

/**
 * @brief 更新分数和等级
 */
void Tetris::updateScore(int linesCleared)
{
    int score { 0 };
    switch(linesCleared)
    {
        case 1: score = 100 * m_level;break;
        case 2: score = 300 * m_level;break;
        case 3: score = 500 * m_level;break;
        case 4: score = 800 * m_level;break; 
        default:break;
    }
    m_score += score;

    m_linesCleared += linesCleared;
    if (m_linesCleared >= m_level * 10) { // 每10行升一级
        m_level++;
    }
}

/**
 * @brief 获取随机方块类型
 */
Block::Type Tetris::getRandomBlockType()
{
    return static_cast<Block::Type>(m_blockDist(m_rng));
}

/**
 * @brief 获取下落速度
 */
std::chrono::milliseconds Tetris::getFallSpeed() const
{
    int speedMs { 1000 - (m_level * 50) };
    return std::chrono::milliseconds(std::max(100, speedMs));
}

/**
 * @brief 获取方块颜色实现
 */
COLORREF Tetris::getBlockColor(Block::Type blockType) const
{
    using namespace screen::color;
    using Type = Block::Type;
    
    switch (blockType) 
    {
        case Type::I: return CYAN;
        case Type::J: return BLUE;
        case Type::L: return ORANGE;
        case Type::O: return YELLOW;
        case Type::S: return GREEN;
        case Type::T: return PINK;
        case Type::Z: return RED;
        default:return WHITE;
    }
}

/**
 * @brief 渲染主函数
 */
void Tetris::render()
{
    m_screen.clearBuffer();
    
    drawGameArea();
    drawUI();
    
    m_screen.refresh();
}

/**
 * @brief 绘制单个方块
 */
void Tetris::drawBlock(const Block& block, int xOffset, int yOffset)
{
    COLORREF color { getBlockColor(block.getType()) };

    for (int y = 0; y < block::HEIGHT; y++)
    {
        for (int x = 0; x < block::WIDTH; x++)
        {
            if (block.getValue(x, y)) 
            {
                int screenX { xOffset + (block.getX() + x) * 2 };
                int screenY { yOffset + block.getY()  + y } ;

                m_screen.draw(screenX, screenY, L'█', color, screen::color::BLACK);
                m_screen.draw(screenX + 1, screenY, L'█', color, screen::color::BLACK);
            }
        }
    }
}
 
/**
 * @brief 绘制游戏区域
 */
void Tetris::drawGameArea()
{
    using namespace tetris;
    using namespace screen::color; // 假设存在 screen::color
    
    // 绘制边框 (底部)
    for (int x = 0; x < gamearea::WIDTH * 2 + 2; x++ )
    {
          m_screen.draw(
               AREA_OFFSET_X + x, 
            AREA_OFFSET_Y + 
            gamearea::HEIGHT + 1, 
            L'▀', 
            WHITE,
            BLACK
         );
     } 

    // 绘制边框 (左右)
    for (int y = 0; y < gamearea::HEIGHT + 1; ++y) {
        m_screen.draw(
            AREA_OFFSET_X, 
            AREA_OFFSET_Y + y, 
               L'█', 
            WHITE,
            BLACK
        );
         m_screen.draw(
            AREA_OFFSET_X + gamearea::WIDTH * 2 + 1, 
            AREA_OFFSET_Y + y, 
              L'█', 
            WHITE,
            BLACK
         );
    }

    // 绘制 m_gameArea 中已锁定的方块
    for (int y = 0; y < gamearea::HEIGHT; y++) 
     {
        for (int x = 0; x < gamearea::WIDTH; x++) 
        {
            int value { m_gameArea.getValue(x, y) };
            if (value) 
            {
                int screenX { AREA_OFFSET_X + 1 + (x * 2) };
                int screenY { AREA_OFFSET_Y + 1 + y };
                
                COLORREF color { getBlockColor(static_cast<Block::Type>(value)) };
                
                m_screen.draw(screenX, screenY, L'█', color, BLACK) ;
                m_screen.draw(screenX + 1, screenY, L'█', color, BLACK);
            }
        }
    }

    // 绘制当前正在下落的方块 (在最顶层)
    drawBlock(m_currentBlock, AREA_OFFSET_X + 1, AREA_OFFSET_Y + 1);
}

/**
 * @brief 绘制 UI 界面实现
 */
void Tetris::drawUI()
{

    using namespace tetris;
    using namespace screen::color; // 假设存在 screen::color

    m_screen.draw(UI_OFFSET_X, UI_OFFSET_Y, L"SCORE", RED, BLACK);
    m_screen.draw(UI_OFFSET_X, UI_OFFSET_Y + 1, std::to_wstring(m_score), WHITE, BLACK);

    m_screen.draw(UI_OFFSET_X, UI_OFFSET_Y + 3, L"LEVEL", GREEN, BLACK);
    m_screen.draw(UI_OFFSET_X, UI_OFFSET_Y + 4, std::to_wstring(m_level), WHITE, BLACK);

    m_screen.draw(UI_OFFSET_X, UI_OFFSET_Y + 6, L"LINES", BLUE, BLACK);
    m_screen.draw(UI_OFFSET_X, UI_OFFSET_Y + 7, std::to_wstring(m_linesCleared), WHITE, BLACK);

    m_screen.draw(UI_OFFSET_X, UI_OFFSET_Y + 9, L"NEXT", YELLOW, BLACK);
    
    // 创建一个临时的 m_nextBlock 以便在 UI 区域绘制 (重置其坐标)
    Block nextPreview = Block(m_nextBlock.getType(), 0, 0, Block::RotationState::Rotation_0);
    drawBlock(nextPreview, UI_OFFSET_X, UI_OFFSET_Y + 10);

    // 绘制控制说明
    int controlsY = UI_OFFSET_Y + 16;
    m_screen.draw(UI_OFFSET_X, controlsY,     L"CONTROLS", WHITE, BLACK);
    m_screen.draw(UI_OFFSET_X, controlsY + 1, L"Left/Right: Move", YELLOW, BLACK);
    m_screen.draw(UI_OFFSET_X, controlsY + 2, L"Up:         Rotate", BLUE, BLACK);
    m_screen.draw(UI_OFFSET_X, controlsY + 3, L"Down:       Soft Drop", ORANGE, BLACK);
    m_screen.draw(UI_OFFSET_X, controlsY + 4, L"Space:      Hard Drop", GREEN, BLACK);
    m_screen.draw(UI_OFFSET_X, controlsY + 5, L"Esc:        Quit", RED, BLACK);
}