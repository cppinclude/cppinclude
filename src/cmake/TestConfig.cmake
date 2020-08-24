find_package (Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(. ${Boost_INCLUDE_DIRS})

if(MSVC)
    set_target_properties(${PROJECT_NAME}_test PROPERTIES LINK_FLAGS "/INCREMENTAL:NO")
endif()
