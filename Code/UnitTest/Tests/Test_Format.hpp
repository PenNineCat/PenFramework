// File /UnitTest/Tests/Test_Format.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "../../Engine/String/Format.hpp"
#include "../UnitTestFramework.h"

#include <numbers>

namespace PenFramework::UnitTest
{
    UNIT_TEST_AREA_BEGIN(TestFormat)
    {
        using namespace PenEngine;
        UNIT_TEST_MESSAGE("测试 Format")

        // 基础类型
            UNIT_TEST_CHECKPOINT("基础类型格式化")
        {
            String s1 = Format("{}", 42);
            UNIT_TEST_CONDITION("整数", s1 == "42")

                String s2 = Format("{}", std::numbers::pi_v<float>);
            UNIT_TEST_CONDITION("浮点数", s2.Contain("3.14159"))

                String s3 = Format("{}", true);
            UNIT_TEST_CONDITION("布尔值", s3 == "true")

                String s4 = Format("{}", 'X');
            UNIT_TEST_CONDITION("字符", s4 == "X")
        }

        // 对齐与填充
        UNIT_TEST_CHECKPOINT("对齐与填充")
        {
            String s = Format("{:>8}", "Hi");
            UNIT_TEST_CONDITION("右对齐", s == "      Hi")

                s = Format("{:*^10}", "Test");
            UNIT_TEST_CONDITION("居中填充", s == "***Test***")
        }

        // 精度与进制
        UNIT_TEST_CHECKPOINT("精度与进制")
        {
	        String s = Format("{:.2f}", 1.23456);
            UNIT_TEST_CONDITION("浮点精度", s == "1.23")

                s = Format("{:x}", 255);
            UNIT_TEST_CONDITION("十六进制", s == "ff")

                s = Format("{:b}", 5);
            UNIT_TEST_CONDITION("二进制", s == "101")
        }

        // 组合与转义
        UNIT_TEST_CHECKPOINT("组合与转义")
        {
            String s = Format("Hello {}, you have {} messages", "Alice", 5);
            UNIT_TEST_CONDITION("多参数", s == "Hello Alice, you have 5 messages")

                s = Format("{{escaped}}");
            UNIT_TEST_CONDITION("双花括号转义", s == "{escaped}")
        }

        // 异常情况（若支持）
        // UNIT_TEST_CHECKPOINT("格式错误处理")
        // {
        //     // 可选：验证无效格式抛异常或返回错误
        // }

        UNIT_TEST_MESSAGE("Format 所有测试通过")
            UNIT_TEST_CHECKPOINT("Format 单元测试完成")
    }
    UNIT_TEST_AREA_END(TestFormat)
}
