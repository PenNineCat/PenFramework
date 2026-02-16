// File /UnitTest/Tests/Test_Math.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "../../Engine/Math/MathFunction.hpp"
#include "../../Engine/Math/Vec2.hpp"
#include "../../Engine/Math/Vec3.hpp"
#include "../../Engine/Math/Vec4.hpp"
#include "../Core/UnitTestFramework.h"

#include <exception>
#include <numbers>
#include <stacktrace>

namespace PenFramework::UnitTest::Math
{
	using namespace PenEngine;

	// 测试 CeilPow2 及相关函数
	UNIT_TEST_AREA_BEGIN(TestCeilFunctions)
	{
		UNIT_TEST_MESSAGE("测试 CeilPow2 函数")

		// 测试无效输入（n 不是 2 的幂）
			UNIT_TEST_CONDITION("n=0 应返回 0", CeilPow2(10, 0) == 0)
			UNIT_TEST_CONDITION("n=1 应返回 0", CeilPow2(10, 1) == 0)
			UNIT_TEST_CONDITION("n=3 应返回 0", CeilPow2(10, 3) == 0)
			UNIT_TEST_CONDITION("n=6 应返回 0", CeilPow2(10, 6) == 0)

			// 测试有效输入
			UNIT_TEST_CHECKPOINT("测试有效的 2 的幂参数")
			UNIT_TEST_CONDITION("CeilPow2(0, 4) = 0", CeilPow2(0, 4) == 0)
			UNIT_TEST_CONDITION("CeilPow2(1, 4) = 1", CeilPow2(1, 4) == 1)
			UNIT_TEST_CONDITION("CeilPow2(2, 4) = 1", CeilPow2(2, 4) == 1)
			UNIT_TEST_CONDITION("CeilPow2(3, 4) = 1", CeilPow2(3, 4) == 1)
			UNIT_TEST_CONDITION("CeilPow2(4, 4) = 1", CeilPow2(4, 4) == 1)
			UNIT_TEST_CONDITION("CeilPow2(5, 4) = 2", CeilPow2(5, 4) == 2)

			// 测试特定 Ceil 函数
			UNIT_TEST_MESSAGE("测试特定对齐函数（Ceil4/8/16...）")
			UNIT_TEST_CONDITION("Ceil4(0) = 0", Ceil4(0) == 0)
			UNIT_TEST_CONDITION("Ceil4(1) = 1", Ceil4(1) == 1)
			UNIT_TEST_CONDITION("Ceil4(3) = 1", Ceil4(3) == 1)
			UNIT_TEST_CONDITION("Ceil4(4) = 1", Ceil4(4) == 1)
			UNIT_TEST_CONDITION("Ceil4(5) = 2", Ceil4(5) == 2)

			UNIT_TEST_CONDITION("Ceil8(7) = 1", Ceil8(7) == 1)
			UNIT_TEST_CONDITION("Ceil8(8) = 1", Ceil8(8) == 1)
			UNIT_TEST_CONDITION("Ceil8(9) = 2", Ceil8(9) == 2)

			UNIT_TEST_CONDITION("Ceil16(15) = 1", Ceil16(15) == 1)
			UNIT_TEST_CONDITION("Ceil16(16) = 1", Ceil16(16) == 1)
			UNIT_TEST_CONDITION("Ceil16(17) = 2", Ceil16(17) == 2)

			UNIT_TEST_CONDITION("Ceil32(31) = 1", Ceil32(31) == 1)
			UNIT_TEST_CONDITION("Ceil32(32) = 1", Ceil32(32) == 1)
			UNIT_TEST_CONDITION("Ceil32(33) = 2", Ceil32(33) == 2)

			UNIT_TEST_CONDITION("Ceil64(63) = 1", Ceil64(63) == 1)
			UNIT_TEST_CONDITION("Ceil64(64) = 1", Ceil64(64) == 1)
			UNIT_TEST_CONDITION("Ceil64(65) = 2", Ceil64(65) == 2)

			UNIT_TEST_CHECKPOINT("所有 Ceil 函数基础测试通过")
	}
	UNIT_TEST_AREA_END(TestCeilFunctions)
	// 测试 Abs 函数
	UNIT_TEST_AREA_BEGIN(TestAbsFunctions)
	{
		UNIT_TEST_MESSAGE("测试 Abs 函数（绝对值）对不同数据类型的支持")

		// 整数测试
			UNIT_TEST_CHECKPOINT("测试整数 Abs")
			UNIT_TEST_CONDITION("Abs(-42) = 42", Abs(-42) == 42)
			UNIT_TEST_CONDITION("Abs(-100) = 100", Abs(-100) == 100)
			UNIT_TEST_CONDITION("Abs(100) = 100", Abs(100) == 100)
			UNIT_TEST_CONDITION("Abs(0) = 0", Abs(0) == 0)

			// 浮点数测试
			UNIT_TEST_CHECKPOINT("测试浮点 Abs")
			UNIT_TEST_CONDITION("Abs(-3.14f) ≈ 3.14f", NearAbs(Abs(-3.14f), 3.14f))
			UNIT_TEST_CONDITION("Abs(-E) ≈ E", NearAbs(static_cast<float>(Abs(-E)), static_cast<float>(E)))

			// 边界与特殊值
			UNIT_TEST_MESSAGE("测试边界情况与特殊浮点值")
			UNIT_TEST_CONDITION("Abs(-0.0f) = 0.0f", Abs(-0.0f) == 0.0f)
			UNIT_TEST_CONDITION("Abs(-INFINITY) = INFINITY",
				Abs(-std::numeric_limits<float>::infinity()) ==
				std::numeric_limits<float>::infinity())

			UNIT_TEST_CHECKPOINT("所有 Abs 测试通过")
	}
	UNIT_TEST_AREA_END(TestAbsFunctions)

