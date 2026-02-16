// File /Engine/Math/Vec2.hpp
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
    /// @brief 二维浮点向量结构体
    struct Vec2
    {
        float X = 0; ///< X 分量（水平方向）
        float Y = 0; ///< Y 分量（垂直方向）

        /// @brief 默认构造函数，初始化为 (0, 0)
        constexpr Vec2() noexcept = default;

        /// @brief 标量构造函数，将两个分量都设为给定值
        /// @param v 用于初始化 X 和 Y 的值
        constexpr explicit Vec2(float v) noexcept : X(v), Y(v) {}

        /// @brief 双参数构造函数，分别指定 X 和 Y 分量
        /// @param x X 分量
        /// @param y Y 分量
        constexpr Vec2(float x, float y) noexcept : X(x), Y(y) {}

        /// @brief 一元正号运算符（返回自身副本）
        constexpr Vec2 operator+() const noexcept;

        /// @brief 一元负号运算符（返回反方向向量）
        constexpr Vec2 operator-() const noexcept;

        /// @brief 向量加法赋值运算符（逐分量相加）
        /// @param v 要加的向量
        constexpr Vec2& operator+=(const Vec2& v) noexcept;

        /// @brief 向量减法赋值运算符（逐分量相减）
        /// @param v 要减的向量
        constexpr Vec2& operator-=(const Vec2& v) noexcept;

        /// @brief 向量乘法赋值运算符（逐分量相乘，Hadamard 积）
        /// @param v 要乘的向量
        constexpr Vec2& operator*=(const Vec2& v) noexcept;

        /// @brief 向量除法赋值运算符（逐分量相除）
        /// @param v 除数向量
        /// @note 若任一分量为 0，结果依照编译器计算
        Vec2& operator/=(const Vec2& v);

        /// @brief 标量加法赋值运算符（X 和 Y 同时加上标量 v）
        /// @param v 要加的标量
        constexpr Vec2& operator+=(float v) noexcept;

        /// @brief 标量减法赋值运算符（X 和 Y 同时减去标量 v）
        /// @param v 要减的标量
        constexpr Vec2& operator-=(float v) noexcept;

        /// @brief 标量乘法赋值运算符（X 和 Y 同时乘以标量 v）
        /// @param v 要乘的标量
        constexpr Vec2& operator*=(float v) noexcept;

        /// @brief 标量除法赋值运算符（X 和 Y 同时除以标量 v）
        /// @note 若 v 为 0，可能导致未定义行为
        /// @param v 除数标量
        Vec2& operator/=(float v);

        // --- 友元运算符（全局二元运算符） ---

        /// @brief 向量加法（逐分量相加）
        /// @param l 左操作数
        /// @param r 右操作数
        friend constexpr Vec2 operator+(Vec2 l, const Vec2& r) noexcept
        {
            return l += r;
        }

        /// @brief 向量减法（逐分量相减）
        /// @param l 左操作数
        /// @param r 右操作数
        friend constexpr Vec2 operator-(Vec2 l, const Vec2& r) noexcept
        {
            return l -= r;
        }

        /// @brief 向量逐分量乘法（Hadamard 积）
        /// @param l 左操作数
        /// @param r 右操作数
        friend constexpr Vec2 operator*(Vec2 l, const Vec2& r) noexcept
        {
            return l *= r;
        }

        /// @brief 向量逐分量除法
        /// @param l 左操作数
        /// @param r 右操作数
        friend Vec2 operator/(Vec2 l, const Vec2& r)
        {
            return l /= r;
        }

        /// @brief 向量与标量加法（X 和 Y 同时加标量）
        /// @param l 向量
        /// @param r 标量
        friend constexpr Vec2 operator+(Vec2 l, float r) noexcept
        {
            return l += r;
        }

        /// @brief 向量与标量减法（X 和 Y 同时减标量）
        /// @param l 向量
        /// @param r 标量
        friend constexpr Vec2 operator-(Vec2 l, float r) noexcept
        {
            return l -= r;
        }

        /// @brief 向量与标量乘法（缩放）
        /// @param l 向量
        /// @param r 标量
        friend constexpr Vec2 operator*(Vec2 l, float r) noexcept
        {
            return l *= r;
        }

        /// @brief 向量与标量除法（缩放）
        /// @param l 向量
        /// @param r 标量（非零）
        friend Vec2 operator/(Vec2 l, float r)
        {
            return l /= r;
        }

        /// @brief 向量相等比较（使用近似相等判断）
        /// @param l 左操作数
        /// @param r 右操作数
        /// @return 若 X 和 Y 分量均在容差范围内相等，则返回 true
        friend constexpr bool operator==(const Vec2& l, const Vec2& r)
        {
            return NearAbs(l.X, r.X) && NearAbs(l.Y, r.Y);
        }

        /// @brief 下标访问（非 const 版本）
        /// @param index 索引（0 表示 X，1 表示 Y）
        /// @exception std::out_of_range 若 index > 1，则抛出该错误
        float& operator[](i32 index);

        /// @brief 下标访问（const 版本）
        /// @param index 索引（0 表示 X，1 表示 Y）
		/// @exception std::out_of_range 若 index > 1，则抛出该错误
        const float& operator[](i32 index) const;

        /// @brief 判断是否为零向量
        [[nodiscard]] constexpr bool IsZero() const noexcept;

        /// @brief 判断是否近似为零向量（使用容差 epsilon）
        /// @param epsilon 容差，默认为 MathEpsilon
        [[nodiscard]] constexpr bool IsNearZero(float epsilon = MathEpsilon) const noexcept;

        /// @brief 计算当前向量与另一向量之间的夹角（弧度）
        /// @param v 目标向量
        [[nodiscard]] float Angle(const Vec2& v) const noexcept;

        /// @brief 计算与另一向量的点积（内积）
        /// @param v 另一向量
        [[nodiscard]] constexpr float Dot(const Vec2& v) const noexcept;

        /// @brief 返回向量长度的平方（避免开方，性能更高）
        [[nodiscard]] constexpr float LenSq() const noexcept;

        /// @brief 返回向量的实际长度（欧几里得范数）
        [[nodiscard]] float Len() const noexcept;

        /// @brief 将当前向量归一化（单位化）
        /// @note 若向量为零向量，不会进行操作
        void Normalize();

        /// @brief 返回当前向量的归一化副本（不修改原向量）
        /// @return 单位向量；
        /// @note 若原向量为零，则返回 (0, 0)
        [[nodiscard]] Vec2 Normalized() const;

        /// @brief 计算从原点到该点的距离（即向量长度）
        [[nodiscard]] float Distance() const noexcept;

        /// @brief 计算从原点到该点距离的平方
        [[nodiscard]] constexpr float DistanceSq() const noexcept;

        /// @brief 计算到另一点的欧几里得距离
        /// @param v 目标点
        [[nodiscard]] float Distance(const Vec2& v) const noexcept;

        /// @brief 计算到另一点距离的平方（避免开方）
        /// @param v 目标点
        [[nodiscard]] constexpr float DistanceSq(const Vec2& v) const noexcept;

        /// @brief 在当前向量与目标向量之间进行线性插值
        /// @param b 目标向量
        /// @param t 插值参数，内部会对其进行0~1限制
        [[nodiscard]] constexpr Vec2 Lerp(const Vec2& b, float t) const noexcept;

        /// @brief 计算当前向量关于法线 v 的反射向量
        /// @param v 表面法线，应为单位向量
        [[nodiscard]] constexpr Vec2 ReflectWithUnit(const Vec2& v) const noexcept;

        /// @brief 计算当前向量关于法线 v 的反射向量
        /// @param v 表面法线，内部会自动将其单位化
        [[nodiscard]] constexpr Vec2 Reflect(const Vec2& v) const noexcept;
    };

	constexpr Vec2 Vec2::operator+() const noexcept
	{
		return *this;
	}

	constexpr Vec2 Vec2::operator-() const noexcept
	{
		return Vec2{ -X,-Y };
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

	inline float& Vec2::operator[](i32 index)
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

	inline const float& Vec2::operator[](i32 index) const
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

	constexpr Vec2 Vec2::ReflectWithUnit(const Vec2& v) const noexcept
	{
        return *this - v * (2.0f * Dot(v));
	}

	constexpr Vec2 Vec2::Reflect(const Vec2& v) const noexcept
	{
        return ReflectWithUnit(v.Normalized());
	}

}