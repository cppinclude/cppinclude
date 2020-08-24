project(project)

include_directories(.)

file(GLOB all_sources
    "ih/*.cpp"
    "impl/*.cpp"
)

add_library( ${PROJECT_NAME}_lib STATIC ${all_sources} )

target_link_libraries(
    ${PROJECT_NAME}_lib
    tools_lib
)

if(CPPINCLUDE_BUILD_WITH_TESTS)
	enable_testing()
	add_subdirectory(test)
endif()
