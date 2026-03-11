#include "Code/UnitTest/BasicOutputInterface/ConsoleInterface.h"
#include "Code/UnitTest/Tests/Test_Format.hpp"
#include "Code/UnitTest/Tests/Test_String.hpp"
#include "Code/UnitTest/Tests/Test_String_NotVectorizedFindXXX.hpp"
#include "Code/UnitTest/UnitTestFramework.h"

#include "Code/Engine/String/String.hpp"

int main()
{
	constexpr bool s = noexcept(PenFramework::PenEngine::Begin(std::declval<std::string&>()));
	std::string_view a = std::string();
	auto& instance = PenFramework::UnitTest::Core::UnitTestManager().GetInstance();

	instance.Init(std::make_unique<PenFramework::UnitTest::SpdlogSTSharedContext>());
	instance.StartUnitTest();
}