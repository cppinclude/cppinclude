project(test_tools)

find_package (Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(. ${Boost_INCLUDE_DIRS})

file(GLOB all_sources "*.cpp" )
add_library( ${PROJECT_NAME}_lib STATIC ${all_sources} )
