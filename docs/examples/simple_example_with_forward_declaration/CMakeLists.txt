cmake_minimum_required(VERSION 2.8)

project(simple_example_with_fowrad_decalration)

file(GLOB all_sources "*.cpp")
add_executable(${PROJECT_NAME} ${all_sources})

