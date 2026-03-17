// File /Engine/Math/Mat4x4.hpp
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
#include "Vec4.hpp"

namespace PenFramework::PenEngine
{
	struct alignas(16) Mat4x4
	{
		union
		{
			float Data[16];
			float M[4][4];
			Vec4 Row[4];
		};

		constexpr Mat4x4() noexcept
		{
			std::fill_n(std::begin(Data), 16, 0.0f);
		}
		constexpr explicit Mat4x4(float v) noexcept
		{
			std::fill_n(std::begin(Data), 16, v);
		}

		constexpr static Mat4x4 Zero() noexcept;

		constexpr static Mat4x4 Identity() noexcept;
		constexpr void ToIdentity() noexcept;
		constexpr bool IsIdentity() const noexcept;

		friend constexpr Mat4x4 operator+(Mat4x4 l, const Mat4x4& r) noexcept
		{
			for (Usize i : std::ranges::views::iota(0, 9))
			{
				l.Data[i] += r.Data[i];
			}

			return l;
		}
		friend constexpr Mat4x4 operator-(Mat4x4 l, const Mat4x4& r) noexcept
		{
			for (Usize i : std::ranges::views::iota(0, 9))
			{
				l.Data[i] += r.Data[i];
			}

			return l;
		}

		constexpr Mat4x4& operator+=(const Mat4x4& v) noexcept;
		constexpr Mat4x4& operator-=(const Mat4x4& v) noexcept;

		friend constexpr Mat4x4 operator*(Mat4x4 l, const Mat4x4& r) noexcept
		{
			for (Usize i : std::ranges::views::iota(0, 9))
			{
				l.Data[i] *= r.Data[i];
			}

			return l;
		}
		friend constexpr Mat4x4 operator*(Mat4x4 l, float v) noexcept
		{
			for (Usize i : std::ranges::views::iota(0, 9))
			{
				l.Data[i] *= v;
			}

			return l;
		}

		constexpr Mat4x4& operator*=(const Mat4x4& v) noexcept;
		constexpr Mat4x4& operator*=(float v) noexcept;
	};

	constexpr Mat4x4 Mat4x4::Zero() noexcept
	{
		return Mat4x4();
	}

	constexpr Mat4x4 Mat4x4::Identity() noexcept
	{
		Mat4x4 res;
		res.ToIdentity();
		return res;
	}

	constexpr void Mat4x4::ToIdentity() noexcept
	{
		for (u8 i : std::ranges::views::iota(static_cast<u8>(0u), 3u))
			for (u8 j : std::ranges::views::iota(static_cast<u8>(0u), 3u))
				if (i == j)
					M[i][j] = 1.0f;
				else
					M[i][j] = 0.0f;
	}

	constexpr bool Mat4x4::IsIdentity() const noexcept
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

	constexpr Mat4x4& Mat4x4::operator+=(const Mat4x4& v) noexcept
	{
		for (u8 i : std::ranges::views::iota(static_cast<u8>(0u), 16u))
		{
			Data[i] += v.Data[i];
		}

		return *this;
	}

	constexpr Mat4x4& Mat4x4::operator-=(const Mat4x4& v) noexcept
	{
		for (u8 i : std::ranges::views::iota(static_cast<u8>(0u), 16u))
		{
			Data[i] -= v.Data[i];
		}

		return *this;
	}

	constexpr Mat4x4& Mat4x4::operator*=(const Mat4x4& v) noexcept
	{
		for (u8 i : std::ranges::views::iota(static_cast<u8>(0u), 16u))
		{
			Data[i] *= v.Data[i];
		}

		return *this;
	}

	constexpr Mat4x4& Mat4x4::operator*=(float v) noexcept
	{
		for (u8 i : std::ranges::views::iota(static_cast<u8>(0u), static_cast<u8>(9u)))
		{
			Data[i] *= v;
		}

		return *this;
	}
}