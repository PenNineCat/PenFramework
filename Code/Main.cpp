 //File Main.cpp
 //This file is a part of PenFramework Project
 //https:github.com/PenNineCat/PenFramework
 //
 //Copyright (C) 2025 - Present PenNineCat. All rights reserved

 //This Source Code Form is subject to the terms of the Mozilla Public
 //License, v. 2.0. If a copy of the MPL was not distributed with this
 //file, You can obtain one at https:mozilla.org/MPL/2.0/.

#include <print>

#include "UnitTest/Core/UnitTestFramework.h"
#include "UnitTest/OutputInterface/ConsoleInterface.h"
#include "UnitTest/Tests/Test_String.hpp"

using namespace PenFramework;
using namespace PenEngine;
using namespace UnitTest;

int main()
{
	auto& instance = PenFramework::UnitTest::Core::UnitTestManager::GetInstance();

	auto context = std::make_unique<PenFramework::UnitTest::SpdlogSTSharedContext>();

	instance.Init(std::move(context));
	instance.StartUnitTest();
}

