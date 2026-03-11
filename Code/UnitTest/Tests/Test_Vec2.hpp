// File /UnitTest/Tests/Test_Vec2.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "../../Engine/Math/Vec2.hpp"
#include "../UnitTestFramework.h"

namespace PenFramework::UnitTest
{
	// 测试 Vec2 向量结构体
	UNIT_TEST_AREA_BEGIN(TestVec2)
	{
		using namespace PenEngine;

		UNIT_TEST_MESSAGE("测试 Vec2 二维向量结构体")

		// --- 构造函数 ---
			UNIT_TEST_CHECKPOINT("测试构造函数")
		{
			Vec2 v0;
			UNIT_TEST_CONDITION("默认构造应为 (0,0)", v0.X == 0.0f && v0.Y == 0.0f)

				Vec2 v1(5.0f);
			UNIT_TEST_CONDITION("单参数构造应为 (5,5)", v1.X == 5.0f && v1.Y == 5.0f)

				Vec2 v2(3.0f, -4.0f);
			UNIT_TEST_CONDITION("双参数构造应为 (3,-4)", v2.X == 3.0f && v2.Y == -4.0f)
		}

		// --- 算术运算符（向量-向量）---
		UNIT_TEST_CHECKPOINT("测试向量-向量算术运算")
		{
			Vec2 a(2.0f, 3.0f);
			Vec2 b(1.0f, -1.0f);

			UNIT_TEST_CONDITION("a + b = (3,2)", (a + b) == Vec2(3.0f, 2.0f))
				UNIT_TEST_CONDITION("a - b = (1,4)", (a - b) == Vec2(1.0f, 4.0f))
				UNIT_TEST_CONDITION("a * b = (2,-3)", (a * b) == Vec2(2.0f, -3.0f))
				UNIT_TEST_CONDITION("a / b = (2,-3)", NearAbs((a / b).X, 2.0f) && NearAbs((a / b).Y, -3.0f))
		}

		// --- 算术运算符（向量-标量）---
		UNIT_TEST_CHECKPOINT("测试向量-标量算术运算")
		{
			Vec2 v(4.0f, -6.0f);
			float s = 2.0f;

			UNIT_TEST_CONDITION("v + s = (6,-4)", (v + s) == Vec2(6.0f, -4.0f))
				UNIT_TEST_CONDITION("v - s = (2,-8)", (v - s) == Vec2(2.0f, -8.0f))
				UNIT_TEST_CONDITION("v * s = (8,-12)", (v * s) == Vec2(8.0f, -12.0f))
				UNIT_TEST_CONDITION("v / s = (2,-3)", (v / s) == Vec2(2.0f, -3.0f))
		}

		// --- 复合赋值运算符 ---
		UNIT_TEST_CHECKPOINT("测试复合赋值运算符")
		{
			Vec2 v(1.0f, 2.0f);
			v += Vec2(3.0f, 4.0f);
			UNIT_TEST_CONDITION("+= 向量", v == Vec2(4.0f, 6.0f))

				v -= 2.0f;
			UNIT_TEST_CONDITION("-= 标量", v == Vec2(2.0f, 4.0f))

				v *= Vec2(0.5f, 0.5f);
			UNIT_TEST_CONDITION("*= 向量", NearAbs(v.X, 1.0f) && NearAbs(v.Y, 2.0f))

				v /= 2.0f;
			UNIT_TEST_CONDITION("/= 标量", v == Vec2(0.5f, 1.0f))
		}

		// --- 下标访问 ---
		UNIT_TEST_CHECKPOINT("测试下标 operator[]")
		{
			Vec2 v(10.0f, 20.0f);
			UNIT_TEST_CONDITION("v[0] = 10", v[0] == 10.0f)
				UNIT_TEST_CONDITION("v[1] = 20", v[1] == 20.0f)

				v[0] = 5.0f;
			v[1] = -5.0f;
			UNIT_TEST_CONDITION("可写入下标", v == Vec2(5.0f, -5.0f))

				const Vec2 cv(1.0f, 2.0f);
			UNIT_TEST_CONDITION("const 下标", cv[0] == 1.0f && cv[1] == 2.0f)
		}

		// --- 零向量判断 ---
		UNIT_TEST_CHECKPOINT("测试 IsZero 与 IsNearZero")
		{
			Vec2 zero;
			Vec2 nearZero(MathEpsilon / 2.0f, -MathEpsilon / 3.0f);
			Vec2 notZero(0.1f, 0.0f);

			UNIT_TEST_CONDITION("IsZero", zero.IsZero())
				UNIT_TEST_CONDITION("!IsZero for non-zero", !notZero.IsZero())

				UNIT_TEST_CONDITION("IsNearZero (default epsilon)", nearZero.IsNearZero())
				UNIT_TEST_CONDITION("!IsNearZero for larger value", !notZero.IsNearZero())
				UNIT_TEST_CONDITION("IsNearZero with custom epsilon", Vec2(0.01f, 0.01f).IsNearZero(0.02f))
		}

		// --- 长度与平方长度 ---
		UNIT_TEST_CHECKPOINT("测试 Len, LenSq")
		{
			Vec2 v(3.0f, 4.0f);
			UNIT_TEST_CONDITION("LenSq = 25", v.LenSq() == 25.0f)
				UNIT_TEST_CONDITION("Len = 5", NearAbs(v.Len(), 5.0f))

				Vec2 zero;
			UNIT_TEST_CONDITION("零向量长度为 0", zero.Len() == 0.0f)
		}

		// --- 归一化 ---
		UNIT_TEST_CHECKPOINT("测试 Normalize 与 Normalized")
		{
			Vec2 v(3.0f, 4.0f);
			Vec2 n = v.Normalized();
			UNIT_TEST_CONDITION("Normalized 长度 ≈ 1", NearAbs(n.Len(), 1.0f))
				UNIT_TEST_CONDITION("方向一致", NearAbs(n.X, 0.6f) && NearAbs(n.Y, 0.8f))

				v.Normalize();
			UNIT_TEST_CONDITION("Normalize in-place", NearAbs(v.Len(), 1.0f))

			// 零向量归一化应安全（不崩溃，返回零向量或 NaN？）
				Vec2 zero;
			Vec2 zero_norm = zero.Normalized();
			UNIT_TEST_CONDITION("零向量归一化后仍为零或无效",
								(zero_norm.IsZero()) || (std::isnan(zero_norm.X) && std::isnan(zero_norm.Y)))
		}

		// --- 距离计算 ---
		UNIT_TEST_CHECKPOINT("测试 Distance 与 DistanceSq")
		{
			Vec2 a(1.0f, 1.0f);
			Vec2 b(4.0f, 5.0f); // delta = (3,4)

			UNIT_TEST_CONDITION("DistanceSq = 25", a.DistanceSq(b) == 25.0f)
				UNIT_TEST_CONDITION("Distance = 5", NearAbs(a.Distance(b), 5.0f))

				// 到原点
				Vec2 c(0.0f, -3.0f);
			UNIT_TEST_CONDITION("Distance to origin = 3", NearAbs(c.Distance(), 3.0f))
				UNIT_TEST_CONDITION("DistanceSq to origin = 9", c.DistanceSq() == 9.0f)
		}

		// --- 点积 ---
		UNIT_TEST_CHECKPOINT("测试 Dot 积")
		{
			Vec2 a(1.0f, 2.0f);
			Vec2 b(3.0f, 4.0f);
			UNIT_TEST_CONDITION("Dot = 1*3 + 2*4 = 11", a.Dot(b) == 11.0f)

				Vec2 perp(1.0f, 0.0f);
			Vec2 perp2(0.0f, 1.0f);
			UNIT_TEST_CONDITION("垂直向量点积为 0", NearAbs(perp.Dot(perp2), 0.0f))
		}

		// --- 角度（弧度）---
		UNIT_TEST_CHECKPOINT("测试 Angle 函数")
		{
			Vec2 right(1.0f, 0.0f);
			Vec2 up(0.0f, 1.0f);
			Vec2 diag(1.0f, 1.0f);

			float angle90 = right.Angle(up);
			UNIT_TEST_CONDITION("90度 = π/2 弧度", NearAbs(angle90, static_cast<float>(std::numbers::pi / 2)))

				float angle45 = right.Angle(diag);
			UNIT_TEST_CONDITION("45度 ≈ π/4", NearAbs(angle45, static_cast<float>(std::numbers::pi / 4)))

			// 相同向量夹角为 0
				UNIT_TEST_CONDITION("相同向量夹角为 0", NearAbs(right.Angle(right), 0.0f));

				// 反向向量夹角为 π
			Vec2 left(-1.0f, 0.0f);
			UNIT_TEST_CONDITION("反向向量夹角为 π", NearAbs(right.Angle(left), static_cast<float>(std::numbers::pi)))
		}

		// --- Lerp 插值 ---
		UNIT_TEST_CHECKPOINT("测试 Vec2::Lerp")
		{
			Vec2 a(0.0f, 0.0f);
			Vec2 b(10.0f, 20.0f);

			Vec2 mid = a.Lerp(b, 0.5f);
			UNIT_TEST_CONDITION("Lerp 0.5 = (5,10)", mid == Vec2(5.0f, 10.0f))

				Vec2 start = a.Lerp(b, 0.0f);
			Vec2 end = a.Lerp(b, 1.0f);
			UNIT_TEST_CONDITION("Lerp t=0 返回起点", start == a)
				UNIT_TEST_CONDITION("Lerp t=1 返回终点", end == b)

				Vec2 ext = a.Lerp(b, 2.0f);
			UNIT_TEST_CONDITION("Lerp 支持外插", ext == Vec2(20.0f, 40.0f))
		}

		// --- Reflect 反射 ---
		UNIT_TEST_CHECKPOINT("测试 Reflect 函数")
		{
			// 入射向量 I，法线 N（应为单位向量，但函数内部可能未归一化）
			Vec2 incident(1.0f, -1.0f);   // 从右上往左下射向 x 轴
			Vec2 normal(0.0f, 1.0f);      // y 轴向上为法线（地面）

			Vec2 reflected = incident.Reflect(normal);
			// 公式: R = I - 2 * dot(I, N) * N
			// dot = (1)(0) + (-1)(1) = -1
			// R = (1, -1) - 2*(-1)*(0,1) = (1, -1) + (0,2) = (1,1)
			UNIT_TEST_CONDITION("反射向量应为 (1,1)", reflected == Vec2(1.0f, 1.0f))

			// 垂直入射
				Vec2 down(0.0f, -1.0f);
			Vec2 up_ref = down.Reflect(normal);
			UNIT_TEST_CONDITION("垂直入射反射为 (0,1)", up_ref == Vec2(0.0f, 1.0f))
		}

		// --- 比较运算符（使用 NearAbs）---
		UNIT_TEST_CHECKPOINT("测试 operator==（基于 NearAbs）")
		{
			Vec2 a(1.0f, 2.0f);
			Vec2 b(1.0f + MathEpsilon / 2.0f, 2.0f - MathEpsilon / 3.0f);
			Vec2 c(1.1f, 2.0f);

			UNIT_TEST_CONDITION("近似相等应返回 true", a == b)
				UNIT_TEST_CONDITION("差异过大应返回 false", !(a == c))
		}

		UNIT_TEST_MESSAGE("Vec2 所有测试通过")
			UNIT_TEST_CHECKPOINT("Vec2 单元测试完成")
	}
	UNIT_TEST_AREA_END(TestVec2)
}