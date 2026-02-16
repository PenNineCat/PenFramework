// File /UnitTest/Core/UnitTestFramework.h
// This file is a part of PenFramework Project
// https://github.com/PenNineCat/PenFramework
// 
// Copyright (C) 2025 - Present PenNineCat. All rights reserved
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include "../../Engine/String/StringUnorderedMap.hpp"
#include "../../Engine/Utils/Preprocessor.hpp"
#include "../../Engine/Utils/Singleton.hpp"
#include "../OutputInterface/Interface.hpp"
#include <chrono>
#include <unordered_map>

#define EXPAND_EACH_INHERIT_CLASS(r,data,InheritClass) public InheritClass
#define EXPAND_INHERIT_CLASS(...) __VA_OPT__( : BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(EXPAND_EACH_INHERIT_CLASS,,VA_TO_SEQ(__VA_ARGS__))))

#define UNIT_TEST_AREA_BEGIN(TestName,...) \
struct TestName##_AutoRunner EXPAND_INHERIT_CLASS(__VA_ARGS__) \
{\
	static PenFramework::PenEngine::usize g_checkpointCount; \
	static inline PenFramework::PenEngine::usize g_checkpointCountMacroStart = __COUNTER__; \
	static void Invoker(PenFramework::UnitTest::Core::IUnitTestHandle* handle) \
	{\
		handle->TestStart(#TestName,g_checkpointCount,std::chrono::steady_clock::now(),std::chrono::system_clock::now()); \
		try \
		{ \
			TestName##_AutoRunner runner;\
			runner.TestMethod(handle);\
		} \
		catch (const PenFramework::UnitTest::Core::UnitTestFailedResult& u) \
		{ \
			handle->Failed(u.Result(),u.Line()); \
		} \
		catch (const std::exception& e) \
		{ \
			handle->UnhandledException(e);\
		} \
		handle->TestFinish(std::chrono::steady_clock::now(),std::chrono::system_clock::now()); \
	}\
	void TestMethod(PenFramework::UnitTest::Core::IUnitTestHandle* handle); \
}; \
inline PenFramework::UnitTest::Core::UnitTestAutoRegister UNIQUE_VAR(g_##TestName##_AutoRegister)(__FILE__,#TestName,&TestName##_AutoRunner::Invoker); \
inline void TestName##_AutoRunner::TestMethod(PenFramework::UnitTest::Core::IUnitTestHandle* handle)

#define UNIT_TEST_AREA_END(TestName) inline PenFramework::PenEngine::usize TestName##_AutoRunner::g_checkpointCount =  __COUNTER__ - TestName##_AutoRunner::g_checkpointCountMacroStart - 1;

#define UNIT_TEST_CONDITION(message,condition) \
	handle->Condition(message,condition,__LINE__);

#define UNIT_TEST_MESSAGE(message) \
	handle->Message(message,__LINE__);

#define UNIT_TEST_CHECKPOINT(message) \
	__COUNTER__; \
	handle->Checkpoint(message,__LINE__);

#define UNIT_TEST_MAY_THROW_EXCEPTION_AREA_BEGIN \
try \
{

#define UNIT_TEST_MAY_THROW_EXCEPTION_AREA_END(message,needRethrow) \
} \
catch(std::exception&e) \
{ \
	handle->Exception(message,e,std::stacktrace::current()); \
	if(needRethrow) \
		throw; \
}

#define UNIT_TEST_MAY_THROW_EXCEPTION_OPERATION(message,operation,needRethrow) \
	try \
	{ \
		operation; \
	} \
	catch(std::exception& e) \
	{ \
		handle->Exception(message,e,std::stacktrace::current()); \
		if (needRethrow) \
			throw; \
	}

#define UNIT_TEST_TERMINAL_CONDITION(message,condition) \
{ \
	bool message_terminalCondition = condition;\
	handle->TerminalCondition(message,message_terminalCondition,__LINE__); \
	if(message_terminalCondition == false) \
		throw UnitTestFailedResult(message,__LINE__); \
} \

#define UNIT_TEST_FAILED(message) \
	throw UnitTestFailedResult(message,__LINE__); \

namespace PenFramework::UnitTest::Core
{
	using InvokerPtr = void(*)(IUnitTestHandle*);

	class UnitTestFailedResult
	{
	public:
		UnitTestFailedResult(std::string_view result, PenEngine::u32 line) : m_result(result), m_line(line) {}

		std::string_view Result() const noexcept { return m_result; }
		PenEngine::u32 Line() const noexcept { return m_line; }
	private:
		std::string m_result;
		PenEngine::u32 m_line;
	};

	class UnitTestManager : public PenEngine::Singleton<UnitTestManager>
	{
	public:
		virtual ~UnitTestManager() noexcept override = default;
		void Init(std::unique_ptr<IUnitContext> context);
		void Register(std::string_view filename, std::string_view testName, InvokerPtr ptr);
		void StartUnitTest(PenEngine::u8 parallelTestNum = 1);
	private:
		struct UnitTestNode
		{
			std::string TestName;
			InvokerPtr InvokerPtr;
		};

		PenEngine::StringUnorderedMap<std::vector<UnitTestNode>> m_registerUnitTest;
		std::unique_ptr<IUnitContext> m_context;
	};

	class UnitTestAutoRegister
	{
	public:
		UnitTestAutoRegister(std::string_view filename,std::string_view testName,InvokerPtr ptr) 
		{
			UnitTestManager::GetInstance().Register(filename,testName,ptr);
		}
	};
}
