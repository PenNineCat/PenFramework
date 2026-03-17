// File /UnitTest/OutputInterface/Interface.hpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "../Engine/Common/Type.hpp"
#include "../Engine/String/String.hpp"
#include <chrono>
#include <stacktrace>

namespace PenFramework::UnitTest::Core
{
	class IUnitTestHandle
	{
	public:
		virtual ~IUnitTestHandle() noexcept = default;
		virtual void Exception(PenEngine::StringView message, const std::exception& exception, const std::stacktrace& stacktrace) = 0;
		virtual void Message(PenEngine::StringView message, PenEngine::U32 line) = 0;
		virtual void Checkpoint(PenEngine::StringView message, PenEngine::U32 line) = 0;
		virtual void Condition(PenEngine::StringView message, bool condition, PenEngine::U32 line) = 0;
		virtual void TerminalCondition(PenEngine::StringView message, bool condition, PenEngine::U32 line) = 0;
		virtual void UnhandledException(const std::exception& exception) = 0;
		virtual void Failed(PenEngine::StringView message, PenEngine::U32 line) = 0;
		virtual void TestStart(PenEngine::StringView testName,PenEngine::Usize checkpointCount, std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint) = 0;
		virtual void TestFinish(std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint) = 0;
	};

	class IUnitContext
	{
	public:
		virtual ~IUnitContext() noexcept = default;
		virtual void UTestStart(std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint) = 0;
		virtual void UTestEnd(std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint) = 0;
		virtual void FileStart(PenEngine::StringView filename, std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint) = 0;
		virtual void FileFinish(std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint) = 0;
		virtual IUnitTestHandle* AllocateTestHandle() = 0;
		virtual void FreeTestHandle(IUnitTestHandle* handle) = 0;
	};

}