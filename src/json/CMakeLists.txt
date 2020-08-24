project(json)

include_directories(.)
include_directories(../3rd-part/json)

file(GLOB all_sources
    "ih/*.cpp"
    "impl/*.cpp"
)
add_library( ${PROJECT_NAME}_lib STATIC ${all_sources} )

if(CPPINCLUDE_BUILD_WITH_TESTS)
	enable_testing()
	add_subdirectory(test)
endif()
