project(application)

include_directories(.)
include_directories(../3rd-part/cxxopts/include)

file(GLOB_RECURSE all_sources "*.cpp")
list(FILTER all_sources EXCLUDE REGEX "test/*")

add_library(${PROJECT_NAME}_lib STATIC ${all_sources})

target_link_libraries(
    ${PROJECT_NAME}_lib
    parser_lib
    project_lib
    model_includes_lib
    reporter_lib
    project_lib
    file_system_lib
    json_lib
	cmake_project_lib
	compilation_db_lib
	fmt
)

if(CPPINCLUDE_BUILD_WITH_TESTS)
	enable_testing()
	add_subdirectory(test)
endif()
