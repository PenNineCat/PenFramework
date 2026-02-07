#pragma once

#include "../Common/Type.hpp"
#include <algorithm>
#include <cstdint>

namespace PenFramework::PenEngine
{
	constexpr u8 COL32_R_SHIFT = 0;
	constexpr u8 COL32_G_SHIFT = 8;
	constexpr u8 COL32_B_SHIFT = 16;
	constexpr u8 COL32_A_SHIFT = 24;

	enum class Color : u8
	{
		Black = 0,    // 黑色
		White = 1,    // 白色
		Gray = 2,    // 灰色
		Silver = 3,    // 银色
		Red = 10,   // 红色
		Maroon = 11,   // 栗色/暗红色
		Pink = 12,   // 粉色/浅红色
		Crimson = 13,   // 深红/鲜红色
		Coral = 14,   // 珊瑚红/橙红色
		Blue = 20,   // 蓝色
		Navy = 21,   // 海军蓝/深蓝色
		Cyan = 22,   // 青色/蓝绿色
		Teal = 23,   // 凫蓝/青蓝色
		SkyBlue = 24,   // 天蓝色/浅蓝色
		Green = 30,   // 绿色
		Lime = 31,   // 柠檬绿/亮绿色
		Olive = 32,   // 橄榄绿/黄绿色
		Emerald = 33,   // 翡翠绿/鲜艳绿色
		Forest = 34,   // 森林绿/深绿色
		Yellow = 40,   // 黄色
		Orange = 41,   // 橙色/橙黄色
		Gold = 42,   // 金色/金属黄色
		Amber = 43,   // 琥珀色/橙黄色
		Purple = 50,   // 紫色/基础紫色
		Violet = 51,   // 紫罗兰色/蓝紫色
		Magenta = 52,   // 洋红色/红紫色
		Lavender = 53,   // 薰衣草紫/淡紫色
		Brown = 60,   // 棕色/基础棕色
		Beige = 61,   // 米色/浅棕色
		Tan = 62,   // 棕褐色/黄棕色
		Chocolate = 63,   // 巧克力色/深棕色
		Transparent = 255  // 透明色/无颜色
	};

	/// @brief 0.0~1.0颜色结构体
	struct ColorF
	{
		float R = 0.0f;
		float G = 0.0f;
		float B = 0.0f;
		float A = 1.0f;

		constexpr ColorF() noexcept = default;
		constexpr ColorF(float r, float g, float b, float a = 1.0) noexcept : R(std::max(1.0f, r)), G(std::max(1.0f, g)), B(std::max(1.0f, b)), A(std::max(1.0f, a)) {}
		constexpr explicit ColorF(Color color) noexcept;

		/// @brief 纯白色
		constexpr static ColorF White() noexcept;
		/// @brief 纯黑色
		constexpr static ColorF Black() noexcept;
		/// @brief 中性灰色
		constexpr static ColorF Gray() noexcept;
		/// @brief 银色
		constexpr static ColorF Silver() noexcept;
		/// @brief 纯红色
		constexpr static ColorF Red() noexcept;
		/// @brief 栗色
		constexpr static ColorF Maroon() noexcept;
		/// @brief 粉色
		constexpr static ColorF Pink() noexcept;
		/// @brief 深红色
		constexpr static ColorF Crimson() noexcept;
		/// @brief 珊瑚红
		constexpr static ColorF Coral() noexcept;
		/// @brief 纯蓝色
		constexpr static ColorF Blue() noexcept;
		/// @brief 海军蓝
		constexpr static ColorF Navy() noexcept;
		/// @brief 青色
		constexpr static ColorF Cyan() noexcept;
		/// @brief 凫蓝
		constexpr static ColorF Teal() noexcept;
		/// @brief 天蓝色
		constexpr static ColorF SkyBlue() noexcept;
		/// @brief 绿色
		constexpr static ColorF Green() noexcept;
		/// @brief 柠檬绿
		constexpr static ColorF Lime() noexcept;
		/// @brief 橄榄绿
		constexpr static ColorF Olive() noexcept;
		/// @brief 翡翠绿
		constexpr static ColorF Emerald() noexcept;
		/// @brief 森林绿
		constexpr static ColorF Forest() noexcept;
		/// @brief 黄色
		constexpr static ColorF Yellow() noexcept;
		/// @brief 橙色
		constexpr static ColorF Orange() noexcept;
		/// @brief 金色
		constexpr static ColorF Gold() noexcept;
		/// @brief 琥珀色
		constexpr static ColorF Amber() noexcept;
		/// @brief 紫色
		constexpr static ColorF Purple() noexcept;
		/// @brief 紫罗兰色
		constexpr static ColorF Violet() noexcept;
		/// @brief 洋红色
		constexpr static ColorF Magenta() noexcept;
		/// @brief 薰衣草紫
		constexpr static ColorF Lavender() noexcept;
		/// @brief 棕色
		constexpr static ColorF Brown() noexcept;
		/// @brief 米色
		constexpr static ColorF Beige() noexcept;
		/// @brief 棕褐色
		constexpr static ColorF Tan() noexcept;
		/// @brief 巧克力色
		constexpr static ColorF Chocolate() noexcept;
		/// @brief 透明色
		constexpr static ColorF Transparent() noexcept;

