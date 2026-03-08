// File /UnitTest/OutputInterface/ConsoleInterface.cpp
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "ConsoleInterface.h"

#define SPDLOG_USE_STD_FORMAT

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

PenFramework::UnitTest::SpdlogSTSharedHandle::SpdlogSTSharedHandle(const std::shared_ptr<spdlog::logger>& logger) noexcept : m_logger(logger)
{}

void PenFramework::UnitTest::SpdlogSTSharedHandle::Exception(std::string_view message, const std::exception& exception,
															 const std::stacktrace& stacktrace)
{}

void PenFramework::UnitTest::SpdlogSTSharedHandle::UnhandledException(const std::exception& exception)
{
	m_logger->error("测试抛出未被捕获的异常 异常类型：{} 异常信息：{}", typeid(exception).name(), exception.what());
	m_successful = false;
}

void PenFramework::UnitTest::SpdlogSTSharedHandle::Failed(std::string_view message, PenEngine::u32 line)
{}

void PenFramework::UnitTest::SpdlogSTSharedHandle::TestStart(std::string_view testName,
															 PenEngine::usize checkpointCount, std::chrono::steady_clock::time_point stPoint,
															 std::chrono::system_clock::time_point scPoint)
{
	m_testName = testName;
	m_startTime = stPoint;
	m_allCheckpointCount = checkpointCount;
	m_reachedCheckpointCount = 0;
	m_logger->info("测试单元：{}开始于：{:%Y-%m-%d %H:%M:%S} 共检测到{}个检查点", testName, scPoint, checkpointCount);
}

void PenFramework::UnitTest::SpdlogSTSharedHandle::TestFinish(std::chrono::steady_clock::time_point stPoint,
															  std::chrono::system_clock::time_point scPoint)
{
	m_logger->info("测试单元：{}结束于：{:%Y-%m-%d %H:%M:%S} 耗时：{} 完成{}/{}个检查点 测试是否成功：{}",
				   m_testName,
				   scPoint,
				   std::chrono::duration_cast<std::chrono::milliseconds>(stPoint - m_startTime),
				   m_reachedCheckpointCount,
				   m_allCheckpointCount,
				   m_successful
	);
}

PenFramework::UnitTest::SpdlogSTSharedContext::SpdlogSTSharedContext()
{
	m_logger = spdlog::stdout_color_st("Log");
	m_handle = SpdlogSTSharedHandle(m_logger);
}

void PenFramework::UnitTest::SpdlogSTSharedHandle::Message(std::string_view message, PenEngine::u32 line)
{
	m_logger->info("测试消息——行数：{} 消息：{}", line, message);
}

void PenFramework::UnitTest::SpdlogSTSharedHandle::Checkpoint(std::string_view message, PenEngine::u32 line)
{
	m_reachedCheckpointCount++;
	m_logger->info("测试检查点——行数：{} 消息：{}", line, message);
}

void PenFramework::UnitTest::SpdlogSTSharedHandle::Condition(std::string_view message, bool condition, PenEngine::u32 line)
{
	m_logger->info("测试条件检查——行数：{} 消息：{} 条件是否达成：{}", line, message, condition);
}

void PenFramework::UnitTest::SpdlogSTSharedHandle::TerminalCondition(std::string_view message, bool condition,
																	 PenEngine::u32 line)
{
	m_logger->info("测试关键条件检查——行数：{}消息：{} 条件是否达成：{}", line, message, condition);
}

void PenFramework::UnitTest::SpdlogSTSharedContext::UTestStart(std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint)
{
	m_uTestStartTimepoint = stPoint;
	m_logger->info("单元测试开始于：{:%Y-%m-%d %H:%M:%S}", scPoint);
}

void PenFramework::UnitTest::SpdlogSTSharedContext::UTestEnd(std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint)
{
	m_logger->info("单元测试结束于：{:%Y-%m-%d %H:%M:%S}", scPoint);
}

void PenFramework::UnitTest::SpdlogSTSharedContext::FileStart(std::string_view filename,
															  std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint)
{}

void PenFramework::UnitTest::SpdlogSTSharedContext::FileFinish(std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint)
{}

PenFramework::UnitTest::Core::IUnitTestHandle* PenFramework::UnitTest::SpdlogSTSharedContext::AllocateTestHandle()
{
	std::unique_lock ul(m_mt);
	m_cv.wait(ul, [this] { return !m_locked; });
	m_locked = true;
	return &m_handle;
}

void PenFramework::UnitTest::SpdlogSTSharedContext::FreeTestHandle(Core::IUnitTestHandle* handle)
{
	m_locked = false;
	m_cv.notify_one();
}

