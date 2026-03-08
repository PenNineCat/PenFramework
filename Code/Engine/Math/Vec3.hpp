// File /Engine/Math/Vec3.hpp
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

	struct alignas(16) Vec3
	{
		float X = 0;
		float Y = 0;
		float Z = 0;

		constexpr Vec3() noexcept = default;
		constexpr explicit Vec3(float v) noexcept : X(v), Y(v), Z(v) {}
		constexpr Vec3(float x, float y, float z) noexcept : X(x), Y(y), Z(z) {}

		constexpr Vec3 operator+() const noexcept;
		constexpr Vec3 operator-() const noexcept;

		constexpr Vec3& operator+=(const Vec3& v) noexcept;
		constexpr Vec3& operator-=(const Vec3& v) noexcept;
		constexpr Vec3& operator*=(const Vec3& v) noexcept;
		Vec3& operator/=(const Vec3& v);

		constexpr Vec3& operator+=(float v) noexcept;
		constexpr Vec3& operator-=(float v) noexcept;
		constexpr Vec3& operator*=(float v) noexcept;
		Vec3& operator/=(float v);

		friend constexpr Vec3 operator+(Vec3 l, const Vec3& r) noexcept
		{
			return l += r;
		}

		friend constexpr Vec3 operator-(Vec3 l, const Vec3& r) noexcept
		{
			return l -= r;
		}

		friend constexpr Vec3 operator*(Vec3 l, const Vec3& r) noexcept
		{
			return l *= r;
		}

		friend Vec3 operator/(Vec3 l, const Vec3& r)
		{
			return l /= r;
		}

		friend constexpr Vec3 operator+(Vec3 l, float r) noexcept
		{
			return l += r;
		}

		friend constexpr Vec3 operator-(Vec3 l, float r) noexcept
		{
			return l -= r;
		}

		friend constexpr Vec3 operator*(Vec3 l, float r) noexcept
		{
			return l *= r;
		}

		friend Vec3 operator/(Vec3 l, float r)
		{
			return l /= r;
		}

		friend constexpr bool operator==(const Vec3& l, const Vec3& r)
		{
			return NearAbs(l.X, r.X) && NearAbs(l.Y, r.Y) && NearAbs(l.Z, r.Z);
		}

		float& operator[](u8 index);
		const float& operator[](u8 index) const;

		[[nodiscard]] constexpr bool IsZero() const noexcept;
		[[nodiscard]] constexpr bool IsNearZero(float epsilon = MathEpsilon) const noexcept;

		[[nodiscard]] float Angle(const Vec3& v) const noexcept;

		[[nodiscard]] constexpr float Dot(const Vec3& v) const noexcept;

		[[nodiscard]] constexpr float LenSq() const noexcept;
		[[nodiscard]] float Len() const noexcept;

		void Normalize();
		[[nodiscard]] Vec3 Normalized() const;

		[[nodiscard]] float Distance() const noexcept;
		[[nodiscard]] constexpr float DistanceSq() const noexcept;
		[[nodiscard]] float Distance(const Vec3& v) const noexcept;
		[[nodiscard]] constexpr float DistanceSq(const Vec3& v) const noexcept;

		[[nodiscard]] constexpr Vec3 Lerp(const Vec3& b, float t) const noexcept;

		[[nodiscard]] constexpr Vec3 Reflect(const Vec3& v) const noexcept;
	};


	constexpr Vec3 Vec3::operator+() const noexcept
	{
		return *this;
	}

	constexpr Vec3 Vec3::operator-() const noexcept
	{
		return Vec3(-X, -Y, -Z);
	}

	constexpr Vec3& Vec3::operator+=(const Vec3& v) noexcept
	{
		X += v.X;
		Y += v.Y;
		Z += v.Z;

		return *this;
	}

	constexpr Vec3& Vec3::operator-=(const Vec3& v) noexcept
	{
		X -= v.X;
		Y -= v.Y;
		Z -= v.Z;

		return *this;
	}

	constexpr Vec3& Vec3::operator*=(const Vec3& v) noexcept
	{
		X *= v.X;
		Y *= v.Y;
		Z *= v.Z;

		return *this;
	}

	inline Vec3& Vec3::operator/=(const Vec3& v)
	{
		X /= v.X;
		Y /= v.Y;
		Z /= v.Z;

		return *this;
	}

	constexpr Vec3& Vec3::operator+=(float v) noexcept
	{
		X += v;
		Y += v;
		Z += v;

		return *this;
	}

	constexpr Vec3& Vec3::operator-=(float v) noexcept
	{
		X -= v;
		Y -= v;
		Z -= v;

		return *this;
	}

	constexpr Vec3& Vec3::operator*=(float v) noexcept
	{
		X *= v;
		Y *= v;
		Z *= v;

		return *this;
	}

	inline Vec3& Vec3::operator/=(float v)
	{
		X /= v;
		Y /= v;
		Z /= v;

		return *this;
	}

	inline float& Vec3::operator[](u8 index)
	{
		switch (index)
		{
		case 0:
			return X;
		case 1:
			return Y;
		case 2:
			return Z;
		default:
			throw std::out_of_range("Index只能为0，1或2");
		}
	}

	inline const float& Vec3::operator[](u8 index) const
	{
		switch (index)
		{
		case 0:
			return X;
		case 1:
			return Y;
		case 2:
			return Z;
		default:
			throw std::out_of_range("Index只能为0，1或2");
		}
	}

	constexpr bool Vec3::IsZero() const noexcept
	{
		return X == 0.0f && Y == 0.0f && Z == 0.0f;
	}

	constexpr bool Vec3::IsNearZero(float epsilon) const noexcept
	{
		return NearAbs(X, 0.0f, epsilon) && NearAbs(Y, 0.0f, epsilon) && NearAbs(Z, 0.0f, epsilon);
	}

	inline float Vec3::Angle(const Vec3& v) const noexcept
	{
		float len1 = Len();
		float len2 = v.Len();

		if (NearAbs(len1, 0.0f) || NearAbs(len2, 0.0f))
			return 0.0f;

		float cos = Dot(v) / (len1 * len2);
		cos = std::max(-1.0f, std::min(1.0f, cos));

		return acos(cos);
	}

	constexpr float Vec3::Dot(const Vec3& v) const noexcept
	{
		return X * v.X + Y * v.Y + Z * v.Z;
	}

	constexpr float Vec3::LenSq() const noexcept
	{
		return X * X + Y * Y + Z * Z;
	}

	inline float Vec3::Len() const noexcept
	{
		return std::sqrt(LenSq());
	}

	inline void Vec3::Normalize()
	{
		if (float len = Len(); !NearAbs(len, 0.0f))
			*this /= len;
	}

	inline Vec3 Vec3::Normalized() const
	{
		if (float len = Len(); !NearAbs(len, 0.0f))
			return *this / len;
		return Vec3(0);
	}

	inline float Vec3::Distance() const noexcept
	{
		return Len();
	}

	constexpr float Vec3::DistanceSq() const noexcept
	{
		return LenSq();
	}

	inline float Vec3::Distance(const Vec3& v) const noexcept
	{
		return std::sqrt(DistanceSq(v));
	}

	constexpr float Vec3::DistanceSq(const Vec3& v) const noexcept
	{
		return (X - v.X) * (X - v.X) + (Y - v.Y) * (Y - v.Y) + (Z - v.Z) * (Z - v.Z);
	}

	constexpr Vec3 Vec3::Lerp(const Vec3& b, float t) const noexcept
	{
		return Vec3(
			PenEngine::Lerp(X, b.X, t),
			PenEngine::Lerp(Y, b.Y, t),
			PenEngine::Lerp(Z, b.Z, t)
		);
	}

	constexpr Vec3 Vec3::Reflect(const Vec3& v) const noexcept
	{
		return *this - v * (2.0f * Dot(v));
	}

}
