project(tools)

file(GLOB_RECURSE all_sources "*.cpp")
add_library(${PROJECT_NAME}_lib STATIC ${all_sources})

target_link_libraries(${PROJECT_NAME}_lib re2::re2)
