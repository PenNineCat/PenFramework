// File /Engine/Math/Mat3x3.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "MathFunction.hpp"
#include "Vec3.hpp"

namespace PenFramework::PenEngine
{
	struct alignas(16) Mat3x3
	{
		union
		{
			float M[3][3];
			float Data[9];
			Vec3 Row[3];
		};

		constexpr Mat3x3() noexcept
		{
			std::fill_n(std::begin(Data), 9, 0.0f);
		}
		constexpr explicit Mat3x3(float v) noexcept
		{
			std::fill_n(std::begin(Data), 9, v);
		}

		constexpr static Mat3x3 Zero() noexcept;

		constexpr static Mat3x3 Identity() noexcept;
		constexpr void ToIdentity() noexcept;
		constexpr bool IsIdentity() const noexcept;

		friend constexpr Mat3x3 operator+(Mat3x3 l, const Mat3x3& r) noexcept
		{
			for (Usize i : std::ranges::views::iota(0, 9))
			{
				l.Data[i] += r.Data[i];
			}

			return l;
		}
		friend constexpr Mat3x3 operator-(Mat3x3 l, const Mat3x3& r) noexcept
		{
			for (Usize i : std::ranges::views::iota(0, 9))
			{
				l.Data[i] += r.Data[i];
			}

			return l;
		}

		constexpr Mat3x3& operator+=(const Mat3x3& v) noexcept;
		constexpr Mat3x3& operator-=(const Mat3x3& v) noexcept;

		friend constexpr Mat3x3 operator*(Mat3x3 l, const Mat3x3& r) noexcept
		{
			for (Usize i : std::ranges::views::iota(0, 9))
			{
				l.Data[i] *= r.Data[i];
			}

			return l;
		}
		friend constexpr Mat3x3 operator*(Mat3x3 l, float v) noexcept
		{
			for (Usize i : std::ranges::views::iota(0, 9))
			{
				l.Data[i] *= v;
			}

			return l;
		}

		constexpr Mat3x3& operator*=(const Mat3x3& v) noexcept;
		constexpr Mat3x3& operator*=(float v) noexcept;
	};

	constexpr Mat3x3 Mat3x3::Zero() noexcept
	{
		return Mat3x3();
	}

	constexpr Mat3x3 Mat3x3::Identity() noexcept
	{
		Mat3x3 res;
		res.ToIdentity();
		return res;
	}

	constexpr void Mat3x3::ToIdentity() noexcept
	{
		for (u8 i : std::ranges::views::iota(static_cast<u8>(0u), 3u))
			for (u8 j : std::ranges::views::iota(static_cast<u8>(0u), 3u))
				if (i == j)
					M[i][j] = 1.0f;
				else
					M[i][j] = 0.0f;
	}

	constexpr bool Mat3x3::IsIdentity() const noexcept
	{
		for (u8 i : std::ranges::views::iota(static_cast<u8>(0u), 3u))
			for (u8 j : std::ranges::views::iota(static_cast<u8>(0u), 3u))
				if (i == j)
				{
					if (M[i][j] != 1.0f)
						return false;
				}
				else
				{
					if (M[i][j] != 0.0f)
						return false;
				}

		return true;
	}

	constexpr Mat3x3& Mat3x3::operator+=(const Mat3x3& v) noexcept
	{
		for (Usize i : std::ranges::views::iota(0, 9))
		{
			Data[i] += v.Data[i];
		}

		return *this;
	}

	constexpr Mat3x3& Mat3x3::operator-=(const Mat3x3& v) noexcept
	{
		for (Usize i : std::ranges::views::iota(0, 9))
		{
			Data[i] -= v.Data[i];
		}

		return *this;
	}

	constexpr Mat3x3& Mat3x3::operator*=(const Mat3x3& v) noexcept
	{
		for (u8 i : std::ranges::views::iota(static_cast<u8>(0u), static_cast<u8>(9u)))
		{
			Data[i] *= v.Data[i];
		}

		return *this;
	}

	constexpr Mat3x3& Mat3x3::operator*=(float v) noexcept
	{
		for (u8 i : std::ranges::views::iota(static_cast<u8>(0u), static_cast<u8>(9u)))
		{
			Data[i] *= v;
		}

		return *this;
	}
}