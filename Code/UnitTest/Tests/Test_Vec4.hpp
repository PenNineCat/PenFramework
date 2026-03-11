// File /UnitTest/Tests/Test_Vec4.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "../../Engine/Math/Vec4.hpp"
#include "../UnitTestFramework.h"

namespace PenFramework::UnitTest::Math
{
	// 测试 Vec4 四维向量结构体
	UNIT_TEST_AREA_BEGIN(TestVec4)
	{
		using namespace PenEngine;

		UNIT_TEST_MESSAGE("测试 Vec4 四维向量结构体")

		// --- 构造函数 ---
			UNIT_TEST_CHECKPOINT("测试构造函数")
		{
			Vec4 v0;
			UNIT_TEST_CONDITION("默认构造应为 (0,0,0,0)", v0.X == 0.0f && v0.Y == 0.0f && v0.Z == 0.0f && v0.W == 0.0f)

				Vec4 v1(2.0f);
			UNIT_TEST_CONDITION("单参数构造应为 (2,2,2,2)", v1.X == 2.0f && v1.Y == 2.0f && v1.Z == 2.0f && v1.W == 2.0f)

				Vec4 v2(1.0f, -1.0f, 2.0f, 0.5f);
			UNIT_TEST_CONDITION("四参数构造正确", v2.X == 1.0f && v2.Y == -1.0f && v2.Z == 2.0f && v2.W == 0.5f)
		}

		// --- 算术运算符（向量-向量）---
		UNIT_TEST_CHECKPOINT("测试向量-向量算术运算")
		{
			Vec4 a(2.0f, 3.0f, -1.0f, 4.0f);
			Vec4 b(1.0f, -1.0f, 2.0f, 0.5f);

			UNIT_TEST_CONDITION("a + b = (3,2,1,4.5)", (a + b) == Vec4(3.0f, 2.0f, 1.0f, 4.5f))
				UNIT_TEST_CONDITION("a - b = (1,4,-3,3.5)", (a - b) == Vec4(1.0f, 4.0f, -3.0f, 3.5f))
				UNIT_TEST_CONDITION("a * b = (2,-3,-2,2)", (a * b) == Vec4(2.0f, -3.0f, -2.0f, 2.0f))
				UNIT_TEST_CONDITION("a / b ≈ (2,-3,-0.5,8)",
					NearAbs((a / b).X, 2.0f) &&
					NearAbs((a / b).Y, -3.0f) &&
					NearAbs((a / b).Z, -0.5f) &&
					NearAbs((a / b).W, 8.0f))
		}

		// --- 算术运算符（向量-标量）---
		UNIT_TEST_CHECKPOINT("测试向量-标量算术运算")
		{
			Vec4 v(4.0f, -8.0f, 12.0f, 2.0f);
			float s = 2.0f;

			UNIT_TEST_CONDITION("v + s = (6,-6,14,4)", (v + s) == Vec4(6.0f, -6.0f, 14.0f, 4.0f))
				UNIT_TEST_CONDITION("v - s = (2,-10,10,0)", (v - s) == Vec4(2.0f, -10.0f, 10.0f, 0.0f))
				UNIT_TEST_CONDITION("v * s = (8,-16,24,4)", (v * s) == Vec4(8.0f, -16.0f, 24.0f, 4.0f))
				UNIT_TEST_CONDITION("v / s = (2,-4,6,1)", (v / s) == Vec4(2.0f, -4.0f, 6.0f, 1.0f))
		}

		// --- 复合赋值与自增自减 ---
		UNIT_TEST_CHECKPOINT("测试复合赋值与自增/自减")
		{
			Vec4 v(1.0f, 1.0f, 1.0f, 1.0f);
			v += Vec4(0.0f, 1.0f, 2.0f, 3.0f);
			UNIT_TEST_CONDITION("+= 向量", v == Vec4(1.0f, 2.0f, 3.0f, 4.0f))

				v *= 0.5f;
			UNIT_TEST_CONDITION("*= 标量", v == Vec4(0.5f, 1.0f, 1.5f, 2.0f))
		}

		// --- 下标访问 ---
		UNIT_TEST_CHECKPOINT("测试下标 operator[]")
		{
			Vec4 v(10.0f, 20.0f, 30.0f, 40.0f);
			UNIT_TEST_CONDITION("下标读取",
				v[0] == 10.0f && v[1] == 20.0f && v[2] == 30.0f && v[3] == 40.0f)

				v[2] = -5.0f;
			v[3] = 0.0f;
			UNIT_TEST_CONDITION("下标写入", v == Vec4(10.0f, 20.0f, -5.0f, 0.0f))

				const Vec4 cv(1.0f, 2.0f, 3.0f, 4.0f);
			UNIT_TEST_CONDITION("const 下标", cv[3] == 4.0f)
		}

		// --- 零向量判断 ---
		UNIT_TEST_CHECKPOINT("测试 IsZero 与 IsNearZero")
		{
			Vec4 zero;
			Vec4 nearZero(MathEpsilon / 2, -MathEpsilon / 3, MathEpsilon / 4, -MathEpsilon / 5);
			Vec4 notZero(0.0f, 0.0f, 0.0f, 0.1f);

			UNIT_TEST_CONDITION("IsZero", zero.IsZero())
				UNIT_TEST_CONDITION("!IsZero", !notZero.IsZero())
				UNIT_TEST_CONDITION("IsNearZero (default)", nearZero.IsNearZero())
				UNIT_TEST_CONDITION("!IsNearZero for 0.1", !notZero.IsNearZero())
		}

		// --- 长度与距离 ---
		UNIT_TEST_CHECKPOINT("测试 Len, LenSq, Distance")
		{
			Vec4 v(1.0f, 2.0f, 2.0f, 1.0f); // 1+4+4+1 = 10 → len = sqrt(10)
			UNIT_TEST_CONDITION("LenSq = 10", v.LenSq() == 10.0f)
				UNIT_TEST_CONDITION("Len = sqrt(10)", NearAbs(v.Len(), std::sqrt(10.0f)))

				Vec4 a(0.0f, 0.0f, 0.0f, 0.0f);
			Vec4 b(3.0f, 0.0f, 4.0f, 0.0f); // 在 XZ 平面，距离 5
			UNIT_TEST_CONDITION("DistanceSq = 25", a.DistanceSq(b) == 25.0f)
				UNIT_TEST_CONDITION("Distance = 5", NearAbs(a.Distance(b), 5.0f))

				Vec4 c(0.0f, 0.0f, 0.0f, 7.0f);
			UNIT_TEST_CONDITION("到原点距离 = 7", NearAbs(c.Distance(), 7.0f))
		}

		// --- 点积 ---
		UNIT_TEST_CHECKPOINT("测试 Dot 积")
		{
			Vec4 a(1.0f, 2.0f, 3.0f, 4.0f);
			Vec4 b(2.0f, 3.0f, 4.0f, 5.0f);
			// 1*2 + 2*3 + 3*4 + 4*5 = 2 + 6 + 12 + 20 = 40
			UNIT_TEST_CONDITION("Dot = 40", a.Dot(b) == 40.0f)

				Vec4 e1(1.0f, 0.0f, 0.0f, 0.0f);
			Vec4 e2(0.0f, 1.0f, 0.0f, 0.0f);
			Vec4 e3(0.0f, 0.0f, 1.0f, 0.0f);
			Vec4 e4(0.0f, 0.0f, 0.0f, 1.0f);
			UNIT_TEST_CONDITION("标准基两两点积为 0",
				NearAbs(e1.Dot(e2), 0.0f) &&
				NearAbs(e1.Dot(e3), 0.0f) &&
				NearAbs(e1.Dot(e4), 0.0f) &&
				NearAbs(e2.Dot(e3), 0.0f) &&
				NearAbs(e2.Dot(e4), 0.0f) &&
				NearAbs(e3.Dot(e4), 0.0f))
		}

		// --- 角度（弧度）---
		UNIT_TEST_CHECKPOINT("测试 Angle 函数")
		{
			Vec4 x(1.0f, 0.0f, 0.0f, 0.0f);
			Vec4 y(0.0f, 1.0f, 0.0f, 0.0f);
			Vec4 w_axis(0.0f, 0.0f, 0.0f, 1.0f);

			float angle90 = x.Angle(y);
			UNIT_TEST_CONDITION("90度 = π/2", NearAbs(angle90, static_cast<float>(std::numbers::pi / 2.0)))

				Vec4 diag(1.0f, 1.0f, 0.0f, 0.0f);
			float angle45 = x.Angle(diag);
			UNIT_TEST_CONDITION("45度 ≈ π/4", NearAbs(angle45, static_cast<float>(std::numbers::pi / 4.0)))

				UNIT_TEST_CONDITION("相同向量夹角为 0", NearAbs(x.Angle(x), 0.0f))

				Vec4 neg_x(-1.0f, 0.0f, 0.0f, 0.0f);
			UNIT_TEST_CONDITION("反向夹角为 π", NearAbs(x.Angle(neg_x), static_cast<float>(std::numbers::pi)))
		}

		// --- 归一化 ---
		UNIT_TEST_CHECKPOINT("测试 Normalize 与 Normalized")
		{
			Vec4 v(0.0f, 0.0f, 3.0f, 4.0f); // len = 5
			Vec4 n = v.Normalized();
			UNIT_TEST_CONDITION("归一化后长度 ≈ 1", NearAbs(n.Len(), 1.0f))
				UNIT_TEST_CONDITION("方向正确", NearAbs(n.Z, 0.6f) && NearAbs(n.W, 0.8f))

				v.Normalize();
			UNIT_TEST_CONDITION("原地归一化", NearAbs(v.Len(), 1.0f))

				Vec4 zero;
			Vec4 zero_norm = zero.Normalized();
			UNIT_TEST_CONDITION("零向量归一化安全",
				zero_norm.IsZero() ||
				(std::isnan(zero_norm.X) && std::isnan(zero_norm.Y) &&
					std::isnan(zero_norm.Z) && std::isnan(zero_norm.W)))
		}

		// --- Lerp 插值 ---
		UNIT_TEST_CHECKPOINT("测试 Vec4::Lerp")
		{
			Vec4 a(0.0f, 0.0f, 0.0f, 0.0f);
			Vec4 b(4.0f, 8.0f, 12.0f, 16.0f);

			Vec4 mid = a.Lerp(b, 0.5f);
			UNIT_TEST_CONDITION("Lerp 0.5 = (2,4,6,8)", mid == Vec4(2.0f, 4.0f, 6.0f, 8.0f))

				Vec4 start = a.Lerp(b, 0.0f);
			Vec4 end = a.Lerp(b, 1.0f);
			UNIT_TEST_CONDITION("t=0 返回起点", start == a)
				UNIT_TEST_CONDITION("t=1 返回终点", end == b)

				Vec4 ext = a.Lerp(b, 2.0f);
			UNIT_TEST_CONDITION("支持外插", ext == Vec4(8.0f, 16.0f, 24.0f, 32.0f))
		}

		// --- Reflect 反射 ---
		UNIT_TEST_CHECKPOINT("测试 Reflect 函数")
		{
			// 入射向量：沿 W 轴负方向，法线为 W 轴正方向
			Vec4 incident(0.0f, 0.0f, 0.0f, -2.0f);
			Vec4 normal(0.0f, 0.0f, 0.0f, 1.0f); // 单位法线

			// R = I - 2 * dot(I, N) * N
			// dot = (0)(0)+...+(-2)(1) = -2
			// R = (0,0,0,-2) - 2*(-2)*(0,0,0,1) = (0,0,0,-2) + (0,0,0,4) = (0,0,0,2)
			Vec4 reflected = incident.Reflect(normal);
			UNIT_TEST_CONDITION("W轴反射应为 (0,0,0,2)", reflected == Vec4(0.0f, 0.0f, 0.0f, 2.0f))

			// 斜入射
				Vec4 slant(1.0f, 0.0f, 0.0f, -1.0f);
			Vec4 refl_slant = slant.Reflect(normal);
			UNIT_TEST_CONDITION("斜入射反射应为 (1,0,0,1)", refl_slant == Vec4(1.0f, 0.0f, 0.0f, 1.0f))
		}

		// --- 比较运算符 ---
		UNIT_TEST_CHECKPOINT("测试 operator==（基于 NearAbs）")
		{
			Vec4 a(1.0f, 2.0f, 3.0f, 4.0f);
			Vec4 b(1.0f + MathEpsilon / 2, 2.0f - MathEpsilon / 3,
				3.0f + MathEpsilon / 4, 4.0f - MathEpsilon / 5);
			Vec4 c(1.0f, 2.0f, 3.0f, 4.1f);

			UNIT_TEST_CONDITION("近似相等", a == b)
				UNIT_TEST_CONDITION("差异过大不等", !(a == c))
		}

		UNIT_TEST_MESSAGE("Vec4 所有测试通过")
			UNIT_TEST_CHECKPOINT("Vec4 单元测试完成")
	}
	UNIT_TEST_AREA_END(TestVec4)
}