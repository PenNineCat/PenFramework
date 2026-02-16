// File /Engine/Math/Vec4.hpp
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

namespace PenFramework::PenEngine
{
	struct alignas(16) Vec4
	{
		float X = 0;
		float Y = 0;
		float Z = 0;
		float W = 0;

		constexpr Vec4() noexcept = default;
		constexpr explicit Vec4(float v) noexcept : X(v), Y(v), Z(v), W(v) {}
		constexpr Vec4(float x, float y, float z, float w) noexcept : X(x), Y(y), Z(z), W(w) {}

		constexpr Vec4 operator+() const noexcept;
		constexpr Vec4 operator-() const noexcept;

		constexpr Vec4& operator+=(const Vec4& v) noexcept;
		constexpr Vec4& operator-=(const Vec4& v) noexcept;
		constexpr Vec4& operator*=(const Vec4& v) noexcept;
		Vec4& operator/=(const Vec4& v);

		constexpr Vec4& operator+=(float v) noexcept;
		constexpr Vec4& operator-=(float v) noexcept;
		constexpr Vec4& operator*=(float v) noexcept;
		Vec4& operator/=(float v);

		friend constexpr Vec4 operator+(Vec4 l, const Vec4& r) noexcept
		{
			return l += r;
		}

		friend constexpr Vec4 operator-(Vec4 l, const Vec4& r) noexcept
		{
			return l -= r;
		}

		friend constexpr Vec4 operator*(Vec4 l, const Vec4& r) noexcept
		{
			return l *= r;
		}

		friend Vec4 operator/(Vec4 l, const Vec4& r)
		{
			return l /= r;
		}

		friend constexpr Vec4 operator+(Vec4 l, float r) noexcept
		{
			return l += r;
		}

		friend constexpr Vec4 operator-(Vec4 l, float r) noexcept
		{
			return l -= r;
		}

		friend constexpr Vec4 operator*(Vec4 l, float r) noexcept
		{
			return l *= r;
		}

		friend Vec4 operator/(Vec4 l, float r)
		{
			return l /= r;
		}

		friend constexpr bool operator==(const Vec4& l, const Vec4& r)
		{
			return NearAbs(l.X, r.X) && NearAbs(l.Y, r.Y) && NearAbs(l.Z, r.Z) && NearAbs(l.W, r.W);
		}

		float& operator[](u8 index);
		const float& operator[](u8 index) const;

		[[nodiscard]] constexpr bool IsZero() const noexcept;
		[[nodiscard]] constexpr bool IsNearZero(float epsilon = MathEpsilon) const noexcept;

		[[nodiscard]] float Angle(const Vec4& v) const noexcept;

		[[nodiscard]] constexpr float Dot(const Vec4& v) const noexcept;

		[[nodiscard]] constexpr float LenSq() const noexcept;
		[[nodiscard]] float Len() const noexcept;

		void Normalize();
		[[nodiscard]] Vec4 Normalized() const;

		[[nodiscard]] float Distance() const noexcept;
		[[nodiscard]] constexpr float DistanceSq() const noexcept;
		[[nodiscard]] float Distance(const Vec4& v) const noexcept;
		[[nodiscard]] constexpr float DistanceSq(const Vec4& v) const noexcept;

		[[nodiscard]] constexpr Vec4 Lerp(const Vec4& b, float t) const noexcept;

		[[nodiscard]] constexpr Vec4 Reflect(const Vec4& v) const noexcept;
	};


	constexpr Vec4 Vec4::operator+() const noexcept
	{
		return *this;
	}

	constexpr Vec4 Vec4::operator-() const noexcept
	{
		return Vec4(-X, -Y, -Z, -W);
	}

	constexpr Vec4& Vec4::operator+=(const Vec4& v) noexcept
	{
		X += v.X;
		Y += v.Y;
		Z += v.Z;
		W += v.W;

		return *this;
	}

	constexpr Vec4& Vec4::operator-=(const Vec4& v) noexcept
	{
		X -= v.X;
		Y -= v.Y;
		Z -= v.Z;
		W -= v.W;

		return *this;
	}

	constexpr Vec4& Vec4::operator*=(const Vec4& v) noexcept
	{
		X *= v.X;
		Y *= v.Y;
		Z *= v.Z;
		W *= v.W;

		return *this;
	}

