// File /Engine/Math/Math.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "../Common/Type.hpp"
#include <bit>
#include <cmath>
#include <numbers>
#include <ranges>
#include <stdexcept>

namespace PenFramework::PenEngine
{
	/// @brief 数学库全局容差
	constexpr float MathEpsilon = 1e-5f;

	constexpr double Pi = std::numbers::pi;
	constexpr double E = std::numbers::e;
	constexpr double Phi = std::numbers::phi;
	constexpr double Ln2 = std::numbers::ln2;
	constexpr double Ln10 = std::numbers::ln10;

	/// @brief 计算向上取整到N的倍数的值
	/// @param n 必须是2的幂且大于等于2
	/// @param x 输入值
	/// @return 向上取整到N的倍数的结果，如果n不满足则返回0
	[[nodiscard]] constexpr u64 CeilPow2(u8 x, u8 n) noexcept
	{
		if (n < 2 || !std::has_single_bit(n))
			return 0;
		return (x + n - 1) >> std::countr_zero(n);
	}
	/// @brief 计算向上取整到64的倍数的值
	/// @param x 输入值
	/// @return 向上取整到64的倍数的结果
	[[nodiscard]] constexpr u64 Ceil64(u8 x) noexcept { return CeilPow2(x, 64); }

	/// @brief 计算向上取整到32的倍数的值
	/// @param x 输入值
	/// @return 向上取整到32的倍数的结果
	[[nodiscard]] constexpr u64 Ceil32(u8 x) noexcept { return CeilPow2(x, 32); }

	/// @brief 计算向上取整到16的倍数的值
	/// @param x 输入值
	/// @return 向上取整到16的倍数的结果
	[[nodiscard]] constexpr u64 Ceil16(u8 x) noexcept { return CeilPow2(x, 16); }

	/// @brief 计算向上取整到8的倍数的值
	/// @param x 输入值
	/// @return 向上取整到8的倍数的结果
	[[nodiscard]] constexpr u64 Ceil8(u8 x) noexcept { return CeilPow2(x, 8); }

	/// @brief 计算向上取整到4的倍数的值
	/// @param x 输入值
	/// @return 向上取整到4的倍数的结果
	[[nodiscard]] constexpr u64 Ceil4(u8 x) noexcept { return CeilPow2(x, 4); }

	template <typename T> requires std::is_integral_v<T> || std::is_floating_point_v<T>
	[[nodiscard]] constexpr T Clamp(T value, T low, T high)
	{
		T tmp = value >= low ? value : low;
		return  tmp <= high ? tmp : high;
	}

	template <typename T>
	[[nodiscard]] constexpr T Clamp01(T value)
	{
		return Clamp(value,static_cast<T>(0),static_cast<T>(1));
	}

	template <typename T> requires std::is_integral_v<T>&& std::is_signed_v<T>
	[[nodiscard]] constexpr T Abs(T value)
	{
		T mask = value >> (sizeof(T) * 8 - 1);
		return (value + mask) ^ mask;
	}

	template <typename T> requires std::is_floating_point_v<T>
	[[nodiscard]] constexpr T Abs(T value)
	{
		if constexpr (std::is_same_v<T, float>)
		{
			auto bits = std::bit_cast<std::uint32_t>(value);
			bits &= 0x7FFFFFFFU;
			return std::bit_cast<float>(bits);
		}
		else if constexpr (std::is_same_v<T, double>)
		{
			auto bits = std::bit_cast<std::uint64_t>(value);
			bits &= 0x7FFFFFFFFFFFFFFFULL;
			return std::bit_cast<double>(bits);
		}
		else
		{
			return value < 0.0f ? -value : value;
		}
	}

	template <typename T> requires std::is_arithmetic_v<T>
	[[nodiscard]] constexpr T Lerp(T a, T b, float t)
	{
		return a + (b - a) * Clamp01(t);
	}

	template <typename T> requires std::is_floating_point_v<T>
	[[nodiscard]] constexpr bool IsInfinity(T value)
	{
		return value == std::numeric_limits<T>::infinity() || value == -std::numeric_limits<T>::infinity();  // NOLINT(clang-diagnostic-float-equal)
	}

	template <typename T> requires std::is_floating_point_v<T>
	[[nodiscard]] constexpr bool IsNaN(T value)
	{
		// ReSharper disable once CppIdenticalOperandsInBinaryExpression
		return value != value;  // NOLINT(misc-redundant-expression)
	}

	template <typename T> requires std::is_floating_point_v<T>
	[[nodiscard]] constexpr bool NearAbs(T a, T b, float epsilon = MathEpsilon)
	{
		if (a == b)  // NOLINT(clang-diagnostic-float-equal)
			return true;

		if (IsInfinity(a) || IsInfinity(b))
			return false;

		return Abs(a - b) <= epsilon;
	}

	template <typename T> requires std::is_floating_point_v<T>
	[[nodiscard]] constexpr bool NearRel(T a, T b, float epsilon = MathEpsilon)
	{
		if (a == b)  // NOLINT(clang-diagnostic-float-equal)
			return true;

		if (IsInfinity(a) || IsInfinity(b))
			return false;

		return Abs(a - b) <= epsilon * std::max(Abs(a), Abs(b));
	}
}
