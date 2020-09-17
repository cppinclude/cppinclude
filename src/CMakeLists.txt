cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)

project(cppinclude)

include_directories(.)
include_directories(./3rd-part/fmt/include/)
include_directories(./3rd-part/cpp-std-fwd/)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

option(CPPINCLUDE_BUILD_WITH_TESTS "Build with tests")

file(GLOB all_sources "*.cpp")
add_executable(${PROJECT_NAME} ${all_sources})

if(CPPINCLUDE_BUILD_WITH_TESTS)
  set(BOOST_LIBRARYDIR "/usr/lib/x86_64-linux-gnu/" )
  enable_testing()
  add_subdirectory(test_tools)
endif()

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

# GCC requires explicit linking to the <filesystem> static lib
if(CMAKE_COMPILER_IS_GNUCXX)
    target_link_libraries(${PROJECT_NAME} stdc++fs)
endif()

if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/INCREMENTAL:NO")
endif()
