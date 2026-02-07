#pragma once

#include "../Common/Type.hpp"
#include <array>
#include <bit>
#include <bitset>
#include <cmath>
#include <numbers>
#include <numeric>
#include <ranges>
#include <stdexcept>

namespace PenFramework::PenEngine
{
	/// @brief 数学库全局容差
	constexpr float MATH_EPSILON = 1e-5f;

	constexpr double PI = std::numbers::pi;
	constexpr double E = std::numbers::e;
	constexpr double PHI = std::numbers::phi;
	constexpr double LN2 = std::numbers::ln2;
	constexpr double LN10 = std::numbers::ln10;

	/// @brief 计算向上取整到N的倍数的值
	/// @param n 必须是2的幂且大于等于2
	/// @param x 输入值
	/// @return 向上取整到N的倍数的结果，如果n不满足则返回0
	constexpr u64 CeilPow2(u8 x, u8 n) noexcept
	{
		if (n < 2 || !std::has_single_bit(n))
			return 0;
		return (x + n - 1) >> std::countr_zero(n);
	}
	/// @brief 计算向上取整到64的倍数的值
	/// @param x 输入值
	/// @return 向上取整到64的倍数的结果
	constexpr u64 Ceil64(u8 x) noexcept { return CeilPow2(x, 64); }

	/// @brief 计算向上取整到32的倍数的值
	/// @param x 输入值
	/// @return 向上取整到32的倍数的结果
	constexpr u64 Ceil32(u8 x) noexcept { return CeilPow2(x, 32); }

	/// @brief 计算向上取整到16的倍数的值
	/// @param x 输入值
	/// @return 向上取整到16的倍数的结果
	constexpr u64 Ceil16(u8 x) noexcept { return CeilPow2(x, 16); }

	/// @brief 计算向上取整到8的倍数的值
	/// @param x 输入值
	/// @return 向上取整到8的倍数的结果
	constexpr u64 Ceil8(u8 x) noexcept { return CeilPow2(x, 8); }

	/// @brief 计算向上取整到4的倍数的值
	/// @param x 输入值
	/// @return 向上取整到4的倍数的结果
	constexpr u64 Ceil4(u8 x) noexcept { return CeilPow2(x, 4); }

	template <typename T> requires std::is_integral_v<T>&& std::is_signed_v<T>
	constexpr T Abs(T value)
	{
		T mask = value >> (sizeof(T) * 8 - 1);
		return (value + mask) ^ mask;
	}

	template <typename T> requires std::is_floating_point_v<T>
	constexpr T Abs(T value)
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
	constexpr T Lerp(T a, T b, float t)
	{
		return a + (b - a) * t;
	}

	template <typename T> requires std::is_floating_point_v<T>
	constexpr bool IsInfinity(T value)
	{
		return value == std::numeric_limits<T>::infinity() || value == -std::numeric_limits<T>::infinity();
	}

	template <typename T> requires std::is_floating_point_v<T>
	constexpr bool IsNaN(T value)
	{
		return value != value;
	}

	template <typename T> requires std::is_floating_point_v<T>
	constexpr bool NearAbs(T a, T b, float epsilon = MATH_EPSILON)
	{
		if (a == b)
			return true;

		if (IsInfinity(a) || IsInfinity(b))
			return false;

		return Abs(a - b) <= epsilon;
	}

	template <typename T> requires std::is_floating_point_v<T>
	constexpr bool NearRel(T a, T b, float epsilon = MATH_EPSILON)
	{
		if (a == b)
			return true;

		if (IsInfinity(a) || IsInfinity(b))
			return false;

		return Abs(a - b) <= epsilon * std::max(Abs(a),Abs(b));
	}

	struct Vec2
	{
		float X = 0;
		float Y = 0;

		constexpr Vec2() noexcept = default;
		constexpr explicit Vec2(float v) noexcept : X(v), Y(v) {}
		constexpr Vec2(float x, float y) noexcept : X(x), Y(y) {}

		constexpr Vec2 operator+() const noexcept;
		constexpr Vec2 operator-() const noexcept;

		constexpr Vec2& operator++() noexcept;
		constexpr Vec2 operator++(int) noexcept;
		constexpr Vec2& operator--() noexcept;
		constexpr Vec2 operator--(int) noexcept;

		constexpr Vec2& operator+=(const Vec2& v) noexcept;
		constexpr Vec2& operator-=(const Vec2& v) noexcept;
		constexpr Vec2& operator*=(const Vec2& v) noexcept;
		Vec2& operator/=(const Vec2& v);

