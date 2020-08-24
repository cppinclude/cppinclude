project(parser)

include_directories(.)

file(GLOB parser_sources
    "ih/*.cpp"
    "impl/*.cpp"
    "resources/*.cpp"
)
add_library( ${PROJECT_NAME}_lib STATIC ${parser_sources} )

if(CPPINCLUDE_BUILD_WITH_TESTS)
	enable_testing()
	add_subdirectory(test)
endif()
