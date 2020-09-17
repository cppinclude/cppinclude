project(file_system)

include_directories(.)

file(GLOB fs_sources
    "ih/*.cpp"
    "impl/*.cpp"
	"impl/**/*.cpp"
	"resources/*.cpp"
)

add_library( ${PROJECT_NAME}_lib STATIC ${fs_sources} )

# GCC requires explicit linking to the <filesystem> static lib
if(CMAKE_COMPILER_IS_GNUCXX)
    target_link_libraries(${PROJECT_NAME}_lib stdc++fs)
endif()
