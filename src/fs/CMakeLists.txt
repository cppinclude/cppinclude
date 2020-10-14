project(file_system)

include_directories(.)

file(GLOB all_sources
    "ih/*.cpp"
    "impl/*.cpp"
	"impl/**/*.cpp"
	"resources/*.cpp"
)

add_library( ${PROJECT_NAME}_lib STATIC ${all_sources} )

TARGET_SPECIFIC( ${PROJECT_NAME}_lib )