PenFramework::UnitTest::SpdlogMTSharedHandle::SpdlogMTSharedHandle(const std::shared_ptr<spdlog::logger>& logger) noexcept : m_logger(logger)
{}

void PenFramework::UnitTest::SpdlogMTSharedHandle::Exception(std::string_view message, const std::exception& exception,
															 const std::stacktrace& stacktrace)
{}

void PenFramework::UnitTest::SpdlogMTSharedHandle::UnhandledException(const std::exception& exception)
{
	m_logger->error("测试抛出未被捕获的异常 异常类型：{} 异常信息：{}", typeid(exception).name(), exception.what());
	m_successful = false;
}

void PenFramework::UnitTest::SpdlogMTSharedHandle::Failed(std::string_view message, PenEngine::u32 line)
{}

void PenFramework::UnitTest::SpdlogMTSharedHandle::TestStart(std::string_view testName,
															 PenEngine::usize checkpointCount, std::chrono::steady_clock::time_point stPoint,
															 std::chrono::system_clock::time_point scPoint)
{
	m_testName = testName;
	m_startTime = stPoint;
	m_allCheckpointCount = checkpointCount;
	m_reachedCheckpointCount = 0;
	m_logger->info("测试单元：{}开始于：{:%Y-%m-%d %H:%M:%S} 共检测到{}个检查点", testName, scPoint, checkpointCount);
}

void PenFramework::UnitTest::SpdlogMTSharedHandle::TestFinish(std::chrono::steady_clock::time_point stPoint,
															  std::chrono::system_clock::time_point scPoint)
{
	m_logger->info("测试单元：{}结束于：{:%Y-%m-%d %H:%M:%S} 耗时：{} 完成{}/{}个检查点 测试是否成功：{}",
				   m_testName,
				   scPoint,
				   std::chrono::duration_cast<std::chrono::milliseconds>(stPoint - m_startTime),
				   m_reachedCheckpointCount,
				   m_allCheckpointCount,
				   m_successful
	);
}

PenFramework::UnitTest::SpdlogMTSharedContext::SpdlogMTSharedContext()
{
	m_logger = spdlog::stdout_color_mt("Log");
	m_handle = SpdlogMTSharedHandle(m_logger);
}

void PenFramework::UnitTest::SpdlogMTSharedHandle::Message(std::string_view message, PenEngine::u32 line)
{
	m_logger->info("测试消息——行数：{} 消息：{}", line, message);
}

void PenFramework::UnitTest::SpdlogMTSharedHandle::Checkpoint(std::string_view message, PenEngine::u32 line)
{
	m_reachedCheckpointCount++;
	m_logger->info("测试检查点——行数：{} 消息：{}", line, message);
}

void PenFramework::UnitTest::SpdlogMTSharedHandle::Condition(std::string_view message, bool condition, PenEngine::u32 line)
{
	m_logger->info("测试条件检查——行数：{} 消息：{} 条件是否达成：{}", line, message, condition);
}

void PenFramework::UnitTest::SpdlogMTSharedHandle::TerminalCondition(std::string_view message, bool condition,
																	 PenEngine::u32 line)
{
	m_logger->info("测试关键条件检查——行数：{}消息：{} 条件是否达成：{}", line, message, condition);
}

void PenFramework::UnitTest::SpdlogMTSharedContext::UTestStart(std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint)
{
	m_uTestStartTimepoint = stPoint;
	m_logger->info("单元测试开始于：{:%Y-%m-%d %H:%M:%S}", scPoint);
}

void PenFramework::UnitTest::SpdlogMTSharedContext::UTestEnd(std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint)
{
	m_logger->info("单元测试结束于：{:%Y-%m-%d %H:%M:%S}", scPoint);
}

void PenFramework::UnitTest::SpdlogMTSharedContext::FileStart(std::string_view filename,
															  std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint)
{}

void PenFramework::UnitTest::SpdlogMTSharedContext::FileFinish(std::chrono::steady_clock::time_point stPoint, std::chrono::system_clock::time_point scPoint)
{}

PenFramework::UnitTest::Core::IUnitTestHandle* PenFramework::UnitTest::SpdlogMTSharedContext::AllocateTestHandle()
{
	std::unique_lock ul(m_mt);
	m_cv.wait(ul, [this] { return !m_locked; });
	m_locked = true;
	return &m_handle;
}

void PenFramework::UnitTest::SpdlogMTSharedContext::FreeTestHandle(Core::IUnitTestHandle* handle)
{
	m_locked = false;
	m_cv.notify_one();
}