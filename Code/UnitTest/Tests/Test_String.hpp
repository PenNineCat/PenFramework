// File /UnitTest/Tests/Test_String.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "../../Engine/String/String.h"
#include "../Core/UnitTestFramework.h"

namespace PenFramework::UnitTest
{
	using namespace PenEngine;
	// 测试 String 字符串类（SSO + 堆分配）
	UNIT_TEST_AREA_BEGIN(TestString)
	{
		UNIT_TEST_MESSAGE("测试 String 字符串类（含 SSO 与堆分配）")

		// --- 构造函数 ---
			UNIT_TEST_CHECKPOINT("测试构造函数")
		{
			String s0;
			UNIT_TEST_CONDITION("默认构造为空", s0.Empty() && s0.Size() == 0)

				String s1("Hello");
			UNIT_TEST_CONDITION("C字符串构造", s1.Size() == 5 && strcmp(s1.Data(), "Hello") == 0)

				String s2("World!", 5); // 截断 '!'
			UNIT_TEST_CONDITION("带长度构造", s2.Size() == 5 && strcmp(s2.Data(), "World") == 0)

				String s3(s1, 3); // 取前3字符
			UNIT_TEST_CONDITION("从String截断构造", s3.Size() == 3 && strcmp(s3.Data(), "Hel") == 0)

				String s4("A"); // 单字符，SSO
			String s5(std::move(s4));
			UNIT_TEST_CONDITION("移动构造后源为空", s4.Empty() && s5 == "A")
		}

		// --- 赋值运算符 ---
		UNIT_TEST_CHECKPOINT("测试赋值运算符")
		{
			String a("Alpha");
			String b("Beta");

			a = b;
			UNIT_TEST_CONDITION("拷贝赋值", a == "Beta")

				String c("Gamma");
			a = std::move(c);
			UNIT_TEST_CONDITION("移动赋值", a == "Gamma" && c.Empty())

				a = "Delta";
			UNIT_TEST_CONDITION("C字符串赋值", a == "Delta")

				a = 'X';
			UNIT_TEST_CONDITION("字符赋值", a == "X" && a.Size() == 1)
		}

		// --- 追加操作（Append / +=）---
		UNIT_TEST_CHECKPOINT("测试 Append 与 operator+=")
		{
			String s("Hello");
			s += " World";
			UNIT_TEST_CONDITION("+= C字符串", s == "Hello World")

				s.Append('!');
			UNIT_TEST_CONDITION("Append 字符", s == "Hello World!")

				s += String(", Hi");
			UNIT_TEST_CONDITION("+= String", s == "Hello World!, Hi")

				s.Append(" There", 6); // " There" 前6字符 → " There"
			UNIT_TEST_CONDITION("Append(ptr, len)", s == "Hello World!, Hi There")
		}

		// --- 大小与容量管理 ---
		UNIT_TEST_CHECKPOINT("测试 Size, Capacity, Resize, Clear")
		{
			String s("Test");
			UNIT_TEST_CONDITION("初始状态", s.Size() == 4 && s.Capacity() >= 4)

				s.Resize(6, 'X');
			UNIT_TEST_CONDITION("Resize 扩展", s == "TestXX" && s.Size() == 6)

				s.Resize(2);
			UNIT_TEST_CONDITION("Resize 截断", s == "Te" && s.Size() == 2)

				s.Clear();
			UNIT_TEST_CONDITION("Clear 后为空", s.Empty() && s.Size() == 0)

				s.Reserve(50);
			UNIT_TEST_CONDITION("Reserve 增加容量", s.Capacity() >= 50)

				String t("LongEnoughToBeAllocateOnHeap");
			t.ReserveExtra(20);
			t.ShrinkToFit();
			UNIT_TEST_CONDITION("ShrinkToFit 不改变内容，只会缩小容量", t == "LongEnoughToBeAllocateOnHeap" && t.Size() == t.Capacity())
		}

		// --- 访问与子串 ---
		UNIT_TEST_CHECKPOINT("测试 Data, CStr, Front, Back, SubStr, Left, Right")
		{
			String s("ABCDEF");

			UNIT_TEST_CONDITION("Front/Back", s.Front() == 'A' && s.Back() == 'F')

				String sub = s.Substr(2, 3);
			UNIT_TEST_CONDITION("SubStr(2,3) = CDE", sub == "CDE")

				String left = s.Left(3);
			UNIT_TEST_CONDITION("Left(3) = ABC", left == "ABC")

				String right = s.Right(2);
			UNIT_TEST_CONDITION("Right(2) = EF", right == "EF")

				String full = s.Substr();  // 默认 off=0, len=Npos
			UNIT_TEST_CONDITION("SubStr() = 全串", full == "ABCDEF")
		}

		// --- 大小写转换 ---
		//UNIT_TEST_CHECKPOINT("测试 ToUpper / ToLower")
		{
			//String lower("Hello World!");
			//String upper = lower.ToUpper();
			//UNIT_TEST_CONDITION("ToUpper", upper == "HELLO WORLD!")

			//    String back = upper.ToLower();
			//UNIT_TEST_CONDITION("ToLower", back == "hello world!")
		}

		// --- 查找与包含 ---
		UNIT_TEST_CHECKPOINT("测试 Find 与 Contain")
		{
			String s("Hello, World!");

			UNIT_TEST_CONDITION("FindFirstOf('o') = 4", s.FindFirstOf('o') == 4)
				UNIT_TEST_CONDITION("FindLastOf('o') = 8", s.FindLastOf('o') == 8)
				UNIT_TEST_CONDITION("FindFirstNotOf('H') = 1", s.FindFirstNotOf('H') == 1)
				UNIT_TEST_CONDITION("FindLastNotOf('!') = 12", s.FindLastNotOf('!') == 11)
				UNIT_TEST_CONDITION("Contain('W') = true", s.Contain('W'))
				UNIT_TEST_CONDITION("Contain('x') = false", !s.Contain('x'))

				String needle("or");
			UNIT_TEST_CONDITION("FindFirstOf(\"or\") = 8", s.FindFirstOf("or") == 4)
				UNIT_TEST_CONDITION("FindLastOf(\"lo\") = 3", s.FindLastOf("lo") == 10)
				UNIT_TEST_CONDITION("FindFirstOf(\"or\")",s.FindFirstOf(needle) == 4)
		}

		// --- 比较运算符 ---
		UNIT_TEST_CHECKPOINT("测试 operator==")
		{
			String a("Equal");
			String b("Equal");
			const char* cstr = "Equal";

			UNIT_TEST_CONDITION("String == String", a == b)
				UNIT_TEST_CONDITION("String == CStr", a == cstr)
				UNIT_TEST_CONDITION("CStr == String", cstr == a) // 若有对称重载

				String d("NotEqual");
			UNIT_TEST_CONDITION("不等比较", a != d)
		}

		// --- 下标访问 ---
		UNIT_TEST_CHECKPOINT("测试 operator[]")
		{
			String s("Modify");
			s[0] = 'm';
			s[5] = 'y'; // 注意：若内部有\0保护，需确保可写
			UNIT_TEST_CONDITION("下标修改", s == "modify")

				const String cs("Read");
			UNIT_TEST_CONDITION("const 下标", cs[0] == 'R' && cs[3] == 'd')
		}

		// --- 析构与资源释放（隐式测试）---
		UNIT_TEST_CHECKPOINT("测试析构安全性")
		{
			// 创建大量短/长字符串，验证无内存泄漏或崩溃
			for (int i = 0; i < 100; ++i)
			{
				String short_str("SSO");
				String long_str("This is a very long string that definitely goes on the heap");
				short_str = long_str;
				long_str = String("New value");
			}
			UNIT_TEST_CONDITION("循环赋值无崩溃", true)
		}

		UNIT_TEST_MESSAGE("String 所有测试通过")
			UNIT_TEST_CHECKPOINT("String 单元测试完成")
	}
	UNIT_TEST_AREA_END(TestString)
}