		constexpr Vec2& operator+=(float v) noexcept;
		constexpr Vec2& operator-=(float v) noexcept;
		constexpr Vec2& operator*=(float v) noexcept;
		Vec2& operator/=(float v);

		friend constexpr Vec2 operator+(Vec2 l, const Vec2& r) noexcept
		{
			return l += r;
		}

		friend constexpr Vec2 operator-(Vec2 l, const Vec2& r) noexcept
		{
			return l -= r;
		}

		friend constexpr Vec2 operator*(Vec2 l, const Vec2& r) noexcept
		{
			return l *= r;
		}

		friend Vec2 operator/(Vec2 l, const Vec2& r)
		{
			return l /= r;
		}

		friend constexpr Vec2 operator+(Vec2 l, float r) noexcept
		{
			return l += r;
		}

		friend constexpr Vec2 operator-(Vec2 l, float r) noexcept
		{
			return l -= r;
		}

		friend constexpr Vec2 operator*(Vec2 l, float r) noexcept
		{
			return l *= r;
		}

		friend Vec2 operator/(Vec2 l, float r)
		{
			return l /= r;
		}

		friend constexpr bool operator==(const Vec2& l, const Vec2& r)
		{
			return NearAbs(l.X, r.X) && NearAbs(l.Y, r.Y);
		}

		float& operator[](u8 index);
		const float& operator[](u8 index) const;

		[[nodiscard]] constexpr bool IsZero() const noexcept;
		[[nodiscard]] constexpr bool IsNearZero(float epsilon = MATH_EPSILON) const noexcept;

		[[nodiscard]] float Angle(const Vec2& v) const noexcept;

		[[nodiscard]] constexpr float Dot(const Vec2& v) const noexcept;

		[[nodiscard]] constexpr float LenSq() const noexcept;
		[[nodiscard]] float Len() const noexcept;

		void Normalize();
		[[nodiscard]] Vec2 Normalized() const;

		[[nodiscard]] float Distance() const noexcept;
		[[nodiscard]] constexpr float DistanceSq() const noexcept;
		[[nodiscard]] float Distance(const Vec2& v) const noexcept;
		[[nodiscard]] constexpr float DistanceSq(const Vec2& v) const noexcept;

		[[nodiscard]] constexpr Vec2 Lerp(const Vec2& b, float t) const noexcept;

		[[nodiscard]] constexpr Vec2 Reflect(const Vec2& v) const noexcept;
	};

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

		constexpr Vec3& operator++() noexcept;
		constexpr Vec3 operator++(int) noexcept;
		constexpr Vec3& operator--() noexcept;
		constexpr Vec3 operator--(int) noexcept;

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
		[[nodiscard]] constexpr bool IsNearZero(float epsilon = MATH_EPSILON) const noexcept;

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

		constexpr Vec4& operator++() noexcept;
		constexpr Vec4 operator++(int) noexcept;
		constexpr Vec4& operator--() noexcept;
		constexpr Vec4 operator--(int) noexcept;

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
		[[nodiscard]] constexpr bool IsNearZero(float epsilon = MATH_EPSILON) const noexcept;

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

	struct alignas(16) Vec3x3
	{
		union
		{
			float M[3][3];
			float Data[9];
			Vec3 Row[3];
		};

		constexpr Vec3x3() noexcept
		{
			std::fill_n(std::begin(Data), 9, 0.0f);
		}
		constexpr explicit Vec3x3(float v) noexcept
		{
			std::fill_n(std::begin(Data), 9, v);
		}

		constexpr static Vec3x3 Zero() noexcept;

		constexpr static Vec3x3 Identity() noexcept;
		constexpr void ToIdentity() noexcept;
		constexpr bool IsIdentity() const noexcept;

		friend constexpr Vec3x3 operator+(Vec3x3 l, const Vec3x3& r) noexcept
		{
			for (usize i : std::ranges::views::iota(0, 9))
			{
				l.Data[i] += r.Data[i];
			}

			return l;
		}
		friend constexpr Vec3x3 operator-(Vec3x3 l, const Vec3x3& r) noexcept
		{
			for (usize i : std::ranges::views::iota(0, 9))
			{
				l.Data[i] += r.Data[i];
			}

			return l;
		}

