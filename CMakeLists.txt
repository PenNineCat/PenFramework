cmake_minimum_required (VERSION 3.30)
project ("PenFramework")

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS ON)

add_compile_options("/Zc:preprocessor")

find_package(spdlog CONFIG REQUIRED)
find_package(Boost CONFIG REQUIRED 
	COMPONENTS
		uuid
		asio
		beast
		preprocessor
)

file(GLOB_RECURSE CodeResource "Code/*.h" "Code/*.hpp" "Code/*.cpp" "Code/*.inl")

add_executable(${PROJECT_NAME} ${CodeResource})

target_link_libraries(${PROJECT_NAME} PRIVATE
	spdlog::spdlog_header_only
)
target_link_libraries(${PROJECT_NAME} PRIVATE
	Boost::uuid
	Boost::asio
	Boost::beast
	Boost::preprocessor
)