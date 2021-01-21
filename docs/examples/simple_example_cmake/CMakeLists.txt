cmake_minimum_required(VERSION 2.8)

project(simple_example)

file(GLOB all_sources "*.cpp")
add_executable(${PROJECT_NAME} ${all_sources})

