// File /UnitTest/Tests/Test_Vec3.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "../../Engine/Math/Vec3.hpp"
#include "../UnitTestFramework.h"

namespace PenFramework::UnitTest
{
	// 测试 Vec3 三维向量结构体
	UNIT_TEST_AREA_BEGIN(TestVec3)
	{
		using namespace PenEngine;

		UNIT_TEST_MESSAGE("测试 Vec3 三维向量结构体")

		// --- 构造函数 ---
			UNIT_TEST_CHECKPOINT("测试构造函数")
		{
			Vec3 v0;
			UNIT_TEST_CONDITION("默认构造应为 (0,0,0)", v0.X == 0.0f && v0.Y == 0.0f && v0.Z == 0.0f)

				Vec3 v1(5.0f);
			UNIT_TEST_CONDITION("单参数构造应为 (5,5,5)", v1.X == 5.0f && v1.Y == 5.0f && v1.Z == 5.0f)

				Vec3 v2(1.0f, -2.0f, 3.0f);
			UNIT_TEST_CONDITION("三参数构造应为 (1,-2,3)", v2.X == 1.0f && v2.Y == -2.0f && v2.Z == 3.0f)
		}

		// --- 算术运算符（向量-向量）---
		UNIT_TEST_CHECKPOINT("测试向量-向量算术运算")
		{
			Vec3 a(2.0f, 3.0f, -1.0f);
			Vec3 b(1.0f, -1.0f, 4.0f);

			UNIT_TEST_CONDITION("a + b = (3,2,3)", (a + b) == Vec3(3.0f, 2.0f, 3.0f))
				UNIT_TEST_CONDITION("a - b = (1,4,-5)", (a - b) == Vec3(1.0f, 4.0f, -5.0f))
				UNIT_TEST_CONDITION("a * b = (2,-3,-4)", (a * b) == Vec3(2.0f, -3.0f, -4.0f))
				UNIT_TEST_CONDITION("a / b ≈ (2,-3,-0.25)",
									NearAbs((a / b).X, 2.0f) &&
									NearAbs((a / b).Y, -3.0f) &&
									NearAbs((a / b).Z, -0.25f))
		}

		// --- 算术运算符（向量-标量）---
		UNIT_TEST_CHECKPOINT("测试向量-标量算术运算")
		{
			Vec3 v(6.0f, -9.0f, 3.0f);
			float s = 3.0f;

			UNIT_TEST_CONDITION("v + s = (9,-6,6)", (v + s) == Vec3(9.0f, -6.0f, 6.0f))
				UNIT_TEST_CONDITION("v - s = (3,-12,0)", (v - s) == Vec3(3.0f, -12.0f, 0.0f))
				UNIT_TEST_CONDITION("v * s = (18,-27,9)", (v * s) == Vec3(18.0f, -27.0f, 9.0f))
				UNIT_TEST_CONDITION("v / s = (2,-3,1)", (v / s) == Vec3(2.0f, -3.0f, 1.0f))
		}

		// --- 复合赋值与自增自减 ---
		UNIT_TEST_CHECKPOINT("测试复合赋值与自增/自减")
		{
			Vec3 v(1.0f, 1.0f, 1.0f);
			v += Vec3(1.0f, 2.0f, 3.0f);
			UNIT_TEST_CONDITION("+= 向量", v == Vec3(2.0f, 3.0f, 4.0f))

				v *= 2.0f;
			UNIT_TEST_CONDITION("*= 标量", v == Vec3(4.0f, 6.0f, 8.0f))
		}

		// --- 下标访问 ---
		UNIT_TEST_CHECKPOINT("测试下标 operator[]")
		{
			Vec3 v(10.0f, 20.0f, 30.0f);
			UNIT_TEST_CONDITION("v[0]=10, v[1]=20, v[2]=30",
								v[0] == 10.0f && v[1] == 20.0f && v[2] == 30.0f)

				v[1] = -5.0f;
			UNIT_TEST_CONDITION("可写入", v == Vec3(10.0f, -5.0f, 30.0f))

				const Vec3 cv(1.0f, 2.0f, 3.0f);
			UNIT_TEST_CONDITION("const 下标", cv[2] == 3.0f)
		}

		// --- 零向量判断 ---
		UNIT_TEST_CHECKPOINT("测试 IsZero 与 IsNearZero")
		{
			Vec3 zero;
			Vec3 nearZero(MathEpsilon / 2.0f, -MathEpsilon / 3.0f, MathEpsilon / 4.0f);
			Vec3 notZero(0.0f, 0.0f, 0.1f);

			UNIT_TEST_CONDITION("IsZero", zero.IsZero())
				UNIT_TEST_CONDITION("!IsZero", !notZero.IsZero())
				UNIT_TEST_CONDITION("IsNearZero (default)", nearZero.IsNearZero())
				UNIT_TEST_CONDITION("!IsNearZero for 0.1", !notZero.IsNearZero())
		}

		// --- 长度与距离 ---
		UNIT_TEST_CHECKPOINT("测试 Len, LenSq, Distance")
		{
			Vec3 v(1.0f, 2.0f, 2.0f); // 1²+2²+2² = 9 → len=3
			UNIT_TEST_CONDITION("LenSq = 9", v.LenSq() == 9.0f);
			UNIT_TEST_CONDITION("Len = 3", NearAbs(v.Len(), 3.0f))

				Vec3 a(0.0f, 0.0f, 0.0f);
			Vec3 b(3.0f, 4.0f, 0.0f); // 在 XY 平面，距离 5
			UNIT_TEST_CONDITION("DistanceSq = 25", a.DistanceSq(b) == 25.0f)
				UNIT_TEST_CONDITION("Distance = 5", NearAbs(a.Distance(b), 5.0f))

				Vec3 c(0.0f, 0.0f, 5.0f);
			UNIT_TEST_CONDITION("到原点距离 = 5", NearAbs(c.Distance(), 5.0f))
		}

		// --- 点积 ---
		UNIT_TEST_CHECKPOINT("测试 Dot 积")
		{
			Vec3 a(1.0f, 2.0f, 3.0f);
			Vec3 b(4.0f, 5.0f, 6.0f);
			// 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
			UNIT_TEST_CONDITION("Dot = 32", a.Dot(b) == 32.0f)

				Vec3 x(1.0f, 0.0f, 0.0f);
			Vec3 y(0.0f, 1.0f, 0.0f);
			Vec3 z(0.0f, 0.0f, 1.0f);
			UNIT_TEST_CONDITION("正交基点积为 0",
								NearAbs(x.Dot(y), 0.0f) &&
								NearAbs(y.Dot(z), 0.0f) &&
								NearAbs(x.Dot(z), 0.0f))
		}

		// --- 角度（弧度）---
		UNIT_TEST_CHECKPOINT("测试 Angle 函数")
		{
			Vec3 right(1.0f, 0.0f, 0.0f);
			Vec3 up(0.0f, 1.0f, 0.0f);
			Vec3 forward(0.0f, 0.0f, 1.0f);

			float angle_xy = right.Angle(up);
			UNIT_TEST_CONDITION("XY 平面 90° = π/2", NearAbs(angle_xy, static_cast<float>(std::numbers::pi / 2)))

				Vec3 diag(1.0f, 1.0f, 0.0f);
			float angle45 = right.Angle(diag);
			UNIT_TEST_CONDITION("45° ≈ π/4", NearAbs(angle45, static_cast<float>(std::numbers::pi / 4)))

			// 相同向量
				UNIT_TEST_CONDITION("相同向量夹角为 0", NearAbs(right.Angle(right), 0.0f));

				// 反向
			Vec3 left(-1.0f, 0.0f, 0.0f);
			UNIT_TEST_CONDITION("反向夹角为 π", NearAbs(right.Angle(left), static_cast<float>(std::numbers::pi)))
		}

		// --- 归一化 ---
		UNIT_TEST_CHECKPOINT("测试 Normalize 与 Normalized")
		{
			Vec3 v(0.0f, 3.0f, 4.0f); // len = 5
			Vec3 n = v.Normalized();
			UNIT_TEST_CONDITION("归一化后长度 ≈ 1", NearAbs(n.Len(), 1.0f))
				UNIT_TEST_CONDITION("方向正确", NearAbs(n.Y, 0.6f) && NearAbs(n.Z, 0.8f))

				v.Normalize();
			UNIT_TEST_CONDITION("原地归一化", NearAbs(v.Len(), 1.0f))

			// 零向量安全
				Vec3 zero;
			Vec3 zero_norm = zero.Normalized();
			UNIT_TEST_CONDITION("零向量归一化安全",
								zero_norm.IsZero() || (std::isnan(zero_norm.X) && std::isnan(zero_norm.Y) && std::isnan(zero_norm.Z)))
		}

		// --- Lerp 插值 ---
		UNIT_TEST_CHECKPOINT("测试 Vec3::Lerp")
		{
			Vec3 a(0.0f, 0.0f, 0.0f);
			Vec3 b(2.0f, 4.0f, 6.0f);

			Vec3 mid = a.Lerp(b, 0.5f);
			UNIT_TEST_CONDITION("Lerp 0.5 = (1,2,3)", mid == Vec3(1.0f, 2.0f, 3.0f))

				Vec3 start = a.Lerp(b, 0.0f);
			Vec3 end = a.Lerp(b, 1.0f);
			UNIT_TEST_CONDITION("t=0 返回起点", start == a)
				UNIT_TEST_CONDITION("t=1 返回终点", end == b)

				Vec3 ext = a.Lerp(b, -1.0f);
			UNIT_TEST_CONDITION("支持外插 (t=-1)", ext == Vec3(-2.0f, -4.0f, -6.0f))
		}

		// --- Reflect 反射 ---
		UNIT_TEST_CHECKPOINT("测试 Reflect 函数")
		{
			// 入射向量：从上方斜射向 XY 平面（法线为 Z 轴）
			Vec3 incident(1.0f, 1.0f, -1.0f);
			Vec3 normal(0.0f, 0.0f, 1.0f); // 平面法线向上

			// R = I - 2 * dot(I, N) * N
			// dot = (1)(0)+(1)(0)+(-1)(1) = -1
			// R = (1,1,-1) - 2*(-1)*(0,0,1) = (1,1,-1) + (0,0,2) = (1,1,1)
			Vec3 reflected = incident.Reflect(normal);
			UNIT_TEST_CONDITION("反射向量应为 (1,1,1)", reflected == Vec3(1.0f, 1.0f, 1.0f))

			// 垂直入射
				Vec3 down(0.0f, 0.0f, -5.0f);
			Vec3 up_ref = down.Reflect(normal);
			UNIT_TEST_CONDITION("垂直反射为 (0,0,5)", up_ref == Vec3(0.0f, 0.0f, 5.0f))
		}

		// --- 比较运算符 ---
		UNIT_TEST_CHECKPOINT("测试 operator==（基于 NearAbs）")
		{
			Vec3 a(1.0f, 2.0f, 3.0f);
			Vec3 b(1.0f + MathEpsilon / 2, 2.0f - MathEpsilon / 3, 3.0f + MathEpsilon / 4);
			Vec3 c(1.0f, 2.1f, 3.0f);

			UNIT_TEST_CONDITION("近似相等", a == b)
				UNIT_TEST_CONDITION("差异过大不等", !(a == c))
		}

		UNIT_TEST_MESSAGE("Vec3 所有测试通过")
			UNIT_TEST_CHECKPOINT("Vec3 单元测试完成")
	}
	UNIT_TEST_AREA_END(TestVec3)
}