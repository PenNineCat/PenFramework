#pragma once

#include <condition_variable>

#include "Interface.hpp"

namespace spdlog
{
	class logger;
}

namespace PenFramework::UnitTest
{
	class SpdlogSTSharedHandle : public Core::IUnitTestHandle
	{
	public:
		SpdlogSTSharedHandle() noexcept = default;
		explicit SpdlogSTSharedHandle(const std::shared_ptr<spdlog::logger>& logger) noexcept;

		SpdlogSTSharedHandle(const SpdlogSTSharedHandle&) = default;
		SpdlogSTSharedHandle(SpdlogSTSharedHandle&&) = default;
		SpdlogSTSharedHandle& operator=(const SpdlogSTSharedHandle&) = default;
		SpdlogSTSharedHandle& operator=(SpdlogSTSharedHandle&&) = default;

		virtual ~SpdlogSTSharedHandle() noexcept override = default;

		virtual void Exception(std::string_view message, const std::exception& exception, const std::stacktrace& stacktrace) override;
		virtual void Message(std::string_view message, PenEngine::u32 line) override;
		virtual void Checkpoint(std::string_view message, PenEngine::u32 line) override;
		virtual void Condition(std::string_view message, bool condition, PenEngine::u32 line) override;
		virtual void TerminalCondition(std::string_view message, bool condition, PenEngine::u32 line) override;
		virtual void UnhandledException(const std::exception& exception) override;
		virtual void Failed(std::string_view message, PenEngine::u32 line) override;
		virtual void TestStart(std::string_view testName, PenEngine::usize checkpointCount, std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint) override;
		virtual void TestFinish(std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint) override;
	private:
		std::shared_ptr<spdlog::logger> m_logger;

		std::string m_testName;
		std::chrono::steady_clock::time_point m_startTime;
		PenEngine::usize m_allCheckpointCount = 0;
		PenEngine::usize m_reachedCheckpointCount = 0;
		bool m_successful = true;
	};

	class SpdlogSTSharedContext : public Core::IUnitContext
	{
	public:
		SpdlogSTSharedContext();
		virtual ~SpdlogSTSharedContext() noexcept override = default;

		virtual void UTestStart(std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint) override;
		virtual void UTestEnd(std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint) override;
		virtual void FileStart(std::string_view filename, std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint) override;
		virtual void FileFinish(std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint) override;
		[[nodiscard]] virtual Core::IUnitTestHandle* AllocateTestHandle() override;
		virtual void FreeTestHandle(Core::IUnitTestHandle* handle) override;
	private:
		std::shared_ptr<spdlog::logger> m_logger;
		std::atomic<bool> m_locked = false;
		std::condition_variable m_cv;
		std::chrono::steady_clock::time_point m_uTestStartTimepoint;
		std::mutex m_mt;
		SpdlogSTSharedHandle m_handle;
	};

	class SpdlogMTSharedHandle : public Core::IUnitTestHandle
	{
	public:
		SpdlogMTSharedHandle() noexcept = default;
		explicit SpdlogMTSharedHandle(const std::shared_ptr<spdlog::logger>& logger) noexcept;

		SpdlogMTSharedHandle(const SpdlogMTSharedHandle&) = default;
		SpdlogMTSharedHandle(SpdlogMTSharedHandle&&) = default;
		SpdlogMTSharedHandle& operator=(const SpdlogMTSharedHandle&) = default;
		SpdlogMTSharedHandle& operator=(SpdlogMTSharedHandle&&) = default;

		virtual ~SpdlogMTSharedHandle() noexcept override = default;

		virtual void Exception(std::string_view message, const std::exception& exception, const std::stacktrace& stacktrace) override;
		virtual void Message(std::string_view message, PenEngine::u32 line) override;
		virtual void Checkpoint(std::string_view message, PenEngine::u32 line) override;
		virtual void Condition(std::string_view message, bool condition, PenEngine::u32 line) override;
		virtual void TerminalCondition(std::string_view message, bool condition, PenEngine::u32 line) override;
		virtual void UnhandledException(const std::exception& exception) override;
		virtual void Failed(std::string_view message, PenEngine::u32 line) override;
		virtual void TestStart(std::string_view testName, PenEngine::usize checkpointCount, std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint) override;
		virtual void TestFinish(std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint) override;
	private:
		std::shared_ptr<spdlog::logger> m_logger;

		std::string m_testName;
		std::chrono::steady_clock::time_point m_startTime;
		PenEngine::usize m_allCheckpointCount = 0;
		PenEngine::usize m_reachedCheckpointCount = 0;
		bool m_successful = true;
	};

	class SpdlogMTSharedContext : public Core::IUnitContext
	{
	public:
		SpdlogMTSharedContext();
		virtual ~SpdlogMTSharedContext() noexcept override = default;

		virtual void UTestStart(std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint) override;
		virtual void UTestEnd(std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint) override;
		virtual void FileStart(std::string_view filename, std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint) override;
		virtual void FileFinish(std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint) override;
		[[nodiscard]] virtual Core::IUnitTestHandle* AllocateTestHandle() override;
		virtual void FreeTestHandle(Core::IUnitTestHandle* handle) override;
	private:
		std::shared_ptr<spdlog::logger> m_logger;
		std::atomic<bool> m_locked = false;
		std::condition_variable m_cv;
		std::chrono::steady_clock::time_point m_uTestStartTimepoint;
		std::mutex m_mt;
		SpdlogMTSharedHandle m_handle;
	};
}