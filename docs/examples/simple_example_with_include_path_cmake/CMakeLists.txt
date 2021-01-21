cmake_minimum_required(VERSION 2.8)

project(simple_example)

include_directories(lib lib/base lib/a lib/b)

file(GLOB_RECURSE all_sources "*.cpp")
add_executable(${PROJECT_NAME} ${all_sources})