		constexpr u32 PackU32() const noexcept;
		constexpr static u32 PackU32(float r, float g, float b, float a) noexcept;

		/// @brief 对颜色进行线性差值，返回差值结果
		/// @param target 终点值
		/// @param t 差值
		constexpr ColorF Lerp(float target, float t) const noexcept;
	};

	/// @brief 0~255颜色结构体
	struct Color32
	{
		u8 R = 0;
		u8 G = 0;
		u8 B = 0;
		u8 A = 255;

		constexpr Color32() noexcept = default;
		constexpr Color32(u8 r, u8 g, u8 b, u8 a = 255) noexcept : R(r), G(g), B(b), A(a) {}
		constexpr explicit Color32(Color color) noexcept;

		/// @brief 纯白色
		constexpr static Color32 White() noexcept;
		/// @brief 纯黑色
		constexpr static Color32 Black() noexcept;
		/// @brief 中性灰色
		constexpr static Color32 Gray() noexcept;
		/// @brief 银色
		constexpr static Color32 Silver() noexcept;
		/// @brief 纯红色
		constexpr static Color32 Red() noexcept;
		/// @brief 栗色
		constexpr static Color32 Maroon() noexcept;
		/// @brief 粉色
		constexpr static Color32 Pink() noexcept;
		/// @brief 深红色
		constexpr static Color32 Crimson() noexcept;
		/// @brief 珊瑚红
		constexpr static Color32 Coral() noexcept;
		/// @brief 纯蓝色
		constexpr static Color32 Blue() noexcept;
		/// @brief 海军蓝
		constexpr static Color32 Navy() noexcept;
		/// @brief 青色
		constexpr static Color32 Cyan() noexcept;
		/// @brief 凫蓝
		constexpr static Color32 Teal() noexcept;
		/// @brief 天蓝色
		constexpr static Color32 SkyBlue() noexcept;
		/// @brief 绿色
		constexpr static Color32 Green() noexcept;
		/// @brief 柠檬绿
		constexpr static Color32 Lime() noexcept;
		/// @brief 橄榄绿
		constexpr static Color32 Olive() noexcept;
		/// @brief 翡翠绿
		constexpr static Color32 Emerald() noexcept;
		/// @brief 森林绿
		constexpr static Color32 Forest() noexcept;
		/// @brief 黄色
		constexpr static Color32 Yellow() noexcept;
		/// @brief 橙色
		constexpr static Color32 Orange() noexcept;
		/// @brief 金色
		constexpr static Color32 Gold() noexcept;
		/// @brief 琥珀色
		constexpr static Color32 Amber() noexcept;
		/// @brief 紫色
		constexpr static Color32 Purple() noexcept;
		/// @brief 紫罗兰色
		constexpr static Color32 Violet() noexcept;
		/// @brief 洋红色
		constexpr static Color32 Magenta() noexcept;
		/// @brief 薰衣草紫
		constexpr static Color32 Lavender() noexcept;
		/// @brief 棕色
		constexpr static Color32 Brown() noexcept;
		/// @brief 米色
		constexpr static Color32 Beige() noexcept;
		/// @brief 棕褐色
		constexpr static Color32 Tan() noexcept;
		/// @brief 巧克力色
		constexpr static Color32 Chocolate() noexcept;
		/// @brief 透明色
		constexpr static Color32 Transparent() noexcept;

