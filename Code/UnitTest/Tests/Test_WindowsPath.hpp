// File /UnitTest/Tests/Test_WindowsPath.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "../../Engine/IO/Path.h"
#include "../UnitTestFramework.h"

namespace PenFramework::UnitTest
{
	UNIT_TEST_AREA_BEGIN(TestWindowsPath)
	{
		using namespace PenFramework::PenEngine;

		#ifndef _WIN32
		#define _WIN32
		#endif

		UNIT_TEST_MESSAGE("开始测试 Windows 路径逻辑")

		// --- 盘符路径解析 (C:/...) ---
			UNIT_TEST_CHECKPOINT("测试 Windows 盘符路径解析")
		{
			// 输入统一使用 /
			Path winPath("C:/Windows/System32/drivers/etc/hosts");

			UNIT_TEST_CONDITION("盘符路径应为绝对路径", winPath.IsAbsolute())

			// 验证根名称 (Root Name)
				UNIT_TEST_CONDITION("RootName 应为 C:", winPath.RootName().ToView() == "C:")

				// 验证根目录 (Root Directory)
				UNIT_TEST_CONDITION("RootDirectory 应为 /", winPath.RootDirectory().ToView() == "/")

				// 验证完整根路径 (Root Path = RootName + RootDirectory)
				UNIT_TEST_CONDITION("RootPath 应为 C:/", winPath.RootPath().ToView() == "C:/")

				// 验证文件名
				UNIT_TEST_CONDITION("Filename 应为 hosts", winPath.Filename().ToView() == "hosts")

				// 验证父路径
				UNIT_TEST_CONDITION("ParentPath 正确", winPath.ParentPath().ToView() == "C:/Windows/System32/drivers/etc")
		}

		// --- UNC 路径解析 (//Server/Share/...) ---
		UNIT_TEST_CHECKPOINT("测试 UNC 网络路径解析")
		{
			Path uncPath("//FileServer/Public/Docs/readme.txt");

			UNIT_TEST_CONDITION("UNC 路径应为绝对路径", uncPath.IsAbsolute())

			// 验证 RootName (通常包含服务器名)
			// 注意：具体实现可能是 "//FileServer" 或 "//FileServer/Public"，视库的设计而定
			// 这里假设符合 std::filesystem generic format: root-name is "//" + server
				StringView rootName = uncPath.RootName().ToView();
			UNIT_TEST_CONDITION("RootName 包含服务器名", rootName.Find("FileServer") != StringView::NPos)

			// 验证文件名
				UNIT_TEST_CONDITION("Filename 应为 readme.txt", uncPath.Filename().ToView() == "readme.txt")

				// 验证父路径
				UNIT_TEST_CONDITION("ParentPath 正确", uncPath.ParentPath().ToView() == "//FileServer/Public/Docs")
		}

		// --- 相对路径与特殊字符 ---
		UNIT_TEST_CHECKPOINT("测试 Windows 环境下的相对路径")
		{
			Path relPath("Program Files/App/config.ini");
			UNIT_TEST_CONDITION("无前缀路径应为相对路径", relPath.IsRelative())
				UNIT_TEST_CONDITION("Filename 正确", relPath.Filename().ToView() == "config.ini")

				// 测试带空格的路径段
				Path spacePath("D:/My Games/Game Name/save.dat");
			UNIT_TEST_CONDITION("带空格路径解析正常", spacePath.Filename().ToView() == "save.dat")
				UNIT_TEST_CONDITION("带空格路径 Parent 正确", spacePath.ParentPath().ToView() == "D:/My Games/Game Name")
		}

		// --- 清理宏定义 ---
		#ifdef __MANUAL_WIN32_DEF__
		#undef _WIN32
		#undef __MANUAL_WIN32_DEF__
		#endif

		UNIT_TEST_MESSAGE("Windows 路径测试单元完成")
	}
	UNIT_TEST_AREA_END(TestWindowsPath)
}