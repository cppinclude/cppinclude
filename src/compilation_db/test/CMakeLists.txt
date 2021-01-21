file(GLOB_RECURSE all_sources "*.cpp" )
add_executable(${PROJECT_NAME}_test ${all_sources})

include(TestConfig)

target_link_libraries(
	${PROJECT_NAME}_test
	${Boost_LIBRARIES}
	${PROJECT_NAME}_lib
	json_lib
)

add_test(NAME ${PROJECT_NAME}_test COMMAND ${PROJECT_NAME}_test)

