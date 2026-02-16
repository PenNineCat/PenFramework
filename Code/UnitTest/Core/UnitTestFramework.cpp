// File /UnitTest/Core/UnitTestFramework.cpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "UnitTestFramework.h"

#include <barrier>
#include <deque>
#include <mutex>
#include <numeric>
#include <print>
#include <ranges>

void PenFramework::UnitTest::Core::UnitTestManager::Init(std::unique_ptr<IUnitContext> context)
{
	m_context = std::move(context);
	PenEngine::usize fileCount = m_registerUnitTest.size();
	PenEngine::usize count = 0;

	for (auto& p : m_registerUnitTest)
	{
		count += p.second.size();
	}

	std::println("扫描完成！共有{}个测试文件{}个测试项目", fileCount, count);
}

void PenFramework::UnitTest::Core::UnitTestManager::Register(std::string_view filename, std::string_view testName, InvokerPtr ptr)
{
	auto it = m_registerUnitTest.find(filename);
	if (it == m_registerUnitTest.end())
		it = m_registerUnitTest.emplace(std::string(filename), std::vector<UnitTestNode>()).first;

	it->second.emplace_back(std::string(testName), ptr);
}

void PenFramework::UnitTest::Core::UnitTestManager::StartUnitTest(PenEngine::u8 parallelTestNum)
{
	if (m_context == nullptr)
		return;

	m_context->UTestStart(std::chrono::steady_clock::now(),std::chrono::system_clock::now());

	if (parallelTestNum == 1)
	{
		for (auto& [filename, vec] : m_registerUnitTest)
		{
			m_context->FileStart(filename, std::chrono::steady_clock::now(), std::chrono::system_clock::now());

			for (auto& [testName, invoker] : vec)
			{
				IUnitTestHandle* handle = m_context->AllocateTestHandle();
				invoker(handle);
				m_context->FreeTestHandle(handle);
			}

			m_context->FileFinish(std::chrono::steady_clock::now(), std::chrono::system_clock::now());
		}
	}
	else
	{

	}

	m_context->UTestEnd(std::chrono::steady_clock::now(), std::chrono::system_clock::now());
}