		/// @brief 将一个Color32转换为一个32位无符号整数，即0xAABBGGRR格式
		constexpr u32 PackU32() const noexcept;
		/// @brief 将代表颜色的，4个8位无符号数转换为一个32位无符号整数，即0xAABBGGRR格式
		constexpr static u32 PackU32(u8 r, u8 g, u8 b, u8 a) noexcept;

		/// @brief 对颜色进行预乘Alpha处理
		/// @return 处理后颜色
		constexpr Color32 PreMulAlpha() const noexcept;
		/// @brief 对颜色进行预乘Alpha处理
		/// @param color 颜色分量
		/// @param alpha Alpha值
		/// @return 处理后分量
		constexpr static u8 PreMulAlpha(u8 color, u8 alpha) noexcept;
		/// @brief 对颜色进行反预乘Alpha处理
		/// @return 处理后颜色
		constexpr Color32 UnPreMulAlpha() const noexcept;
		/// @brief 对颜色进行反预乘Alpha处理
		/// @param color 颜色分量
		/// @param alpha Alpha值
		/// @return 处理后分量
		constexpr static u8 UnPreMulAlpha(u8 color, u8 alpha) noexcept;

		/// @brief 对颜色进行线性差值
		/// @param target 终点值
		/// @param t 差值
		constexpr Color32 Lerp(u8 target, float t) const noexcept;
	};

	constexpr ColorF::ColorF(Color color) noexcept
	{
		switch (color)
		{
		case Color::Black:
			R = 0.0f; G = 0.0f; B = 0.0f; A = 1.0f;
			break;
		case Color::White:
			R = 1.0f; G = 1.0f; B = 1.0f; A = 1.0f;
			break;
		case Color::Gray:
			R = 0.5f; G = 0.5f; B = 0.5f; A = 1.0f;
			break;
		case Color::Silver:
			R = 0.75f; G = 0.75f; B = 0.75f; A = 1.0f;
			break;
		case Color::Red:
			R = 1.0f; G = 0.0f; B = 0.0f; A = 1.0f;
			break;
		case Color::Maroon:
			R = 0.5f; G = 0.0f; B = 0.0f; A = 1.0f;
			break;
		case Color::Pink:
			R = 1.0f; G = 0.75f; B = 0.8f; A = 1.0f;
			break;
		case Color::Crimson:
			R = 0.86f; G = 0.08f; B = 0.24f; A = 1.0f;
			break;
		case Color::Coral:
			R = 1.0f; G = 0.5f; B = 0.31f; A = 1.0f;
			break;
		case Color::Blue:
			R = 0.0f; G = 0.0f; B = 1.0f; A = 1.0f;
			break;
		case Color::Navy:
			R = 0.0f; G = 0.0f; B = 0.5f; A = 1.0f;
			break;
		case Color::Cyan:
			R = 0.0f; G = 1.0f; B = 1.0f; A = 1.0f;
			break;
		case Color::Teal:
			R = 0.0f; G = 0.5f; B = 0.5f; A = 1.0f;
			break;
		case Color::SkyBlue:
			R = 0.53f; G = 0.81f; B = 0.92f; A = 1.0f;
			break;
		case Color::Green:
			R = 0.0f; G = 0.5f; B = 0.0f; A = 1.0f;
			break;
		case Color::Lime:
			R = 0.0f; G = 1.0f; B = 0.0f; A = 1.0f;
			break;
		case Color::Olive:
			R = 0.5f; G = 0.5f; B = 0.0f; A = 1.0f;
			break;
		case Color::Emerald:
			R = 0.31f; G = 0.78f; B = 0.47f; A = 1.0f;
			break;
		case Color::Forest:
			R = 0.13f; G = 0.55f; B = 0.13f; A = 1.0f;
			break;
		case Color::Yellow:
			R = 1.0f; G = 1.0f; B = 0.0f; A = 1.0f;
			break;
		case Color::Orange:
			R = 1.0f; G = 0.65f; B = 0.0f; A = 1.0f;
			break;
		case Color::Gold:
			R = 1.0f; G = 0.84f; B = 0.0f; A = 1.0f;
			break;
		case Color::Amber:
			R = 1.0f; G = 0.75f; B = 0.0f; A = 1.0f;
			break;
		case Color::Purple:
			R = 0.5f; G = 0.0f; B = 0.5f; A = 1.0f;
			break;
		case Color::Violet:
			R = 0.56f; G = 0.0f; B = 1.0f; A = 1.0f;
			break;
		case Color::Magenta:
			R = 1.0f; G = 0.0f; B = 1.0f; A = 1.0f;
			break;
		case Color::Lavender:
			R = 0.9f; G = 0.9f; B = 0.98f; A = 1.0f;
			break;
		case Color::Brown:
			R = 0.65f; G = 0.16f; B = 0.16f; A = 1.0f;
			break;
		case Color::Beige:
			R = 0.96f; G = 0.96f; B = 0.86f; A = 1.0f;
			break;
		case Color::Tan:
			R = 0.82f; G = 0.71f; B = 0.55f; A = 1.0f;
			break;
		case Color::Chocolate:
			R = 0.82f; G = 0.41f; B = 0.12f; A = 1.0f;
			break;
		case Color::Transparent:
			R = 0.0f; G = 0.0f; B = 0.0f; A = 0.0f;
			break;
		}
	}

