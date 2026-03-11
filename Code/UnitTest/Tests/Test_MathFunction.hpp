// File /UnitTest/Tests/Test_MathFunction.hpp
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
#include "../UnitTestFramework.h"

namespace PenFramework::UnitTest
{
	UNIT_TEST_AREA_BEGIN(TestMathFunction)
	{
		using namespace PenEngine;
			// 测试 CeilPow2 及相关函数
		UNIT_TEST_MESSAGE("测试 CeilPow2 函数")
			UNIT_TEST_CHECKPOINT("测试基础无效幂参数")
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

			UNIT_TEST_MESSAGE("所有 Ceil 函数基础测试通过")

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

			UNIT_TEST_MESSAGE("所有 Abs 测试通过")

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

			UNIT_TEST_MESSAGE("所有 Lerp 测试通过")

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

			UNIT_TEST_MESSAGE("所有 NearAbs 与 NearRel 测试通过")

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
	UNIT_TEST_AREA_END(TestMathFunction)
}
