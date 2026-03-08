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

	template <typename T>
	using NumericLimits = std::numeric_limits<T>;


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
	[[nodiscard]] constexpr T Clamp(T value, T low, T high) noexcept
	{
		T tmp = value >= low ? value : low;
		return  tmp <= high ? tmp : high;
	}

	template <typename T>
	[[nodiscard]] constexpr T Clamp01(T value) noexcept
	{
		return Clamp(value, static_cast<T>(0), static_cast<T>(1));
	}

	template <typename T> requires std::is_arithmetic_v<T>
	[[nodiscard]] constexpr T Pow(T base, usize exponent) noexcept
	{
		if consteval
		{
			if (exponent == static_cast<T>(0))
				return static_cast<T>(1);

			if (base == static_cast<T>(0))
				return static_cast<T>(0);

			T result = static_cast<T>(1);

			T currentProduct = base;
			T currentExponent = exponent;

			while (currentExponent > static_cast<T>(0))
			{
				if (static_cast<int>(currentExponent) % 2 == 1)
				{
					result *= currentProduct;
				}
				currentProduct *= currentProduct;
				currentExponent /= static_cast<T>(2);
			}
			return result;
		}

		return std::pow(base, exponent);
	}

	template <typename T> requires std::is_floating_point_v<T>
	[[nodiscard]] constexpr T Floor(T value) noexcept
	{
		if consteval
		{
			if (value == static_cast<T>(0))
				return value;

			if (IsInfinity(value))
				return value;

			if (IsNaN(value))
				return value;

			if (value > 0)
			{
				constexpr T maxComputeValue = static_cast<T>(1) / NumericLimits<T>::epsilon();

				if (value >= maxComputeValue)
					return value;

				T res = 1;

				if (res <= value)
				{
					while (res < value)
					{
						res *= 2;
					}
					while (res > value)
					{
						--res;
					}

					return res;
				}

				return 0;
			}

			T res = -1;

			if (res > value)
			{
				while (res > value)
				{
					res *= 2;
				}
				while (res < value)
				{
					++res;
				}
				if (res != value)
				{
					--res;
				}
			}

			return res;
		}

		return std::floor(value);
	}
	
	template <typename T> requires std::is_floating_point_v<T>
	[[nodiscard]] constexpr T Ceil(T value) noexcept
	{
		if consteval
		{
			T result = Floor(value);

			if (result == value)
			{
				return result;
			}

			return result + 1;
		}

		return std::ceil(value);
	}

	template <typename T> requires std::is_integral_v<T>&& std::is_signed_v<T>
	[[nodiscard]] constexpr T Abs(T value) noexcept
	{
		if consteval
		{
			T mask = value >> (sizeof(T) * 8 - 1);
			return (value + mask) ^ mask;
		}

		return std::abs(value);
	}

	template <typename T> requires std::is_floating_point_v<T>
	[[nodiscard]] constexpr T Abs(T value) noexcept
	{
		if consteval
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
		return std::abs(value);
	}

	template <typename T> requires std::is_arithmetic_v<T>
	[[nodiscard]] constexpr T Lerp(T a, T b, float t) noexcept
	{
		return a + (b - a) * Clamp01(t);
	}

	template <typename T> requires std::is_floating_point_v<T>
	[[nodiscard]] constexpr bool IsInfinity(T value) noexcept
	{
		if consteval
		{
			return value == NumericLimits<T>::infinity() || value == -NumericLimits<T>::infinity();  // NOLINT(clang-diagnostic-float-equal)
		}

		return std::isinf(value);
	}

	template <typename T> requires std::is_floating_point_v<T>
	[[nodiscard]] constexpr bool IsNaN(T value) noexcept
	{
		if consteval
		{
		// ReSharper disable once CppIdenticalOperandsInBinaryExpression
			return value != value;  // NOLINT(misc-redundant-expression)
		}

		return std::isnan(value);
	}

	template <typename T> requires std::is_floating_point_v<T>
	[[nodiscard]] constexpr bool NearAbs(T a, T b, float epsilon = MathEpsilon) noexcept
	{
		if (a == b)  // NOLINT(clang-diagnostic-float-equal)
			return true;

		if (IsInfinity(a) || IsInfinity(b))
			return false;

		return Abs(a - b) <= epsilon;
	}

	template <typename T> requires std::is_floating_point_v<T>
	[[nodiscard]] constexpr bool NearRel(T a, T b, float epsilon = MathEpsilon) noexcept
	{
		if (a == b)  // NOLINT(clang-diagnostic-float-equal)
			return true;

		if (IsInfinity(a) || IsInfinity(b))
			return false;

		return Abs(a - b) <= epsilon * std::max(Abs(a), Abs(b));
	}
}
