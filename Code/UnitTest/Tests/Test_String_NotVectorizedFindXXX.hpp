// File /UnitTest/Tests/Test_String_NotVectorizedFindXXX.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#undef _USE_STD_VECTOR_ALGORITHMS

#include "../../Engine/String/String.h"
#include "../Core/UnitTestFramework.h"

namespace PenFramework::UnitTest
{
	UNIT_TEST_AREA_BEGIN(TestStringNotVectorizedFindXXX)
	{
		// --- 非向量化查找与包含 ---
		UNIT_TEST_CHECKPOINT("测试 Find 与 Contain")
		{
			PenEngine::String s("Hello, World!");

			UNIT_TEST_CONDITION("FindFirstOf('o') = 4", s.FindFirstOf('o') == 4)
				UNIT_TEST_CONDITION("FindLastOf('o') = 8", s.FindLastOf('o') == 8)
				UNIT_TEST_CONDITION("FindFirstNotOf('H') = 1", s.FindFirstNotOf('H') == 1)
				UNIT_TEST_CONDITION("FindLastNotOf('!') = 12", s.FindLastNotOf('!') == 11)
				UNIT_TEST_CONDITION("Contain('W') = true", s.Contain('W'))
			UNIT_TEST_CONDITION("Contain('x') = false", !s.Contain('x'))

			PenEngine::String needle("or");
			UNIT_TEST_CONDITION("FindFirstOf(\"or\") = 8", s.FindFirstOf("or") == 4)
				UNIT_TEST_CONDITION("FindLastOf(\"lo\") = 3", s.FindLastOf("lo") == 10)
				UNIT_TEST_CONDITION("FindFirstOf(\"or\")", s.FindFirstOf(needle) == 4)
		}
	}
	UNIT_TEST_AREA_END(TestStringNotVectorizedFindXXX)
}