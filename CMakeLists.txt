cmake_minimum_required(VERSION 2.8.10)

# project name
project(Medusa)

# medusa version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 4)
set(VERSION_PATCH 4)

enable_testing()

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

configure_file(
  ${CMAKE_SOURCE_DIR}/inc/medusa/version.hpp.in
  ${CMAKE_SOURCE_DIR}/inc/medusa/version.hpp
)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

if (NOT EXISTS "${CMAKE_SOURCE_DIR}/deps/ogdf/")
  message(FATAL_ERROR "OGDF folder is missing, please run git submodule update --init --recursive")
endif()
add_subdirectory("${CMAKE_SOURCE_DIR}/deps/ogdf")
include_directories("${CMAKE_SOURCE_DIR}/deps/ogdf/include")

# We need boost
set(Boost_USE_STATIC_LIBS OFF) # We want to link boost dynamically
if (MSVC)
  add_definitions(-DBOOST_ALL_DYN_LINK)
endif()
find_package(Boost 1.55 COMPONENTS system filesystem thread date_time program_options unit_test_framework REQUIRED)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY          ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY          ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY          ${CMAKE_BINARY_DIR}/bin)

# add include directory
include_directories(
  ${CMAKE_SOURCE_DIR}/inc
  ${Boost_INCLUDE_DIRS}
  )

link_directories(
  ${Boost_LIBRARY_DIRS}
  )

# Found it in CMakeLists.txt from keepassx project
if("${CMAKE_CXX_COMPILER}" MATCHES "clang(\\+\\+)?$" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_COMPILER_IS_CLANGXX 1)
endif()

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
  add_definitions("-std=c++11")
endif()

# src: ogdf cmake
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  add_definitions(
    "/D_CRT_SECURE_NO_WARNINGS"
    "/D_SCL_SECURE_NO_WARNINGS"
    "/wd4251")
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
