cmake_minimum_required(VERSION 3.12)

# -- DETECT: If this is the MASTER_PROJECT or used within other CMake build.
set(MASTER_PROJECT OFF)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(MASTER_PROJECT ON)
endif()

project(cppinclude VERSION 0.2.1 LANGUAGES CXX)

include_directories(.)
include_directories(./3rd-part/fmt/include/)
include_directories(./3rd-part/cpp-std-fwd/)
include_directories(./3rd-part/std_fs/include)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(CPPINCLUDE_BUILD_WITH_TESTS "Build with tests")

file(GLOB all_sources "*.cpp")
add_executable(${PROJECT_NAME} ${all_sources})
add_executable(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

if(CPPINCLUDE_BUILD_WITH_TESTS)
	set(BOOST_LIBRARYDIR "/usr/lib/x86_64-linux-gnu/" )
	enable_testing()
	add_subdirectory(test_tools)
endif()

if(MSVC)
	if(NOT CPPINCLUDE_BUILD_WITH_TESTS)
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
		set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	endif()
        add_compile_options(/W3 /WX)
endif()

macro(TARGET_SPECIFIC targetName)
	if(CMAKE_COMPILER_IS_GNUCXX)
		target_link_libraries(${targetName} stdc++fs)
	endif()
endmacro()

add_subdirectory(parser)
add_subdirectory(fs)
add_subdirectory(model_includes)
add_subdirectory(reporter)
add_subdirectory(tools)
add_subdirectory(project)
add_subdirectory(application)
add_subdirectory(3rd-part/fmt)
add_subdirectory(json)

target_link_libraries(${PROJECT_NAME} application_lib)

if(MSVC)
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/INCREMENTAL:NO")
endif()


# ---------------------------------------------------------------------------
# SECTION: install  -- Install build artifacts
# ---------------------------------------------------------------------------
# SEE: https://crascit.com/tag/cmake/
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if(NOT DEFINED CMAKE_INSTALL_PREFIX AND NOT WIN32 AND MASTER_PROJECT)
    # -- SCHEMA: /opt/<provider>/<package> (normally; simplified here)
    set(CMAKE_INSTALL_PREFIX "/opt/${PROJECT_NAME}")
    # MAYBE: set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
message(STATUS "USING: CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")

set(PROJECT_NAMESPACE ${PROJECT_NAME})
set(cppinclude_EXECUTABLES  ${PROJECT_NAME})
set(cppinclude_LIBRARIES )
install(TARGETS ${cppinclude_EXECUTABLES} ${cppinclude_LIBRARIES}
    EXPORT ${PROJECT_NAMESPACE}-targets
    RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT ${PROJECT_NAME}_runtime
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT ${PROJECT_NAME}_runtime
        NAMELINK_COMPONENT ${PROJECT_NAME}_develop
    ARCHIVE
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT ${PROJECT_NAME}_develop
    # MAYBE: CONFIGURATIONS Release RelWithDebInfo MinSizeRel
)
install(EXPORT ${PROJECT_NAMESPACE}-targets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAMESPACE}
        NAMESPACE   ${PROJECT_NAMESPACE}::
)
# install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/application
#         DESTINATION include
#         FILES_MATCHING PATTERN "*.hpp"
#         PATTERN ".cmake" EXCLUDE
# )

set(cmakeModulesDir cmake)
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAMESPACE}-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAMESPACE}-version.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAMESPACE}")


configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/.cmake_project-config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAMESPACE}-config.cmake"
    INSTALL_DESTINATION "lib/cmake/${PROJECT_NAMESPACE}"
    PATH_VARS cmakeModulesDir
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAMESPACE}-config.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAMESPACE}")


# ----------------------------------------------------------------------------
# SECTION: pack/cpack -- Build binary-bundle or source-bundle packages w/ cpack
# ----------------------------------------------------------------------------
# HINT: Source-pack includes CMAKE_CURRENT_BUILDIR, ...
# set(CPACK_SOURCE_GENERATOR "ZIP")
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VENDOR "cppinclude")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Tool for analyzing includes in C++.")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}_sourcepack")
set(CPACK_SOURCE_IGNORE_FILES "build/;build.*/;.git/;.vscode/;.idea/;\.DS_Store;*.user")
if(CPPINCLUDE_CPACK_SOURCE_IGNORE_THIRD_PARTY)
    message("CPACK-SOURCE: Ignore bundled third-party libraries.")
    set(CPACK_SOURCE_IGNORE_FILES "${CPACK_SOURCE_IGNORE_FILES};3rd-part/")
endif()

set(CPACK_VERBATIM_VARIABLES ON)
set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_LIST_DIR}/../README.md)
# MAYBE (missing): set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_LIST_FILE}/../LICENSE)
# MAYBE: set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_LIST_FILE}/../DESCRIPTION.md)

include(CPack)
