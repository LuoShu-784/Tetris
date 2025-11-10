/**
 * @file screen.cpp
 * @brief Screen 类的实现 (支持真彩色)
 * @details 实现了 Screen 中定义的 Windows 控制台功能
 * @author LuoShu
 * @version 2.2
 * @date 2025-11-10
 */
#include "screen.h"
#include <stdexcept>
#include <string>
#include <sstream>

namespace {
    /**
     * @brief VT (Virtual Terminal) 转义序列常量
     */

    // 重置所有属性
    constexpr std::wstring_view VT_RESET = L"\x1b[0m";
    // SGR 命令的后缀
    constexpr std::wstring_view VT_SGR_SUFFIX = L"m";
    
    // 设置前景色
    constexpr std::wstring_view VT_FG_TRUECOLOR = L"\x1b[38;2;";
    // 设置背景色
    constexpr std::wstring_view VT_BG_TRUECOLOR = L"\x1b[48;2;";


    // 禁用自动换行
    constexpr std::wstring_view VT_AUTOWRAP_OFF = L"\x1b[?7l"; 
    // 启用自动换行 (恢复默认)
    constexpr std::wstring_view VT_AUTOWRAP_ON = L"\x1b[?7h";

    // 显示光标
    constexpr std::wstring_view VT_CURSOR_SHOW = L"\x1b[?25h";
    // 隐藏光标
    constexpr std::wstring_view VT_CURSOR_HIDE = L"\x1b[?25l";
}


/**
 * @brief 构造函数
 */
Screen::Screen()
    : m_hConsole(INVALID_HANDLE_VALUE) // 初始化为无效值
{
    // 创建一个新的屏幕缓冲区
    m_hConsole = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL
    );

    if (m_hConsole == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Failed to create console screen buffer.");
    }

    // 将新缓冲区设为活动屏幕
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
 * @brief 析构函数
 */
Screen::~Screen()
{
    if (m_hConsole != INVALID_HANDLE_VALUE) {
        // 恢复默认颜色并显示光标
        std::wstring resetCmd;
        resetCmd.append(VT_RESET);
        resetCmd.append(VT_CURSOR_SHOW);
        resetCmd.append(VT_AUTOWRAP_ON);

        DWORD written = 0;
        WriteConsoleW(m_hConsole, resetCmd.c_str(), (DWORD)resetCmd.length(), &written, NULL);
        
        CloseHandle(m_hConsole);
    }
}

/**
 * @brief 清空缓冲区
 */
void Screen::clearBuffer(COLORREF rbgColor)
{
    Cell defaultCell;
    defaultCell.ch = L' ';
    defaultCell.fg = screen::color::GRAY;
    defaultCell.bg = rbgColor;

    m_buffer.fill(defaultCell); 
}

/**
 * @brief 在缓冲区绘制单个字符
 */
void Screen::draw(int x, int y, wchar_t ch, COLORREF fg, COLORREF bg)
{
    if (isInConsole(x, y)) 
    {
        Cell& cell = m_buffer[y * screen::WIDTH + x];
        cell.ch = ch;
        cell.fg = fg;
        cell.bg = bg;
    }
}

/**
 * @brief 在缓冲区绘制字符串
 */
void Screen::draw(int x, int y, std::wstring_view sv, COLORREF fg, COLORREF bg)
{
    int len = static_cast<int>(sv.length());
    for (int i = 0; i < len; i++)
    {
        int px = x + i;
        if (isInConsole(px, y)) 
        {
            Cell& cell = m_buffer[y * screen::WIDTH + px];
            cell.ch = sv[i];
            cell.fg = fg;
            cell.bg = bg;
        }
    }
}


/**
 * @brief 刷新屏幕
 */
void Screen::refresh()
{
    // 清空并重用字符串流
    m_frameBuilder.str(L"");
    m_frameBuilder.clear();

    COLORREF lastFg = -1; // 使用无效颜色强制第一次设置
    COLORREF lastBg = -1;

    for (int y = 0; y < screen::HEIGHT; ++y) 
    {
        // VT 序列坐标是 1-based, 所以 y=0 是第 1 行
        m_frameBuilder << L"\x1b[" << (y + 1) << L";1H";

        for (int x = 0; x < screen::WIDTH; ++x) 
        {
            const Cell& cell = m_buffer[y * screen::WIDTH + x];

            // 仅在颜色改变时才设置 VT 码 (高效)
            if (cell.fg != lastFg) 
            {
                m_frameBuilder  << VT_FG_TRUECOLOR 
                                << GetRValue(cell.fg) << L";"
                                << GetGValue(cell.fg) << L";" 
                                << GetBValue(cell.fg) 
                                << VT_SGR_SUFFIX;
                lastFg = cell.fg;
            }
            if (cell.bg != lastBg)
            {
                m_frameBuilder  << VT_BG_TRUECOLOR 
                                << GetRValue(cell.bg) << L";"
                                << GetGValue(cell.bg) << L";" 
                                << GetBValue(cell.bg) 
                                << VT_SGR_SUFFIX;
                lastBg = cell.bg;
            }

            // 写入字符
            m_frameBuilder << cell.ch;
        }
    }

    std::wstring frame = m_frameBuilder.str();
    DWORD bytesWritten = 0;
    
    // 将构建好的整个帧字符串一次性写入控制台
    WriteConsoleW(
        m_hConsole,
        frame.c_str(),
        static_cast<DWORD>(frame.length()),
        &bytesWritten,
        NULL
    );
}

/**
 * @brief 初始化控制台设置
 */
void Screen::initConsole()
{
    // 隐藏光标 (使用 VT 序列, 与析构函数保持一致)
    DWORD written = 0;
    WriteConsoleW(m_hConsole, VT_CURSOR_HIDE.data(), (DWORD)VT_CURSOR_HIDE.length(), &written, NULL);

    // 禁用自动换行, 防止在写入时触发滚动
    WriteConsoleW(m_hConsole, VT_AUTOWRAP_OFF.data(), (DWORD)VT_AUTOWRAP_OFF.length(), &written, NULL);

    // 启用虚拟终端 (VT) 处理
    DWORD dwMode = 0;
    if (!GetConsoleMode(m_hConsole, &dwMode)) {
        throw std::runtime_error("Failed to get console mode.");
    }

    dwMode |= ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(m_hConsole, dwMode)) {
        throw std::runtime_error("Failed to enable virtual terminal processing.");
    }
}

/**
 * @brief 坐标边界检查
 */
bool Screen::isInConsole(int x, int y) const
{
    return x >= 0 && x < screen::WIDTH && y >= 0 && y < screen::HEIGHT;
}