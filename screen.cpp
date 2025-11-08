/**
 * @file screen.cpp
 * @brief Screen 类的实现
 * @details 实现了 Screen 中定义的 Windows 控制台双缓冲功能.
 * @author LuoShu
 * @version 1.0
 * @date 2025-11-08
 */
#include "screen.h"
#include <stdexcept>

/**
 * @brief 构造函数实现
 */
Screen::Screen()
    : m_hConsole(INVALID_HANDLE_VALUE) // 推荐初始化为无效值
{
    // 创建一个新的屏幕缓冲区 (而不是获取标准句柄)
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

    // 将这个新缓冲区设为活动屏幕
    if (!SetConsoleActiveScreenBuffer(m_hConsole)) {
        throw std::runtime_error("Failed to set active console screen buffer.");
    }
    
    // 设置窗口标题
    SetConsoleTitleW(screen::TITLE.data());
    
    // 初始化控制台
    initConsole();

    // 清空缓冲区
    clearBuffer();
}

/**
 * @brief 析构函数实现
 */
Screen::~Screen()
{
    if (m_hConsole != INVALID_HANDLE_VALUE) {
        CloseHandle(m_hConsole);
    }
}

/**
 * @brief 清空缓冲区.
 */
void Screen::clearBuffer(short color)
{
    CHAR_INFO defaultChar;
    defaultChar.Char.UnicodeChar = L' ';
    defaultChar.Attributes = color; // 默认属性 (例如: 黑底)

    m_buffer.fill(defaultChar); 
}

/**
 * @brief 在缓冲区绘制单个字符
 */
void Screen::draw(int x, int y, wchar_t ch, short color)
{
    if (isInConsole(x, y)) 
    {
        m_buffer[y * screen::WIDTH + x].Char.UnicodeChar = ch;
        m_buffer[y * screen::WIDTH + x].Attributes = color;
    }
}

/**
 * @brief 在缓冲区绘制字符串
 */
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


/**
 * @brief 刷新屏幕
 */
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

/**
 * @brief 初始化控制台设置
 */
void Screen::initConsole()
{
    // 隐藏光标
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(m_hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE; // 隐藏
    SetConsoleCursorInfo(m_hConsole, &cursorInfo);
}

/**
 * @brief 坐标边界检查
 */
bool Screen::isInConsole(int x, int y) const // 标记为 const
{
    return x >= 0 && x < screen::WIDTH && y >= 0 && y < screen::HEIGHT;
}