		constexpr Vec3x3& operator+=(const Vec3x3& v) noexcept;
		constexpr Vec3x3& operator-=(const Vec3x3& v) noexcept;

		friend constexpr Vec3x3 operator*(Vec3x3 l, const Vec3x3& r) noexcept
		{
			for (usize i : std::ranges::views::iota(0, 9))
			{
				l.Data[i] *= r.Data[i];
			}

			return l;
		}
		friend constexpr Vec3x3 operator*(Vec3x3 l, float v) noexcept
		{
			for (usize i : std::ranges::views::iota(0, 9))
			{
				l.Data[i] *= v;
			}

			return l;
		}

		constexpr Vec3x3& operator*=(const Vec3x3& v) noexcept;
		constexpr Vec3x3& operator*=(float v) noexcept;
	};

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

	constexpr Vec2 Vec2::operator+() const noexcept
	{
		return *this;
	}

	constexpr Vec2 Vec2::operator-() const noexcept
	{
		return Vec2{ -X,-Y };
	}

	constexpr Vec2& Vec2::operator++() noexcept
	{
		X++;
		Y++;

		return *this;
	}

	constexpr Vec2 Vec2::operator++(int) noexcept
	{
		Vec2 tmp = *this;

		X++;
		Y++;

		return tmp;
	}

	constexpr Vec2& Vec2::operator--() noexcept
	{
		X--;
		Y--;

		return *this;
	}

	constexpr Vec2 Vec2::operator--(int) noexcept
	{
		Vec2 tmp = *this;

		X--;
		Y--;

		return tmp;
	}

	constexpr Vec2& Vec2::operator+=(const Vec2& v) noexcept
	{
		X += v.X;
		Y += v.Y;

		return *this;
	}

	constexpr Vec2& Vec2::operator-=(const Vec2& v) noexcept
	{
		X -= v.X;
		Y -= v.Y;

		return *this;
	}

	constexpr Vec2& Vec2::operator*=(const Vec2& v) noexcept
	{
		X *= v.X;
		Y *= v.Y;

		return *this;
	}

	inline Vec2& Vec2::operator/=(const Vec2& v)
	{
		X /= v.X;
		Y /= v.Y;

		return *this;
	}

	constexpr Vec2& Vec2::operator+=(float v) noexcept
	{
		X += v;
		Y += v;
		return *this;
	}

	constexpr Vec2& Vec2::operator-=(float v) noexcept
	{
		X -= v;
		Y -= v;
		return *this;
	}

	constexpr Vec2& Vec2::operator*=(float v) noexcept
	{
		X *= v;
		Y *= v;
		return *this;
	}

	inline Vec2& Vec2::operator/=(float v)
	{
		X /= v;
		Y /= v;
		return *this;
	}

	inline float& Vec2::operator[](u8 index)
	{
		switch (index)
		{
		case 0:
			return X;
		case 1:
			return Y;
		default:
			throw std::out_of_range("Index只能为0或1");
		}
	}

	inline const float& Vec2::operator[](u8 index) const
	{
		switch (index)
		{
		case 0:
			return X;
		case 1:
			return Y;
		default:
			throw std::out_of_range("Index只能为0或1");
		}
	}

	constexpr bool Vec2::IsZero() const noexcept
	{
		return X == 0.0f && Y == 0.0f;
	}

	constexpr bool Vec2::IsNearZero(float epsilon) const noexcept
	{
		return NearAbs(X, 0.0f, epsilon) && NearAbs(Y, 0.0f, epsilon);
	}

	inline float Vec2::Angle(const Vec2& v) const noexcept
	{
		float len1 = Len();
		float len2 = v.Len();

		if (NearAbs(len1, 0.0f) || NearAbs(len2, 0.0f))
			return 0.0f;

		float cos = Dot(v) / (len1 * len2);
		cos = std::max(-1.0f, std::min(1.0f, cos));

		return acos(cos);
	}

	constexpr float Vec2::Dot(const Vec2& v) const noexcept
	{
		return X * v.X + Y * v.Y;
	}

	constexpr float Vec2::LenSq() const noexcept
	{
		return X * X + Y * Y;
	}

	inline float Vec2::Len() const noexcept
	{
		return std::sqrt(LenSq());
	}

	inline void Vec2::Normalize()
	{
		if (float len = Len(); !NearAbs(len, 0.0f))
			*this /= len;
	}

	inline Vec2 Vec2::Normalized() const
	{
		if (float len = Len(); !NearAbs(len, 0.0f))
			return *this / len;
		return Vec2(0);
	}