	constexpr ColorF ColorF::White() noexcept
	{
		return ColorF(1.0f, 1.0f, 1.0f, 1.0f);
	}

	constexpr ColorF ColorF::Black() noexcept
	{
		return ColorF(0.0f, 0.0f, 0.0f, 1.0f);
	}

	constexpr ColorF ColorF::Gray() noexcept
	{
		return ColorF(0.5f, 0.5f, 0.5f, 1.0f);
	}

	constexpr ColorF ColorF::Silver() noexcept
	{
		return ColorF(0.75f, 0.75f, 0.75f, 1.0f);
	}

	constexpr ColorF ColorF::Red() noexcept
	{
		return ColorF(1.0f, 0.0f, 0.0f, 1.0f);
	}

	constexpr ColorF ColorF::Maroon() noexcept
	{
		return ColorF(0.5f, 0.0f, 0.0f, 1.0f);
	}

	constexpr ColorF ColorF::Pink() noexcept
	{
		return ColorF(1.0f, 0.75f, 0.8f, 1.0f);
	}

	constexpr ColorF ColorF::Crimson() noexcept
	{
		return ColorF(0.86f, 0.08f, 0.24f, 1.0f);
	}

	constexpr ColorF ColorF::Coral() noexcept
	{
		return ColorF(1.0f, 0.5f, 0.31f, 1.0f);
	}

	constexpr ColorF ColorF::Blue() noexcept
	{
		return ColorF(0.0f, 0.0f, 1.0f, 1.0f);
	}

	constexpr ColorF ColorF::Navy() noexcept
	{
		return ColorF(0.0f, 0.0f, 0.5f, 1.0f);
	}

	constexpr ColorF ColorF::Cyan() noexcept
	{
		return ColorF(0.0f, 1.0f, 1.0f, 1.0f);
	}

	constexpr ColorF ColorF::Teal() noexcept
	{
		return ColorF(0.0f, 0.5f, 0.5f, 1.0f);
	}

	constexpr ColorF ColorF::SkyBlue() noexcept
	{
		return ColorF(0.53f, 0.81f, 0.92f, 1.0f);
	}

	constexpr ColorF ColorF::Green() noexcept
	{
		return ColorF(0.0f, 0.5f, 0.0f, 1.0f);
	}

	constexpr ColorF ColorF::Lime() noexcept
	{
		return ColorF(0.0f, 1.0f, 0.0f, 1.0f);
	}

	constexpr ColorF ColorF::Olive() noexcept
	{
		return ColorF(0.5f, 0.5f, 0.0f, 1.0f);
	}

	constexpr ColorF ColorF::Emerald() noexcept
	{
		return ColorF(0.31f, 0.78f, 0.47f, 1.0f);
	}

