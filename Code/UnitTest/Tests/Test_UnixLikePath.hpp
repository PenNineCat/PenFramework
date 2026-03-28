// File /UnitTest/Tests/Test_UnixLikePath.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.
#pragma once

#ifdef _WIN32
#undef _WIN32
// 还需要手动在Path.cpp中undef _WIN32
#endif
#include "../UnitTestFramework.h"
#include "../../Engine/IO/Path.h"

namespace PenFramework::UnitTest
{
    UNIT_TEST_AREA_BEGIN(TestUnixLikePath)
    {
        using namespace PenEngine;

        UNIT_TEST_MESSAGE("开始测试 Unix-Like 路径逻辑 (强制非 Windows 环境)")

        // --- 标准绝对路径 (/usr/...) ---
            UNIT_TEST_CHECKPOINT("测试标准 Unix 绝对路径")
        {
            Path unixPath("/usr/local/bin/gcc");

            UNIT_TEST_CONDITION("Unix 路径应为绝对路径", unixPath.IsAbsolute())

            // Unix 风格没有盘符 (RootName 应为空)
                UNIT_TEST_CONDITION("Unix 路径无 RootName", unixPath.RootName().Empty())

                // 根目录应为 /
                UNIT_TEST_CONDITION("RootDirectory 应为 /", unixPath.RootDirectory().ToView() == "/")

                // 验证文件名
                UNIT_TEST_CONDITION("Filename 应为 gcc", unixPath.Filename().ToView() == "gcc")

                // 验证父路径
                UNIT_TEST_CONDITION("ParentPath 正确", unixPath.ParentPath().ToView() == "/usr/local/bin")
        }

        // --- 相对路径 ---
        UNIT_TEST_CHECKPOINT("测试 Unix 相对路径")
        {
            Path relPath("src/main.cpp");
            UNIT_TEST_CONDITION("相对路径判断", relPath.IsRelative())
                UNIT_TEST_CONDITION("Filename 正确", relPath.Filename().ToView() == "main.cpp")
                UNIT_TEST_CONDITION("ParentPath 正确", relPath.ParentPath().ToView() == "src")

                // 测试当前目录和父目录
                Path dotPath("./scripts/build.sh");
            UNIT_TEST_CONDITION("点开头路径为相对", dotPath.IsRelative())

                Path parentPath("../include/header.h");
            UNIT_TEST_CONDITION("双点开头路径为相对", parentPath.IsRelative())
                UNIT_TEST_CONDITION("ParentPath 识别 ../include", parentPath.ParentPath().ToView() == "../include")
        }

        // --- 边缘情况：在 Unix 模式下输入 Windows 风格路径 ---
        // 验证 Path 类在非 Windows 平台遇到 "C:/" 时的行为
        // 通常有两种处理方式：视为普通相对路径（目录名为 "C:"）或 视为绝对路径。
        // 这里测试其是否被识别为绝对路径（取决于你的库是否支持跨平台通用解析）
        UNIT_TEST_CHECKPOINT("测试 Unix 环境下对 Windows 风格路径的兼容性")
        {
            // 注意：在纯 Unix 逻辑下，"C:/path" 可能被视为一个名为 "C:" 的目录
            // 如果你的 Path 类设计为通用解析，这里应该通过。
            // 如果设计为严格 POSIX 兼容，这里 IsAbsolute 可能为 false。
            // 假设我们要验证它能“自动识别”，则预期如下：

            Path mixedPath("C:/Windows/System32");

            // 如果你的库支持通用格式，这里应为 true。
            // 如果库严格遵循 POSIX，"C:" 只是目录名，路径是相对路径。
            // 这里假设你的需求是“自动识别”，所以期望它是绝对路径：
            // UNIT_TEST_CONDITION("自动识别盘符路径", mixedPath.IsAbsolute()) 

            // 如果库是严格 POSIX 模式，则期望如下（视为名为 C: 的目录）：
            UNIT_TEST_CONDITION("在 Unix 模式下，C: 被视为目录名", mixedPath.IsRelative()) // 视具体实现策略而定
                UNIT_TEST_CONDITION("在 Unix 模式下，RootName 应为空", mixedPath.RootName().Empty())
        }

        // --- 根目录与空路径 ---
        UNIT_TEST_CHECKPOINT("测试根目录与空值")
        {
            Path root("/");
            UNIT_TEST_CONDITION("根目录 IsAbsolute", root.IsAbsolute())
                UNIT_TEST_CONDITION("根目录 Filename 为空", root.Filename().Empty())

                Path empty;
            UNIT_TEST_CONDITION("空路径 IsRelative", empty.IsRelative())
        }

        UNIT_TEST_MESSAGE("Unix-Like 路径测试单元完成")
    }
    UNIT_TEST_AREA_END(TestUnixLikePath)
}