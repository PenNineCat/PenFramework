#include "Code/UnitTest/Tests/Test_UnixLikePath.hpp"
#include "Code/UnitTest/BasicOutputInterface/ConsoleInterface.h"
#include "Code/UnitTest/UnitTestFramework.h"

int main()
{
	auto& instance = PenFramework::UnitTest::Core::UnitTestManager().GetInstance();
	instance.Init(std::make_unique<PenFramework::UnitTest::SpdlogSTSharedContext>());
	instance.StartUnitTest();
}