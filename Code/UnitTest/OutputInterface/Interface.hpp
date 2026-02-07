#pragma once

#include "../../Engine/Common/Type.hpp"
#include <chrono>
#include <stacktrace>

namespace PenFramework::UnitTest::Core
{
	class IUnitTestHandle
	{
	public:
		virtual ~IUnitTestHandle() noexcept = default;
		virtual void Exception(std::string_view message, const std::exception& exception, const std::stacktrace& stacktrace) = 0;
		virtual void Message(std::string_view message, PenEngine::u32 line) = 0;
		virtual void Checkpoint(std::string_view message, PenEngine::u32 line) = 0;
		virtual void Condition(std::string_view message, bool condition, PenEngine::u32 line) = 0;
		virtual void TerminalCondition(std::string_view message, bool condition, PenEngine::u32 line) = 0;
		virtual void UnhandledException(const std::exception& exception) = 0;
		virtual void Failed(std::string_view message, PenEngine::u32 line) = 0;
		virtual void TestStart(std::string_view testName,PenEngine::usize checkpointCount, std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint) = 0;
		virtual void TestFinish(std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint) = 0;
	};

	class IUnitContext
	{
	public:
		virtual ~IUnitContext() noexcept = default;
		virtual void UTestStart(std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint) = 0;
		virtual void UTestEnd(std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint) = 0;
		virtual void FileStart(std::string_view filename, std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint) = 0;
		virtual void FileFinish(std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint) = 0;
		virtual IUnitTestHandle* AllocateTestHandle() = 0;
		virtual void FreeTestHandle(IUnitTestHandle* handle) = 0;
	};

}