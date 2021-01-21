project(model_includes)

include_directories(.)

file(GLOB_RECURSE all_sources "*.cpp")
list(FILTER all_sources EXCLUDE REGEX "test/*")

add_library( ${PROJECT_NAME}_lib STATIC ${all_sources} )

target_link_libraries(
	${PROJECT_NAME}_lib
	parser_lib
	project_lib
)

if(CPPINCLUDE_BUILD_WITH_TESTS)
	enable_testing()
	add_subdirectory(test)
endif()