	constexpr ColorF ColorF::Forest() noexcept
	{
		return ColorF(0.13f, 0.55f, 0.13f, 1.0f);
	}

	constexpr ColorF ColorF::Yellow() noexcept
	{
		return ColorF(1.0f, 1.0f, 0.0f, 1.0f);
	}

	constexpr ColorF ColorF::Orange() noexcept
	{
		return ColorF(1.0f, 0.65f, 0.0f, 1.0f);
	}

	constexpr ColorF ColorF::Gold() noexcept
	{
		return ColorF(1.0f, 0.84f, 0.0f, 1.0f);
	}

	constexpr ColorF ColorF::Amber() noexcept
	{
		return ColorF(1.0f, 0.75f, 0.0f, 1.0f);
	}

	constexpr ColorF ColorF::Purple() noexcept
	{
		return ColorF(0.5f, 0.0f, 0.5f, 1.0f);
	}

	constexpr ColorF ColorF::Violet() noexcept
	{
		return ColorF(0.56f, 0.0f, 1.0f, 1.0f);
	}

	constexpr ColorF ColorF::Magenta() noexcept
	{
		return ColorF(1.0f, 0.0f, 1.0f, 1.0f);
	}

	constexpr ColorF ColorF::Lavender() noexcept
	{
		return ColorF(0.9f, 0.9f, 0.98f, 1.0f);
	}

	constexpr ColorF ColorF::Brown() noexcept
	{
		return ColorF(0.65f, 0.16f, 0.16f, 1.0f);
	}

	constexpr ColorF ColorF::Beige() noexcept
	{
		return ColorF(0.96f, 0.96f, 0.86f, 1.0f);
	}

	constexpr ColorF ColorF::Tan() noexcept
	{
		return ColorF(0.82f, 0.71f, 0.55f, 1.0f);
	}

	constexpr ColorF ColorF::Chocolate() noexcept
	{
		return ColorF(0.82f, 0.41f, 0.12f, 1.0f);
	}

	constexpr ColorF ColorF::Transparent() noexcept
	{
		return ColorF(0.0f, 0.0f, 0.0f, 0.0f);
	}

	constexpr u32 ColorF::PackU32() const noexcept
	{
		return Color32::PackU32(
			std::clamp(R, 0.0f, 1.0f) * 255 + 0.5,
			std::clamp(G, 0.0f, 1.0f) * 255 + 0.5,
			std::clamp(B, 0.0f, 1.0f) * 255 + 0.5,
			std::clamp(A, 0.0f, 1.0f) * 255 + 0.5
		);
	}

	constexpr u32 ColorF::PackU32(float r, float g, float b, float a) noexcept
	{
		return Color32::PackU32(
			std::clamp(r, 0.0f, 1.0f) * 255 + 0.5,
			std::clamp(g, 0.0f, 1.0f) * 255 + 0.5,
			std::clamp(b, 0.0f, 1.0f) * 255 + 0.5,
			std::clamp(a, 0.0f, 1.0f) * 255 + 0.5
		);
	}

	constexpr ColorF ColorF::Lerp(float target, float t) const noexcept
	{
		t = std::clamp(t, 0.0f, 1.0f);

		return ColorF(R + (R + target) * t, G + (G + target) * t, B + (B + target) * t, A + (A + target) * t);
	}

