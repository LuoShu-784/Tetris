#include "screen.h"
#include <stdexcept>

Screen::Screen()
    : m_hConsole(INVALID_HANDLE_VALUE) // 推荐初始化为无效值
{
    // 1. 创建一个新的屏幕缓冲区 (而不是获取标准句柄)
    m_hConsole = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE,   // 读写权限
        0,                              // 不共享
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL
    );

    if (m_hConsole == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Failed to create console screen buffer.");
    }

    // 2. 将这个新缓冲区设为活动屏幕
    if (!SetConsoleActiveScreenBuffer(m_hConsole)) {
        throw std::runtime_error("Failed to set active console screen buffer.");
    }
    
    // 3. 设置窗口标题 (这仍然有效)
    SetConsoleTitleW(L"Tetris C++17 (Optimized)");
    
    // 4. 初始化控制台 (隐藏光标等)
    initConsole();

    // 5. 清空缓冲区
    clearBuffer();
}

Screen::~Screen()
{
    if (m_hConsole != INVALID_HANDLE_VALUE) {
        CloseHandle(m_hConsole);
    }
}

void Screen::clearBuffer(short color)
{
    // C++17: 结构化绑定 (这里不需要, 但知道)
    // 我们用一个默认的 CHAR_INFO 填充
    CHAR_INFO defaultChar;
    defaultChar.Char.UnicodeChar = L' ';
    defaultChar.Attributes = color; // 默认属性 (例如: 黑底)

    m_buffer.fill(defaultChar); 
}

void Screen::draw(int x, int y, wchar_t ch, short color)
{
    if (isInConsole(x, y)) 
    {
        m_buffer[y * screen::WIDTH + x].Char.UnicodeChar = ch;
        m_buffer[y * screen::WIDTH + x].Attributes = color;
    }
}

void Screen::draw(int x, int y, std::wstring_view sv, short color)
{
    for (size_t i = 0; i < sv.size(); ++i)
    {
        int px = x + static_cast<int>(i);
        if (isInConsole(px, y)) {
            m_buffer[y * screen::WIDTH + px].Char.UnicodeChar = sv[i];
            m_buffer[y * screen::WIDTH + px].Attributes = color;
        }
    }
}


void Screen::refresh()
{
    
    COORD bufferSize = { screen::WIDTH, screen::HEIGHT };
    
    COORD bufferCoord = { 0, 0 };
    
    SMALL_RECT writeRegion = { 0, 0, screen::WIDTH - 1, screen::HEIGHT - 1 };

    WriteConsoleOutputW(
        m_hConsole,         // 目标句柄
        m_buffer.data(),    // 源数据 (CHAR_INFO 数组)
        bufferSize,         // 源数据大小
        bufferCoord,        // 源数据起始坐标
        &writeRegion        // 目标写入区域
    );
}

void Screen::initConsole()
{
    // 隐藏光标
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(m_hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE; // 隐藏
    SetConsoleCursorInfo(m_hConsole, &cursorInfo);
}

bool Screen::isInConsole(int x, int y) const // 标记为 const
{
    return x >= 0 && x < screen::WIDTH && y >= 0 && y < screen::HEIGHT;
}