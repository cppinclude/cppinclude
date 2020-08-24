project(file_system)

include_directories(.)

file(GLOB fs_sources
    "ih/*.cpp"
    "impl/*.cpp"
	"impl/**/*.cpp"
	"resources/*.cpp"
)

add_library( ${PROJECT_NAME}_lib STATIC ${fs_sources} )
