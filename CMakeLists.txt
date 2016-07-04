cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(GoblinPi)

set(CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_STANDARD 14)

include(GenerateExportHeader)
include(ExternalProject)

include(compile-options.cmake)

# Set runtime path
set(CMAKE_SKIP_BUILD_RPATH            FALSE) # Add absolute path to all dependencies for BUILD
set(CMAKE_BUILD_WITH_INSTALL_RPATH    FALSE) # Use CMAKE_INSTALL_RPATH for INSTALL
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE) # Do NOT add path to dependencies for INSTALL

find_package(Boost REQUIRED COMPONENTS filesystem system)
find_package(SDL2 REQUIRED)

add_subdirectory(engine)
add_subdirectory(example)
