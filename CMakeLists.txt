cmake_minimum_required(VERSION 3.2)

# Name

project(Medusa)

# Version

set(VERSION_MAJOR 0)
set(VERSION_MINOR 5)
set(VERSION_PATCH 4)

# Variables

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(MEDUSA_MODULES_TO_BUILD "all"
  CACHE STRING "Semicolon-separated list of modules to build, or \"all\"")
list(REMOVE_DUPLICATES MEDUSA_MODULES_TO_BUILD)
list(SORT MEDUSA_MODULES_TO_BUILD)

if ("${MEDUSA_MODULES_TO_BUILD}" MATCHES "all")
  message("Compile all modules")
else()
  foreach (medusa_module ${MEDUSA_MODULES_TO_BUILD})
    # TODO(wisk): check if the module if correct
    message("Compile module ${medusa_module}")
  endforeach()
endif()

set(MEDUSA_EXECUTABLE_PATH ".")
set(MEDUSA_LIBRARY_PATH lib)
set(MEDUSA_MODULE_PATH lib/medusa)
set(MEDUSA_RESOURCE_PATH share/medusa)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Options

## Tools
option(MEDUSA_BUILD_TOOLS
  "Build Medusa tools"
  TRUE)

## Tests
option(MEDUSA_BUILD_TESTS
  "Build Medusa tests"
  TRUE)
if (MEDUSA_BUILD_TESTS)
  enable_testing()
endif()

# Features

## C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG(-fabi-version=9 HAVE_ABI_VERSION)
if (HAVE_ABI_VERSION)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fabi-version=9")
endif()
# TODO(wisk): set default visiblity as hidden on ELF
## CHECK_CXX_COMPILER_FLAG(-fvisibility=hidden HAVE_VISIBILITY_HIDDEN)
## if (HAVE_VISIBILITY_HIDDEN)
##   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
## endif()
## CHECK_CXX_COMPILER_FLAG(-fvisibility-inlines-hidden HAVE_VISIBILITY_INLINES_HIDDEN)
## if (HAVE_VISIBILITY_INLINES_HIDDEN)
##   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility-inlines-hidden")
## endif()

# src: ogdf cmake
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  add_definitions(
    "/DNOMINMAX"
    "/D_CRT_SECURE_NO_WARNINGS"
    "/D_SCL_SECURE_NO_WARNINGS"
    "/wd4251" "/wd4503"
    "/Zm256")
endif()

# Sources
add_subdirectory(src)

# Packaging
add_subdirectory(package)

find_package(Doxygen)
if (DOXYGEN_FOUND)
  message("Doxygen found, I'll generate documentations for you")
  set(DOCROOT "${CMAKE_SOURCE_DIR}/doc/api")

  configure_file("${DOCROOT}/Doxyfile.in" "${DOCROOT}/Doxyfile" @ONLY)

  set(DOC_TARGET_NAME "doc")
  if (MSVC_IDE)
    set(DOC_TARGET_NAME "DOCUMENTATION")
  endif()

  add_custom_target(${DOC_TARGET_NAME}
    ${DOXYGEN_EXECUTABLE} ${DOCROOT}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with doxygen" VERBATIM
    )
endif()