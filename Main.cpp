#include "Code/UnitTest/BasicOutputInterface/ConsoleInterface.h"
#include "Code/UnitTest/Tests/Test_Format.hpp"
#include "Code/UnitTest/Tests/Test_String.hpp"
#include "Code/UnitTest/Tests/Test_String_NotVectorizedFindXXX.hpp"
#include "Code/UnitTest/UnitTestFramework.h"

#include "Code/Engine/String/String.hpp"

using namespace PenFramework;
using namespace PenEngine;
using namespace UnitTest;

int main()
{
	auto& instance = PenFramework::UnitTest::Core::UnitTestManager().GetInstance();

	instance.Init(std::make_unique<PenFramework::UnitTest::SpdlogSTSharedContext>());
	instance.StartUnitTest();
}