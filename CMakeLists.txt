cmake_minimum_required(VERSION 3.10)

project(cppinclude
	VERSION 0.4.0
	DESCRIPTION "Tool for analyzing includes in C++"
	LANGUAGES CXX
)

include_directories(src/)
include_directories(src/3rd-part/fmt/include/)
include_directories(src/3rd-part/stdfwd/include/)
include_directories(src/3rd-part/std_fs/include/)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/src/cmake")

if(CMAKE_COMPILER_IS_GNUCC)
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-function")
endif()
if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

option(CPPINCLUDE_BUILD_WITH_TESTS "Build with tests")
option(CPPINCLUDE_BUILD_WITH_CLANG_TIDY "Build with clang-tidy")

if(CPPINCLUDE_BUILD_WITH_CLANG_TIDY)
	set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
endif()

file(GLOB all_sources "src/*.cpp")
add_executable(${PROJECT_NAME} ${all_sources})

if(CPPINCLUDE_BUILD_WITH_TESTS)
	set(BOOST_LIBRARYDIR "/usr/lib/x86_64-linux-gnu/" )
	enable_testing()
	add_subdirectory(src/test_tools)
endif()

if(MSVC)
	if(NOT CPPINCLUDE_BUILD_WITH_TESTS)
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
		set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	endif()
        add_compile_options(/W3 /WX)
endif()

macro(TARGET_SPECIFIC targetName)
	if(CMAKE_COMPILER_IS_GNUCXX)
		target_link_libraries(${targetName} stdc++fs)
	endif()
endmacro()

add_subdirectory(src/parser)
add_subdirectory(src/fs)
add_subdirectory(src/model_includes)
add_subdirectory(src/reporter)
add_subdirectory(src/tools)
add_subdirectory(src/project)
add_subdirectory(src/application)
add_subdirectory(src/3rd-part/fmt)
add_subdirectory(src/json)
add_subdirectory(src/compilation_db)
add_subdirectory(src/cmake_project)

target_link_libraries(${PROJECT_NAME} application_lib)

if(MSVC)
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/INCREMENTAL:NO")
endif()

include(InstallConfig)
include(CPack)

