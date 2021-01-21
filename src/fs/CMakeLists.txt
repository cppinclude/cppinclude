project(file_system)

include_directories(.)

file(GLOB_RECURSE all_sources "*.cpp")
add_library(${PROJECT_NAME}_lib STATIC ${all_sources})

TARGET_SPECIFIC(${PROJECT_NAME}_lib)
