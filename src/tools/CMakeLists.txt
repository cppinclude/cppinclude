project(tools)

file(GLOB all_sources "*.cpp" )
add_library( ${PROJECT_NAME}_lib STATIC ${all_sources} )
