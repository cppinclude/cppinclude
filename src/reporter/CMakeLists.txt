project(reporter)

include_directories(.)

file(GLOB all_sources
	"ih/*.cpp"
	"impl/*.cpp"
	"impl/**/*.cpp"
	"resources/*.cpp"
)
add_library( ${PROJECT_NAME}_lib STATIC ${all_sources} )

target_link_libraries(
	${PROJECT_NAME}_lib
	fmt
)

if(CPPINCLUDE_BUILD_WITH_TESTS)
	enable_testing()
	add_subdirectory(test)
endif()