	// 测试 Lerp 函数
	UNIT_TEST_AREA_BEGIN(TestLerpFunction)
	{
		UNIT_TEST_MESSAGE("测试 Lerp（线性插值）函数")

		// 整数插值
			UNIT_TEST_CHECKPOINT("测试整数 Lerp")
			UNIT_TEST_CONDITION("Lerp(0, 100, 0.5f) = 50", Lerp(0, 100, 0.5f) == 50)

			// 浮点插值
			UNIT_TEST_CHECKPOINT("测试 float Lerp")
			UNIT_TEST_CONDITION("Lerp(0.0f, 1.0f, 0.3f) ≈ 0.3f", NearAbs(Lerp(0.0f, 1.0f, 0.3f), 0.3f))

			// 边界条件
			UNIT_TEST_MESSAGE("测试 Lerp 边界条件")
			UNIT_TEST_CONDITION("t=0 时返回起点", NearAbs(Lerp(10.0f, 20.0f, 0.0f), 10.0f))
			UNIT_TEST_CONDITION("t=1 时返回终点", NearAbs(Lerp(10.0f, 20.0f, 1.0f), 20.0f))
			UNIT_TEST_CONDITION("t=0.5 时返回中点", NearAbs(Lerp(0.0f, 100.0f, 0.5f), 50.0f))

			// 负值插值
			UNIT_TEST_CONDITION("支持负值插值", NearAbs(Lerp(-10.0f, 10.0f, 0.5f), 0.0f))

			UNIT_TEST_CHECKPOINT("所有 Lerp 测试通过")
	}
	UNIT_TEST_AREA_END(TestLerpFunction)
	// 测试 NearAbs 与 NearRel 函数
	UNIT_TEST_AREA_BEGIN(TestNearFunctions)
	{
		UNIT_TEST_MESSAGE("测试 NearAbs（绝对误差）与 NearRel（相对误差）比较函数")

		// --- NearAbs 测试 ---
			UNIT_TEST_CHECKPOINT("测试 NearAbs（绝对误差）")
			UNIT_TEST_CONDITION("1.0 ≈ 1.0", NearAbs(1.0f, 1.0f))
			UNIT_TEST_CONDITION("1.0 与 1.0 + ε/2 应视为相等", NearAbs(1.0f, 1.0f + MathEpsilon / 2.0f))
			UNIT_TEST_CONDITION("1.0 与 1.0 - ε/2 应视为相等", NearAbs(1.0f, 1.0f - MathEpsilon / 2.0f))
			UNIT_TEST_CONDITION("1.0 与 1.1 超出容差，不应视为相等", !NearAbs(1.0f, 1.1f))

			// 自定义 epsilon（显式传入）
			UNIT_TEST_CONDITION("1.0 ≈ 1.01（epsilon=0.02）", NearAbs(1.0f, 1.01f, 0.02f))
			UNIT_TEST_CONDITION("1.0 ≠ 1.01（epsilon=0.001）", !NearAbs(1.0f, 1.01f, 0.001f))

			// 特殊值
			UNIT_TEST_CONDITION("正无穷 ≈ 正无穷", NearAbs(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()))
			UNIT_TEST_CONDITION("负无穷 ≈ 负无穷", NearAbs(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity()))

			// --- NearRel 测试 ---
			UNIT_TEST_CHECKPOINT("测试 NearRel（相对误差）")
			UNIT_TEST_CONDITION("1000.0 与 1000.001 在相对误差下应接近",
				NearRel(1000.0f, 1000.001f)) // 默认 epsilon = MATH_EPSILON

			UNIT_TEST_CONDITION("1e-5 与 1.1e-5 相对误差较大，不应视为相等",
				!NearRel(1e-5f, 1.1e-5f))

			UNIT_TEST_CONDITION("0.0 与 0.0 应相等（相对误差需处理零值）",
				NearRel(0.0f, 0.0f))

			UNIT_TEST_CONDITION("0.0 与非零值不应视为相等",
				!NearRel(0.0f, MathEpsilon))

			// 显式 epsilon
			UNIT_TEST_CONDITION("100.0 与 101.0 在 epsilon=0.02 下应视为相等（相对误差 1%）",
				NearRel(100.0f, 101.0f, 0.02f))

			UNIT_TEST_CONDITION("1.0 与 2.0 在 epsilon=0.5 下应视为相等（相对误差 100%，容差 50%）",
				NearRel(1.0f, 2.0f, 0.5f))

			UNIT_TEST_CHECKPOINT("所有 NearAbs 与 NearRel 测试通过")
	}
	UNIT_TEST_AREA_END(TestNearFunctions)

