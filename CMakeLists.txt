# CMakeList.txt: PenFramework 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.10)

project ("PenFramework")

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message("Enable MSVC standard preprocessor")
    add_compile_options("/Zc:preprocessor")
endif()

find_package(spdlog REQUIRED)
find_package(Boost REQUIRED
    COMPONENTS
        locale
)

file(GLOB_RECURSE PRO_CPP ${CMAKE_CURRENT_SOURCE_DIR}/Code/*.cpp)
file(GLOB_RECURSE PRO_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/Code/*.h)
file(GLOB_RECURSE PRO_HPP ${CMAKE_CURRENT_SOURCE_DIR}/Code/*.hpp)

set(PROJECT_FILES
    ${PRO_CPP}
    ${PRO_HEADER}
    ${PRO_HPP}
)

add_executable (PenFramework ${PROJECT_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE Boost::locale)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog_header_only)