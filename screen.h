/**
 * @file screen.h
 * @brief 定义 Screen 类, 用于 Windows 控制台
 * @details 使用 VT 序列实现高效、无闪烁的真彩色双缓冲
 * @author LuoShu
 * @version 2.0
 * @date 2025-11-09
 */
#pragma once

#include <windows.h>
#include <string_view>
#include <array>
#include <string>       // 用于 std::wstring
#include <sstream>      // 用于 std::wstringstream

/**
 * @brief 包含屏幕(控制台)相关的常量
 */
namespace screen {
    inline constexpr int WIDTH { 80 };    ///< 控制台屏幕宽度 (列)
    inline constexpr int HEIGHT { 30 };   ///< 控制台屏幕高度 (行)
    inline constexpr std::wstring_view TITLE { L"Tetris by LuoShu" };

    /**
     * @brief 封装常用的 24-bit RGB 颜色常量
     * @details
     * 所有常量均为 COLORREF 类型, 可直接用于 Screen::draw()
     */
    namespace color {
        // --- 基础色 ---
        inline constexpr COLORREF BLACK       { RGB(0, 0, 0) };
        inline constexpr COLORREF WHITE       { RGB(255, 255, 255) };
        inline constexpr COLORREF GRAY        { RGB(128, 128, 128) };
        inline constexpr COLORREF LIGHT_GRAY  { RGB(192, 192, 192) };
        inline constexpr COLORREF DARK_GRAY   { RGB(64, 64, 64) };

        // --- 亮色 (饱和色) ---
        inline constexpr COLORREF RED         { RGB(255, 0, 0) };
        inline constexpr COLORREF GREEN       { RGB(0, 255, 0) };
        inline constexpr COLORREF BLUE        { RGB(0, 0, 255) };
        inline constexpr COLORREF YELLOW      { RGB(255, 255, 0) };
        inline constexpr COLORREF CYAN        { RGB(0, 255, 255) };
        inline constexpr COLORREF MAGENTA     { RGB(255, 0, 255) };

        // --- 暗色 (不饱和色) ---
        inline constexpr COLORREF DARK_RED    { RGB(128, 0, 0) };
        inline constexpr COLORREF DARK_GREEN  { RGB(0, 128, 0) };
        inline constexpr COLORREF DARK_BLUE   { RGB(0, 0, 128) };

        // --- 其他 ---
        inline constexpr COLORREF ORANGE      { RGB(255, 165, 0) };
        inline constexpr COLORREF PINK        { RGB(255, 192, 203) };
        inline constexpr COLORREF PURPLE      { RGB(128, 0, 128) };
    }
};

/**
 * @brief 封装 Windows 控制台屏幕缓冲区
 * @details 使用 Windows API 创建一个新的缓冲区, 并启用 VT 序列支持,
 * 绘图操作写入一个包含 24-bit 色的后台缓冲区
 * 将缓冲区渲染为 VT 序列字符串, 并一次性写入控制台, 实现高性能、无闪烁的绘图
 */
class Screen {
public:
    /**
     * @brief 构造函数: 创建并激活一个新的控制台屏幕缓冲区
     * @throws 如果创建缓冲区、激活或启用 VT 模式失败
     */
    Screen();

    /**
     * @brief 析构函数: 关闭创建的控制台句柄
     */
    ~Screen();

    // 禁用拷贝和赋值
    Screen(const Screen&) = delete;             ///< 禁用拷贝构造
    Screen& operator=(const Screen&) = delete;  ///< 禁用拷贝赋值

    /**
     * @brief 清空后台缓冲区
     * @param[in] color (可选) 清屏时使用的背景色 (24-bit RGB)
     */
    void clearBuffer(COLORREF rbgColor = screen::color::BLACK);

    /**
     * @brief 在后台缓冲区绘制单个字符
     * @param[in] x x 坐标
     * @param[in] y y 坐标
     * @param[in] ch 要绘制的宽字符
     * @param[in] fg 24-bit RGB 前景色 (使用 RGB(r,g,b) 宏)
     * @param[in] bg 24-bit RGB 背景色 (使用 RGB(r,g,b) 宏)
     */
    void draw(int x, int y, wchar_t ch, COLORREF fg, COLORREF bg);

    /**
     * @brief 在后台缓冲区绘制字符串
     * @param[in] x 起始 x 坐标
     * @param[in] y 起始 y 坐标
     * @param[in] sv 要绘制的宽字符串视图
     * @param[in] fg 24-bit RGB 前景色
     * @param[in] bg 24-bit RGB 背景色
     */
    void draw(int x, int y, std::wstring_view sv, COLORREF fg, COLORREF bg);

    /**
     * @brief 将后台缓冲区 (Cell) 渲染为 VT 序列并刷新到控制台屏幕
     */
    void refresh();

private:
    /**
     * @brief 存储单个控制台单元格的真彩色状态
     */
    struct Cell {
        wchar_t ch { L' ' };                    ///< 字符
        COLORREF fg { RGB(255, 255, 255) };     ///< 前景色 (True Color)
        COLORREF bg { RGB(0, 0, 0) };           ///< 背景色 (True Color)
    };
    
    /**
     * @brief 初始化控制台
     */
    void initConsole();

    /**
     * @brief 检查坐标是否在缓冲区内
     * @param[in] x x 坐标
     * @param[in] y y 坐标
     * @return bool 如果坐标有效, 返回 true.
     */
    bool isInConsole(int x, int y) const;


    /**
     * @brief 控制台屏幕缓冲区句柄
     */
    HANDLE m_hConsole;

    /**
     * @brief 屏幕后台缓冲区
     */
    std::array<Cell, screen::WIDTH * screen::HEIGHT> m_buffer;

    /**
     * @brief 用于构建 VT 序列的字符串流
     */
    std::wstringstream m_frameBuilder;
};