	// 综合测试：多个函数组合使用
	UNIT_TEST_AREA_BEGIN(TestMathLibraryIntegration)
	{
		UNIT_TEST_MESSAGE("测试多个数学函数的组合使用")

			UNIT_TEST_MAY_THROW_EXCEPTION_AREA_BEGIN

			// 场景1：对齐 + 插值
			u8 originalValue = 17;
		u64 alignedBlocks = Ceil16(originalValue);        // = 2
		u64 alignedBytes = alignedBlocks * 16;            // = 32
		float interpolated = Lerp(0.0f, static_cast<float>(alignedBytes), 0.5f); // = 16.0f
		UNIT_TEST_CONDITION("组合测试 1：Ceil16 + Lerp", NearAbs(interpolated, 16.0f))

		// 场景2：Abs + Lerp
			float neg = -50.0f;
		float pos = Abs(neg);
		float mid = Lerp(neg, pos, 0.5f);
		UNIT_TEST_CONDITION("组合测试 2：Abs + Lerp", NearAbs(mid, 0.0f))

		// 场景3：验证 Lerp 精度（使用 NearRel）
			float exact = 30.0f;
		float approx = Lerp(0.0f, 100.0f, 0.3f);
		UNIT_TEST_CONDITION("组合测试 3：Lerp 相对误差应在容差内", NearRel(approx, exact))

			UNIT_TEST_MAY_THROW_EXCEPTION_AREA_END("数学库集成测试", false)

			UNIT_TEST_MESSAGE("数学库集成测试成功完成")
			UNIT_TEST_CHECKPOINT("所有集成测试通过")
	}
	UNIT_TEST_AREA_END(TestMathLibraryIntegration)

	// 测试 Vec2 向量结构体
	UNIT_TEST_AREA_BEGIN(TestVec2)
	{
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
	// 测试 Vec3 三维向量结构体
	UNIT_TEST_AREA_BEGIN(TestVec3)
	{
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
	// 测试 Vec4 四维向量结构体
	UNIT_TEST_AREA_BEGIN(TestVec4)
	{
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