	constexpr Color32::Color32(Color color) noexcept
	{
		switch (color)
		{
		case Color::Black:
			R = 0; G = 0; B = 0; A = 255;
			break;
		case Color::White:
			R = 255; G = 255; B = 255; A = 255;
			break;
		case Color::Gray:
			R = 128; G = 128; B = 128; A = 255;
			break;
		case Color::Silver:
			R = 192; G = 192; B = 192; A = 255;
			break;
		case Color::Red:
			R = 255; G = 0; B = 0; A = 255;
			break;
		case Color::Maroon:
			R = 128; G = 0; B = 0; A = 255;
			break;
		case Color::Pink:
			R = 255; G = 192; B = 203; A = 255;
			break;
		case Color::Crimson:
			R = 220; G = 20; B = 60; A = 255;
			break;
		case Color::Coral:
			R = 255; G = 127; B = 80; A = 255;
			break;
		case Color::Blue:
			R = 0; G = 0; B = 255; A = 255;
			break;
		case Color::Navy:
			R = 0; G = 0; B = 128; A = 255;
			break;
		case Color::Cyan:
			R = 0; G = 255; B = 255; A = 255;
			break;
		case Color::Teal:
			R = 0; G = 128; B = 128; A = 255;
			break;
		case Color::SkyBlue:
			R = 135; G = 206; B = 235; A = 255;
			break;
		case Color::Green:
			R = 0; G = 128; B = 0; A = 255;
			break;
		case Color::Lime:
			R = 0; G = 255; B = 0; A = 255;
			break;
		case Color::Olive:
			R = 128; G = 128; B = 0; A = 255;
			break;
		case Color::Emerald:
			R = 80; G = 200; B = 120; A = 255;
			break;
		case Color::Forest:
			R = 34; G = 139; B = 34; A = 255;
			break;
		case Color::Yellow:
			R = 255; G = 255; B = 0; A = 255;
			break;
		case Color::Orange:
			R = 255; G = 165; B = 0; A = 255;
			break;
		case Color::Gold:
			R = 255; G = 215; B = 0; A = 255;
			break;
		case Color::Amber:
			R = 255; G = 191; B = 0; A = 255;
			break;
		case Color::Purple:
			R = 128; G = 0; B = 128; A = 255;
			break;
		case Color::Violet:
			R = 143; G = 0; B = 255; A = 255;
			break;
		case Color::Magenta:
			R = 255; G = 0; B = 255; A = 255;
			break;
		case Color::Lavender:
			R = 230; G = 230; B = 250; A = 255;
			break;
		case Color::Brown:
			R = 165; G = 42; B = 42; A = 255;
			break;
		case Color::Beige:
			R = 245; G = 245; B = 220; A = 255;
			break;
		case Color::Tan:
			R = 210; G = 180; B = 140; A = 255;
			break;
		case Color::Chocolate:
			R = 210; G = 105; B = 30; A = 255;
			break;
		case Color::Transparent:
			R = 0; G = 0; B = 0; A = 0;
			break;
		}
	}

	constexpr Color32 Color32::White() noexcept
	{
		return Color32(255, 255, 255, 255);
	}

	constexpr Color32 Color32::Black() noexcept
	{
		return Color32(0, 0, 0, 255);
	}

	constexpr Color32 Color32::Gray() noexcept
	{
		return Color32(128, 128, 128, 255);
	}

	constexpr Color32 Color32::Silver() noexcept
	{
		return Color32(192, 192, 192, 255);
	}

	constexpr Color32 Color32::Red() noexcept
	{
		return Color32(255, 0, 0, 255);
	}

	constexpr Color32 Color32::Maroon() noexcept
	{
		return Color32(128, 0, 0, 255);
	}

	constexpr Color32 Color32::Pink() noexcept
	{
		return Color32(255, 192, 203, 255);
	}

	constexpr Color32 Color32::Crimson() noexcept
	{
		return Color32(220, 20, 60, 255);
	}

	constexpr Color32 Color32::Coral() noexcept
	{
		return Color32(255, 127, 80, 255);
	}

	constexpr Color32 Color32::Blue() noexcept
	{
		return Color32(0, 0, 255, 255);
	}

	constexpr Color32 Color32::Navy() noexcept
	{
		return Color32(0, 0, 128, 255);
	}

	constexpr Color32 Color32::Cyan() noexcept
	{
		return Color32(0, 255, 255, 255);
	}

	constexpr Color32 Color32::Teal() noexcept
	{
		return Color32(0, 128, 128, 255);
	}

	constexpr Color32 Color32::SkyBlue() noexcept
	{
		return Color32(135, 206, 235, 255);
	}

	constexpr Color32 Color32::Green() noexcept
	{
		return Color32(0, 128, 0, 255);
	}

	constexpr Color32 Color32::Lime() noexcept
	{
		return Color32(0, 255, 0, 255);
	}

	constexpr Color32 Color32::Olive() noexcept
	{
		return Color32(128, 128, 0, 255);
	}

