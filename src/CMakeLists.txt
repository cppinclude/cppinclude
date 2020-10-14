cmake_minimum_required(VERSION 3.10)

project(cppinclude)

include_directories(.)
include_directories(./3rd-part/fmt/include/)
include_directories(./3rd-part/cpp-std-fwd/)
include_directories(./3rd-part/std_fs/include)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(CPPINCLUDE_BUILD_WITH_TESTS "Build with tests")

file(GLOB all_sources "*.cpp")
add_executable(${PROJECT_NAME} ${all_sources})

if(CPPINCLUDE_BUILD_WITH_TESTS)
	set(BOOST_LIBRARYDIR "/usr/lib/x86_64-linux-gnu/" )
	enable_testing()
	add_subdirectory(test_tools)
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

add_subdirectory(parser)
add_subdirectory(fs)
add_subdirectory(model_includes)
add_subdirectory(reporter)
add_subdirectory(tools)
add_subdirectory(project)
add_subdirectory(application)
add_subdirectory(3rd-part/fmt)
add_subdirectory(json)

target_link_libraries(${PROJECT_NAME} application_lib)

if(MSVC)
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/INCREMENTAL:NO")
endif()

