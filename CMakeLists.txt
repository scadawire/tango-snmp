cmake_minimum_required(VERSION 3.5)

file(STRINGS version.txt TangoSnmp_VERSION)
message(STATUS "Building TangoSnmp version ${TangoSnmp_VERSION}")

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

# Project configuration, specifying version, languages,
# and the C++ standard to use for the whole project
project(TangoSnmp LANGUAGES CXX VERSION ${TangoSnmp_VERSION})
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/external/cmake-modules)
include(dependencies)
include(lint)
include(CTest)

# External projects
if (BUILD_TESTING)
  add_subdirectory(external/gtest-1.8.1 googletest EXCLUDE_FROM_ALL)
# installed as git submodule - if this is your first clone you need to
# git submodule init
# git submodule update
# This is a cmake module and needs no further input from you
endif()

find_dependencies()

#add_subdirectory(src/ska/helloworld)
add_subdirectory(src/apps)

# Install cmake config + version + target files
include(CMakePackageConfigHelpers)
configure_package_config_file(
  cmake/TangoSnmpConfig.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/TangoSnmpConfig.cmake"
  INSTALL_DESTINATION
    share/helloworld/cmake
)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/TangoSnmpVersion.cmake"
  COMPATIBILITY
    AnyNewerVersion
)

#install(EXPORT helloworld-targets   FILE     HelloWorldTargets.cmake   DESTINATION     share/helloworld/cmake   COMPONENT     dev   NAMESPACE     helloworld:: )

install(
    PROGRAMS
        ${EXECUTABLE_OUTPUT_PATH}/TangoSnmp
    DESTINATION
        bin
    COMPONENT
        runtime
)
