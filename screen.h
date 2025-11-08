#pragma once

#include <windows.h>
#include <string_view>
#include <array>

namespace screen {
    inline constexpr int WIDTH { 80 };    // 控制台屏幕宽度 (列)
    inline constexpr int HEIGHT { 30 };   // 控制台屏幕高度 (行)

    // C++17 风格的颜色常量, 封装了 Windows 宏
    // 这使得代码更清晰, 避免宏污染
    namespace Color {
        inline constexpr short FG_BLACK         { 0x0000 };
        inline constexpr short FG_BLUE          { 0x0001 };
        inline constexpr short FG_GREEN         { 0x0002 };
        inline constexpr short FG_CYAN          { 0x0003 };
        inline constexpr short FG_RED           { 0x0004 };
        inline constexpr short FG_MAGENTA       { 0x0005 };
        inline constexpr short FG_YELLOW        { 0x0006 };
        inline constexpr short FG_WHITE         { 0x0007 };
        inline constexpr short FG_INTENSITY     { 0x0008 }; // 亮色
        inline constexpr short BG_BLACK         { 0x0000 };
        inline constexpr short BG_BLUE          { 0x0010 };
        inline constexpr short BG_GREEN         { 0x0020 };
        inline constexpr short BG_CYAN          { 0x0030 };
        inline constexpr short BG_RED           { 0x0040 };
        inline constexpr short BG_MAGENTA       { 0x0050 };
        inline constexpr short BG_YELLOW        { 0x0060 };
        inline constexpr short BG_WHITE         { 0x0070 };
        inline constexpr short BG_INTENSITY     { 0x0080 }; // 亮背景
    }
};

class Screen {
public:
    /**
     * @brief 构造函数: 创建并激活一个新的控制台屏幕缓冲区
     * @throws std::runtime_error 如果创建缓冲区或激活失败
     */
    Screen();

    /**
     * @brief 析构函数: 关闭创建的控制台句柄
     * @note 必须调用 CloseHandle, 因为这个句柄是 CreateConsoleScreenBuffer 创建的
     */
    ~Screen();

    // 禁用拷贝和赋值
    Screen(const Screen&) = delete;
    Screen& operator=(const Screen&) = delete;

    /**
     * @brief 清空后台缓冲区 (填充为空格, 默认颜色)
     * @param color (可选) 清屏时使用的背景色
     */
    void clearBuffer(short color = screen::Color::BG_BLACK);

    /**
     * @brief 在后台缓冲区绘制单个字符 (带颜色)
     * @param x x 坐标
     * @param y y 坐标
     * @param ch 要绘制的宽字符
     * @param color Windows 颜色属性 (例如: screen::Color::FG_RED)
     */
    void draw(int x, int y, wchar_t ch, short color);

    /**
     * @brief 在后台缓冲区绘制字符串 (带颜色)
     * @param x 起始 x 坐标
     * @param y y 坐标
     * @param sv 要绘制的宽字符串视图
     * @param color Windows 颜色属性
     */
    void draw(int x, int y, std::wstring_view sv, short color);

    /**
     * @brief 将后台缓冲区 (CHAR_INFO) 完整刷新到控制台屏幕
     * @details 使用 WriteConsoleOutputW 实现高性能、无闪烁的彩色绘制
     */
    void refresh();

private:
    /**
     * @brief 初始化控制台 (隐藏光标, 禁用 QuickEdit)
     */
    void initConsole();

    /**
     * @brief 检查坐标是否在缓冲区内
     */
    bool isInConsole(int x, int y) const; // 标记为 const

    HANDLE m_hConsole; // 句柄 (由 CreateConsoleScreenBuffer 创建)

    /**
     * @brief 后台缓冲区
     * @details 现在存储 CHAR_INFO, 它包含字符和颜色属性
     */
    std::array<CHAR_INFO, screen::WIDTH * screen::HEIGHT> m_buffer;
};