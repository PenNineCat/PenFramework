// File /Engine/Math/Vec4x4.hpp
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
	struct alignas(16) Vec4x4
	{
		union
		{
			float Data[16];
			float M[4][4];
			Vec4 Row[4];
		};

		constexpr Vec4x4() noexcept
		{
			std::fill_n(std::begin(Data), 16, 0.0f);
		}
		constexpr explicit Vec4x4(float v) noexcept
		{
			std::fill_n(std::begin(Data), 16, v);
		}

		constexpr static Vec4x4 Zero() noexcept;

		constexpr static Vec4x4 Identity() noexcept;
		constexpr void ToIdentity() noexcept;
		constexpr bool IsIdentity() const noexcept;

		friend constexpr Vec4x4 operator+(Vec4x4 l, const Vec4x4& r) noexcept
		{
			for (usize i : std::ranges::views::iota(0, 9))
			{
				l.Data[i] += r.Data[i];
			}

			return l;
		}
		friend constexpr Vec4x4 operator-(Vec4x4 l, const Vec4x4& r) noexcept
		{
			for (usize i : std::ranges::views::iota(0, 9))
			{
				l.Data[i] += r.Data[i];
			}

			return l;
		}

		constexpr Vec4x4& operator+=(const Vec4x4& v) noexcept;
		constexpr Vec4x4& operator-=(const Vec4x4& v) noexcept;

		friend constexpr Vec4x4 operator*(Vec4x4 l, const Vec4x4& r) noexcept
		{
			for (usize i : std::ranges::views::iota(0, 9))
			{
				l.Data[i] *= r.Data[i];
			}

			return l;
		}
		friend constexpr Vec4x4 operator*(Vec4x4 l, float v) noexcept
		{
			for (usize i : std::ranges::views::iota(0, 9))
			{
				l.Data[i] *= v;
			}

			return l;
		}

		constexpr Vec4x4& operator*=(const Vec4x4& v) noexcept;
		constexpr Vec4x4& operator*=(float v) noexcept;
	};

	constexpr Vec4x4 Vec4x4::Zero() noexcept
	{
		return Vec4x4();
	}

	constexpr Vec4x4 Vec4x4::Identity() noexcept
	{
		Vec4x4 res;
		res.ToIdentity();
		return res;
	}

	constexpr void Vec4x4::ToIdentity() noexcept
	{
		for (u8 i : std::ranges::views::iota(static_cast<u8>(0u), 3u))
			for (u8 j : std::ranges::views::iota(static_cast<u8>(0u), 3u))
				if (i == j)
					M[i][j] = 1.0f;
				else
					M[i][j] = 0.0f;
	}

	constexpr bool Vec4x4::IsIdentity() const noexcept
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

	constexpr Vec4x4& Vec4x4::operator+=(const Vec4x4& v) noexcept
	{
		for (u8 i : std::ranges::views::iota(static_cast<u8>(0u), 16u))
		{
			Data[i] += v.Data[i];
		}

		return *this;
	}

	constexpr Vec4x4& Vec4x4::operator-=(const Vec4x4& v) noexcept
	{
		for (u8 i : std::ranges::views::iota(static_cast<u8>(0u), 16u))
		{
			Data[i] -= v.Data[i];
		}

		return *this;
	}

	constexpr Vec4x4& Vec4x4::operator*=(const Vec4x4& v) noexcept
	{
		for (u8 i : std::ranges::views::iota(static_cast<u8>(0u), 16u))
		{
			Data[i] *= v.Data[i];
		}

		return *this;
	}

	constexpr Vec4x4& Vec4x4::operator*=(float v) noexcept
	{
		for (u8 i : std::ranges::views::iota(static_cast<u8>(0u), static_cast<u8>(9u)))
		{
			Data[i] *= v;
		}

		return *this;
	}
}