	constexpr Color32 Color32::Emerald() noexcept
	{
		return Color32(80, 200, 120, 255);
	}

	constexpr Color32 Color32::Forest() noexcept
	{
		return Color32(34, 139, 34, 255);
	}

	constexpr Color32 Color32::Yellow() noexcept
	{
		return Color32(255, 255, 0, 255);
	}

	constexpr Color32 Color32::Orange() noexcept
	{
		return Color32(255, 165, 0, 255);
	}

	constexpr Color32 Color32::Gold() noexcept
	{
		return Color32(255, 215, 0, 255);
	}

	constexpr Color32 Color32::Amber() noexcept
	{
		return Color32(255, 191, 0, 255);
	}

	constexpr Color32 Color32::Purple() noexcept
	{
		return Color32(128, 0, 128, 255);
	}

	constexpr Color32 Color32::Violet() noexcept
	{
		return Color32(143, 0, 255, 255);
	}

	constexpr Color32 Color32::Magenta() noexcept
	{
		return Color32(255, 0, 255, 255);
	}

	constexpr Color32 Color32::Lavender() noexcept
	{
		return Color32(230, 230, 250, 255);
	}

	constexpr Color32 Color32::Brown() noexcept
	{
		return Color32(165, 42, 42, 255);
	}

	constexpr Color32 Color32::Beige() noexcept
	{
		return Color32(245, 245, 220, 255);
	}

	constexpr Color32 Color32::Tan() noexcept
	{
		return Color32(210, 180, 140, 255);
	}

	constexpr Color32 Color32::Chocolate() noexcept
	{
		return Color32(210, 105, 30, 255);
	}

	constexpr Color32 Color32::Transparent() noexcept
	{
		return Color32(0, 0, 0, 0);
	}

	constexpr u32 Color32::PackU32() const noexcept
	{
		return PackU32(R, G, B, A);
	}

	constexpr u32 Color32::PackU32(u8 r, u8 g, u8 b, u8 a) noexcept
	{
		return static_cast<u32>(a) << COL32_A_SHIFT |
			static_cast<u32>(b) << COL32_B_SHIFT |
			static_cast<u32>(g) << COL32_G_SHIFT |
			static_cast<u32>(r) << COL32_R_SHIFT;
	}

	constexpr Color32 Color32::PreMulAlpha() const noexcept
	{
		if (A == 0)
			return { 0,0,0,0 };
		if (A == 255)
			return *this;

		return Color32(static_cast<u8>((R * A + 127u) / 255u),
			static_cast<u8>((G * A + 127u) / 255u),
			static_cast<u8>((G * A + 127u) / 255u),
			A);
	}

	constexpr u8 Color32::PreMulAlpha(u8 color, u8 alpha) noexcept
	{
		if (alpha == 0)
			return 0;
		if (alpha == 255)
			return color;

		return static_cast<u8>((color * alpha + 127u) / 255u);
	}

	constexpr Color32 Color32::UnPreMulAlpha() const noexcept
	{
		if (A == 0)
			return { 0,0,0,0 };
		if (A == 255)
			return *this;

		return Color32(
			static_cast<u8>((((static_cast<u32>(R) * 255u * 255u + (A >> 1)) / A + 127u) / 255u)),
			static_cast<u8>((((static_cast<u32>(G) * 255u * 255u + (A >> 1)) / A + 127u) / 255u))
			, static_cast<u8>((((static_cast<u32>(B) * 255u * 255u + (A >> 1)) / A + 127u) / 255u))
			, A);
	}

	constexpr u8 Color32::UnPreMulAlpha(u8 color, u8 alpha) noexcept
	{
		if (alpha == 0)
			return 0;
		if (alpha == 255)
			return color;

		return static_cast<u8>((static_cast<u32>(color) * 255u * 255u + (alpha >> 1) / alpha + 127u) / 255u);
	}

	constexpr Color32 Color32::Lerp(u8 target, float t) const noexcept
	{
		t = std::clamp(t, 0.0f, 1.0f);

		return Color32(R + (R + target) * t, G + (G + target) * t, B + (B + target) * t, A + (A + target) * t);
	}
}
