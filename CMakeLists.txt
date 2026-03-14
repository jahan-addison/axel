cmake_minimum_required(VERSION 3.16...3.29)

set(CMAKE_POLICY_VERSION_MINIMUM 3.10)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(
    FATAL_ERROR
      "Do not build in-source. Please remove CMakeCache.txt and the CMakeFiles/ directory. Then build out-of-source."
  )
endif()

project(
  lionheart
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(GNUInstallDirs)
include(CMakeDependentOption)
include(cmake/CPM.cmake)

include(GNUInstallDirs)

# ##############################################################################
# DEPENDENCIES
# ##############################################################################

cpmaddpackage("gh:StableCoder/cmake-scripts#24.04")
cpmaddpackage("gh:TheLartians/PackageProject.cmake@1.8.0")

cpmaddpackage("gh:doctest/doctest@2.4.12")

include(${cmake-scripts_SOURCE_DIR}/tools.cmake)

cpmaddpackage(NAME matchit VERSION 1.0.1 GITHUB_REPOSITORY BowenFu/matchit.cpp)
cpmaddpackage("gh:fmtlib/fmt#12.1.0")
cpmaddpackage("gh:yhirose/cpp-peglib#v1.10.0")

# ##############################################################################
# BUILD OPTIONS
# ##############################################################################

if(MSVC)
  set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} /Zc:__cplusplus /utf-8 /DEBUG /D_CRT_SECURE_NO_DEPRECATE /W4 /WX"
  )
else()
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Werror -Wpedantic -DDEBUG")
  else()
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic -O2 -march=native")
  endif()
endif()

# Enable sanitizer with `-DUSE_SANITIZER=<VALUE>` argument to CMake Address,
# Memory, MemoryWithOrigins, Undefined, Thread, Leak, 'Address;Undefined'
if(USE_SANITIZER)
  include(${cmake-scripts_SOURCE_DIR}/sanitizers.cmake)
endif()

# Include-what-you-use https://include-what-you-use.org/

if(IWYU)
  find_program(IWYU_EXECUTABLE include-what-you-use)
  if(IWYU_EXECUTABLE)
    if(APPLE)
      set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE
          "${IWYU_EXECUTABLE};;-isysroot $(xcrun --sdk macosx --show-sdk-path)")
    else()
      set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "${IWYU_EXECUTABLE};;")
    endif()
    message(STATUS "IWYU enabled for C++ targets.")
  else()
    message(
      WARNING "Include-What-You-Use executable not found. IWYU will not be run."
    )
  endif()
endif()

# ##############################################################################
# Executables
# ##############################################################################

file(GLOB_RECURSE headers CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/*.h")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/*.cc")
file(GLOB_RECURSE test_sources CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cc")

add_executable(${PROJECT_NAME} ${sources} ${headers})

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}>
         $<INSTALL_INTERFACE:${PROJECT_NAME}-${PROJECT_VERSION}>)

target_include_directories(${PROJECT_NAME} PUBLIC fmt::fmt matchit peglib)

target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt matchit peglib)

include(cmake/doctest.cmake)