	inline Vec4& Vec4::operator/=(const Vec4& v)
	{
		X /= v.X;
		Y /= v.Y;
		Z /= v.Z;
		W /= v.W;

		return *this;
	}

	constexpr Vec4& Vec4::operator+=(float v) noexcept
	{
		X += v;
		Y += v;
		Z += v;
		W += v;

		return *this;
	}

	constexpr Vec4& Vec4::operator-=(float v) noexcept
	{
		X -= v;
		Y -= v;
		Z -= v;
		W -= v;

		return *this;
	}

	constexpr Vec4& Vec4::operator*=(float v) noexcept
	{
		X *= v;
		Y *= v;
		Z *= v;
		W *= v;

		return *this;
	}

	inline Vec4& Vec4::operator/=(float v)
	{
		X /= v;
		Y /= v;
		Z /= v;
		W /= v;

		return *this;
	}

	inline float& Vec4::operator[](u8 index)
	{
		switch (index)
		{
		case 0:
			return X;
		case 1:
			return Y;
		case 2:
			return Z;
		case 3:
			return W;
		default:
			throw std::out_of_range("Index只能为0，1，2或3");
		}
	}

	inline const float& Vec4::operator[](u8 index) const
	{
		switch (index)
		{
		case 0:
			return X;
		case 1:
			return Y;
		case 2:
			return Z;
		case 3:
			return W;
		default:
			throw std::out_of_range("Index只能为0，1，2或3");
		}
	}

	constexpr bool Vec4::IsZero() const noexcept
	{
		return X == 0.0f && Y == 0.0f && Z == 0.0f && W == 0.0f;
	}

	constexpr bool Vec4::IsNearZero(float epsilon) const noexcept
	{
		return NearAbs(X, 0.0f, epsilon) && NearAbs(Y, 0.0f, epsilon) && NearAbs(Z, 0.0f, epsilon) && NearAbs(W, 0.0f, epsilon);
	}

	inline float Vec4::Angle(const Vec4& v) const noexcept
	{
		float len1 = Len();
		float len2 = v.Len();

		if (NearAbs(len1, 0.0f) || NearAbs(len2, 0.0f))
			return 0.0f;

		float cos = Dot(v) / (len1 * len2);
		cos = std::max(-1.0f, std::min(1.0f, cos));

		return acos(cos);
	}

	constexpr float Vec4::Dot(const Vec4& v) const noexcept
	{
		return X * v.X + Y * v.Y + Z * v.Z + W * v.W;
	}

	constexpr float Vec4::LenSq() const noexcept
	{
		return X * X + Y * Y + Z * Z + W * W;
	}

	inline float Vec4::Len() const noexcept
	{
		return std::sqrt(LenSq());
	}

	inline void Vec4::Normalize()
	{
		if (float len = Len(); !NearAbs(len, 0.0f))
			*this /= len;
	}

	inline Vec4 Vec4::Normalized() const
	{
		if (float len = Len(); !NearAbs(len, 0.0f))
			return *this / len;

		return Vec4(0);
	}

	inline float Vec4::Distance() const noexcept
	{
		return Len();
	}

	constexpr float Vec4::DistanceSq() const noexcept
	{
		return LenSq();
	}

	inline float Vec4::Distance(const Vec4& v) const noexcept
	{
		return std::sqrt(DistanceSq(v));
	}

	constexpr float Vec4::DistanceSq(const Vec4& v) const noexcept
	{
		return (X - v.X) * (X - v.X) + (Y - v.Y) * (Y - v.Y) + (Z - v.Z) * (Z - v.Z) + (W - v.W) * (W - v.W);
	}

	constexpr Vec4 Vec4::Lerp(const Vec4& b, float t) const noexcept
	{
		return Vec4(
			PenEngine::Lerp(X, b.X, t),
			PenEngine::Lerp(Y, b.Y, t),
			PenEngine::Lerp(Z, b.Z, t),
			PenEngine::Lerp(W, b.W, t)
		);
	}

	constexpr Vec4 Vec4::Reflect(const Vec4& v) const noexcept
	{
		return *this - v * (2.0f * Dot(v));
	}
}
