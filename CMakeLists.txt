cmake_minimum_required(VERSION 3.1)

# project name
project(Medusa)

# medusa version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 4)
set(VERSION_PATCH 4)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
#include(cotire) # https://github.com/sakra/cotire

enable_testing()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (MSVC)
  # https://stackoverflow.com/questions/9742003/platform-detection-in-cmake
  macro(GET_WIN32_WINNT version)
    if (WIN32 AND CMAKE_SYSTEM_VERSION)
      set(ver ${CMAKE_SYSTEM_VERSION})
      string(REPLACE "." "" ver ${ver})
      string(REGEX REPLACE "([0-9])" "0\\1" ver ${ver})

      set(${version} "0x${ver}")
    endif()
  endmacro()

  GET_WIN32_WINNT(WIN_VER)
  add_definitions(-D_WIN32_WINNT=${WIN_VER})
  message(STATUS "MS Visual Studio generator detected, define _WIN32_WINNT=${WIN_VER}")
endif()

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# features

## C++
set(CMAKE_CXX_STANDARD 11)
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG(-fabi-version=6 HAVE_ABI_VERSION)
if (HAVE_ABI_VERSION)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fabi-version=6")
endif()

## Python
set(MEDUSA_PYTHON_VERSION 2.7)

## Boost
set(MEDUSA_BOOST_MIN_VERSION 1.55)

set(Boost_USE_STATIC_LIBS OFF) # We want to link boost dynamically
add_definitions(-DBOOST_ALL_DYN_LINK)
find_package(Boost ${MEDUSA_BOOST_MIN_VERSION} COMPONENTS system filesystem thread date_time program_options unit_test_framework REQUIRED)

## OGDF
option(MEDUSA_BUILD_WITH_OGDF
  "OGDF library allows medusa to display graph"
  FALSE)

if (MEDUSA_BUILD_WITH_OGDF)
  message("Build medusa with OGDF")

  if (NOT EXISTS "${CMAKE_SOURCE_DIR}/deps/ogdf")
    find_package(Git)
    if (NOT GIT_FOUND)
      message(FATAL_ERROR "Unable to find Git executable")
    endif()

  execute_process(
    COMMAND "${GIT_EXECUTABLE}" clone https://github.com/wisk/ogdf
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/deps")
  endif()

  if (NOT EXISTS "${CMAKE_SOURCE_DIR}/deps/ogdf/")
    message(FATAL_ERROR "OGDF folder is missing")
  endif()
  add_subdirectory("${CMAKE_SOURCE_DIR}/deps/ogdf")
  include_directories("${CMAKE_SOURCE_DIR}/deps/ogdf/include")
  set_target_properties(coin PROPERTIES FOLDER "Dependencies")
  set_target_properties(ogdf PROPERTIES FOLDER "Dependencies")
  set_target_properties(ogdf-test PROPERTIES FOLDER "Dependencies")

  set(MEDUSA_HAS_OGDF "#define MEDUSA_HAS_OGDF")

endif()

# all generated files are going to bin folder
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# add include directory
include_directories(
  ${CMAKE_SOURCE_DIR}/inc
  ${CMAKE_BINARY_DIR}/inc
  ${Boost_INCLUDE_DIRS}
  )

link_directories(
  ${Boost_LIBRARY_DIRS}
  )

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
  set(DOCROOT ${CMAKE_SOURCE_DIR}/doc/api)

  configure_file(${DOCROOT}/Doxyfile.in ${DOCROOT}/Doxyfile @ONLY)

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
