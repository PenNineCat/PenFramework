#include "UnitTest/Core/Core.h"
#include "UnitTest/OutputInterface/ConsoleInterface.h"
#include "UnitTest/Tests/Test_Math.hpp"

int main()
{
	auto& instance = PenFramework::UnitTest::Core::UnitTestManager::GetInstance();

	auto context = std::make_unique<PenFramework::UnitTest::SpdlogSTSharedContext>();

	instance.Init(std::move(context));
	instance.StartUnitTest();
}