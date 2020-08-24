project(application)

include_directories(.)
include_directories(../3rd-part/cxxopts/include)

file(GLOB all_sources
    "*.cpp"
    "tools/*.cpp"
    "resources/*.cpp"
	"exceptions/*.cpp"
)
add_library( ${PROJECT_NAME}_lib STATIC ${all_sources} )

target_link_libraries(
    ${PROJECT_NAME}_lib
    parser_lib
    project_lib
    model_includes_lib
    reporter_lib
    project_lib
    file_system_lib
    json_lib
	fmt
)

if(CPPINCLUDE_BUILD_WITH_TESTS)
	enable_testing()
	add_subdirectory(test)
endif()
