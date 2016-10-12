cmake_minimum_required(VERSION 3.1)

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/wisk/hunter/archive/medusa-deps.tar.gz"
    SHA1 "7b6ced4c4cb8cada49f4572fff69a01629c5c61c"
)

# project name

project(Medusa)

# medusa version

set(VERSION_MAJOR 0)
set(VERSION_MINOR 4)
set(VERSION_PATCH 4)

# variables

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

set(MEDUSA_EXECUTABLE_PATH bin)
set(MEDUSA_LIBRARY_PATH lib)
set(MEDUSA_MODULE_PATH lib/medusa)
set(MEDUSA_RESOURCE_PATH share/medusa)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# options

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

## Cotire
option(MEDUSA_CMAKE_USE_COTIRE
  "use cotire"
  FALSE)
if (MEDUSA_CMAKE_USE_COTIRE)
  include(${CMAKE_SOURCE_DIR}/cmake/cotire.cmake) # https://github.com/sakra/cotire
  message("Cotire was enabled")
endif()

## Boost
add_definitions("-DBOOST_ALL_NO_LIB=1") # HACK(wisk): disable auto-link
option(MEDUSA_BUILD_WITH_BOOST
  "Boost library is required to build medusa"
  TRUE)
if (MEDUSA_BUILD_WITH_BOOST)
  hunter_add_package(Boost COMPONENTS system atomic filesystem program_options)
endif()

find_package(Boost COMPONENTS system atomic filesystem program_options REQUIRED)
if (NOT Boost_FOUND)
  message(FATAL_ERROR "Unable to find Boost")
endif()

## OGDF
option(MEDUSA_BUILD_WITH_OGDF
  "OGDF library allows medusa to display graph"
  TRUE)
if (MEDUSA_BUILD_WITH_OGDF)
  hunter_add_package(OGDF)
endif()

## LLVM
option(MEDUSA_BUILD_WITH_LLVM
  "LLVM library allows medusa to compile to code in the module emul_llvm"
  TRUE)
if (MEDUSA_BUILD_WITH_LLVM)
endif()

## QT5
option(MEDUSA_BUILD_WITH_QT
  "Qt library allows to build qMedusa tool"
  TRUE)
if (MEDUSA_BUILD_WITH_QT)
  hunter_add_package(Qt)
endif()

## SQLite3
option(MEDUSA_BUILD_WITH_SQLITE3
  "SQLite3 library allows to build db_soci"
  TRUE)
if (MEDUSA_BUILD_WITH_SQLITE3)
  if (MSVC)
    hunter_add_package(SQLite3-cmake)
  else()
    hunter_add_package(SQLite3)
  endif()
endif()

## SOCI
option(MEDUSA_BUILD_WITH_SOCI
  "SOCI library allows to build db_soci"
  TRUE)
if (MEDUSA_BUILD_WITH_SOCI)
  hunter_add_package(SOCI)
endif()


set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# features

include(ExternalProject)

## C++
set(CMAKE_CXX_STANDARD 11)
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

# add source directory
add_subdirectory(src)

# add packaging directory
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

configure_file(
  ${CMAKE_SOURCE_DIR}/inc/medusa/version.hpp.in
  ${CMAKE_BINARY_DIR}/inc/medusa/version.hpp
)