	inline float Vec2::Distance() const noexcept
	{
		return Len();
	}

	constexpr float Vec2::DistanceSq() const noexcept
	{
		return LenSq();
	}

	inline float Vec2::Distance(const Vec2& v) const noexcept
	{
		return std::sqrt(DistanceSq(v));
	}

	constexpr float Vec2::DistanceSq(const Vec2& v) const noexcept
	{
		return (X - v.X) * (X - v.X) + (Y - v.Y) * (Y - v.Y);
	}

	constexpr Vec2 Vec2::Lerp(const Vec2& b, float t) const noexcept
	{
		return Vec2(
			PenEngine::Lerp(X, b.X, t),
			PenEngine::Lerp(Y, b.Y, t)
		);
	}

	constexpr Vec2 Vec2::Reflect(const Vec2& v) const noexcept
	{
		return *this - v * (2.0f * Dot(v));
	}

	constexpr Vec3 Vec3::operator+() const noexcept
	{
		return *this;
	}

	constexpr Vec3 Vec3::operator-() const noexcept
	{
		return Vec3(-X, -Y, -Z);
	}

	constexpr Vec3& Vec3::operator++() noexcept
	{
		X++;
		Y++;
		Z++;

		return *this;
	}

	constexpr Vec3 Vec3::operator++(int) noexcept
	{
		Vec3 tmp = *this;

		X++;
		Y++;
		Z++;

		return tmp;
	}

	constexpr Vec3& Vec3::operator--() noexcept
	{
		X--;
		Y--;
		Z--;

		return *this;
	}

	constexpr Vec3 Vec3::operator--(int) noexcept
	{
		Vec3 tmp = *this;

		X--;
		Y--;
		Z--;

		return tmp;
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

	constexpr Vec4 Vec4::operator+() const noexcept
	{
		return *this;
	}

	constexpr Vec4 Vec4::operator-() const noexcept
	{
		return Vec4(-X, -Y, -Z, -W);
	}

	constexpr Vec4& Vec4::operator++() noexcept
	{
		X++;
		Y++;
		Z++;
		W++;

		return *this;
	}

	constexpr Vec4 Vec4::operator++(int) noexcept
	{
		Vec4 tmp = *this;

		X++;
		Y++;
		Z++;
		W++;

		return tmp;
	}

	constexpr Vec4& Vec4::operator--() noexcept
	{
		X--;
		Y--;
		Z--;
		W--;

		return *this;
	}

	constexpr Vec4 Vec4::operator--(int) noexcept
	{
		Vec4 tmp = *this;

		X--;
		Y--;
		Z--;
		W--;

		return tmp;
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

	constexpr Vec3x3 Vec3x3::Zero() noexcept
	{
		return Vec3x3();
	}

	constexpr Vec3x3 Vec3x3::Identity() noexcept
	{
		Vec3x3 res;
		res.ToIdentity();
		return res;
	}

	constexpr void Vec3x3::ToIdentity() noexcept
	{
		for (u8 i : std::ranges::views::iota(static_cast<u8>(0u), 3u))
			for (u8 j : std::ranges::views::iota(static_cast<u8>(0u), 3u))
				if (i == j)
					M[i][j] = 1.0f;
				else
					M[i][j] = 0.0f;
	}

	constexpr bool Vec3x3::IsIdentity() const noexcept
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

	constexpr Vec3x3& Vec3x3::operator+=(const Vec3x3& v) noexcept
	{
		for (usize i : std::ranges::views::iota(0, 9))
		{
			Data[i] += v.Data[i];
		}

		return *this;
	}

	constexpr Vec3x3& Vec3x3::operator-=(const Vec3x3& v) noexcept
	{
		for (usize i : std::ranges::views::iota(0, 9))
		{
			Data[i] -= v.Data[i];
		}

		return *this;
	}

	constexpr Vec3x3& Vec3x3::operator*=(const Vec3x3& v) noexcept
	{
		for (u8 i : std::ranges::views::iota(static_cast<u8>(0u), static_cast<u8>(9u)))
		{
			Data[i] *= v.Data[i];
		}

		return *this;
	}

	constexpr Vec3x3& Vec3x3::operator*=(float v) noexcept
	{
		for (u8 i : std::ranges::views::iota(static_cast<u8>(0u), static_cast<u8>(9u)))
		{
			Data[i] *= v;
		}

		return *this;
	}

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
