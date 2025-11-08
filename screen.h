/**
 * @file screen.h
 * @brief 定义 Screen 类, 用于 Windows 控制台的绘图
 * @details 提供了 Screen 类中声明的成员函数的具体实现
 * @author LuoShu
 * @version 1.0
 * @date 2025-11-08
 */
#pragma once

#include <windows.h>
#include <string_view>
#include <array>

/**
 * @brief 包含屏幕(控制台)相关的常量.
 */
namespace screen {
    inline constexpr int WIDTH { 80 };    ///< 控制台屏幕宽度 (列)
    inline constexpr int HEIGHT { 30 };   ///< 控制台屏幕高度 (行)
    inline constexpr std::wstring_view TITLE { L"Tetris by LuoShu" };
    /**
     * @brief 封装 Windows 控制台颜色属性 (前景 FG, 背景 BG).
     */
    namespace Color {
        inline constexpr short FG_BLACK         { 0x0000 }; ///< 前景: 黑
        inline constexpr short FG_BLUE          { 0x0001 }; ///< 前景: 蓝
        inline constexpr short FG_GREEN         { 0x0002 }; ///< 前景: 绿
        inline constexpr short FG_CYAN          { 0x0003 }; ///< 前景: 青
        inline constexpr short FG_RED           { 0x0004 }; ///< 前景: 红
        inline constexpr short FG_MAGENTA       { 0x0005 }; ///< 前景: 品红
        inline constexpr short FG_YELLOW        { 0x0006 }; ///< 前景: 黄
        inline constexpr short FG_WHITE         { 0x0007 }; ///< 前景: 白
        inline constexpr short FG_INTENSITY     { 0x0008 }; ///< 前景: 亮色
        inline constexpr short BG_BLACK         { 0x0000 }; ///< 背景: 黑
        inline constexpr short BG_BLUE          { 0x0010 }; ///< 背景: 蓝
        inline constexpr short BG_GREEN         { 0x0020 }; ///< 背景: 绿
        inline constexpr short BG_CYAN          { 0x0030 }; ///< 背景: 青
        inline constexpr short BG_RED           { 0x0040 }; ///< 背景: 红
        inline constexpr short BG_MAGENTA       { 0x0050 }; ///< 背景: 品红
        inline constexpr short BG_YELLOW        { 0x0060 }; ///< 背景: 黄
        inline constexpr short BG_WHITE         { 0x0070 }; ///< 背景: 白
        inline constexpr short BG_INTENSITY     { 0x0080 }; ///< 背景: 亮背景
    }
};

/**
 * @brief 封装 Windows 控制台屏幕缓冲区.
 * @details
 * 使用 Windows API 创建一个新的缓冲区, 并在内存中维护一个后台缓冲区
 * 绘图操作写入缓冲区, 再将缓冲区刷新到活动屏幕, 从而实现高性能、无闪烁的绘图
 */
class Screen {
public:
    /**
     * @brief 构造函数: 创建并激活一个新的控制台屏幕缓冲区
     * @throws 如果创建缓冲区或激活失败
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
     * @brief 清空后台缓冲区 (填充为空格, 默认颜色)
     * @param[in] color (可选) 清屏时使用的背景色
     */
    void clearBuffer(short color = screen::Color::BG_BLACK);

    /**
     * @brief 在后台缓冲区绘制单个字符 (带颜色)
     * @param[in] x x 坐标
     * @param[in] y y 坐标
     * @param[in] ch 要绘制的宽字符
     * @param[in] color Windows 颜色属性 (例如: screen::Color::FG_RED)
     */
    void draw(int x, int y, wchar_t ch, short color);

    /**
     * @brief 在后台缓冲区绘制字符串 (带颜色)
     * @param[in] x 起始 x 坐标
     * @param[in] y y 坐标
     * @param[in] sv 要绘制的宽字符串视图
     * @param[in] color Windows 颜色属性
     */
    void draw(int x, int y, std::wstring_view sv, short color);

    /**
     * @brief 将后台缓冲区 (CHAR_INFO) 完整刷新到控制台屏幕
     * @details 使用 WriteConsoleOutputW 实现高性能、无闪烁的彩色绘制
     */
    void refresh();

private:
    /**
     * @brief 初始化控制台 (隐藏光标)
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
     * @brief 缓冲区 (存储字符和颜色)
     */
    std::array<CHAR_INFO, screen::WIDTH * screen::HEIGHT> m_buffer;
};