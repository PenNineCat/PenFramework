#include <print>
#include "Code/UnitTest/BasicOutputInterface/ConsoleInterface.h"
#include "Code/UnitTest/Tests/Test_String_ConvertSolutionBenchmark.hpp"
#include "Code/UnitTest/UnitTestFramework.h"
int main()
{
	auto& instance = PenFramework::UnitTest::Core::UnitTestManager().GetInstance();
	instance.Init(std::make_unique<PenFramework::UnitTest::SpdlogSTSharedContext>());
	instance.StartUnitTest();
}