cmake_minimum_required(VERSION 2.8.10)
# project name
project(Medusa)

# medusa version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 4)
set(VERSION_PATCH 3)

configure_file(
  ${CMAKE_SOURCE_DIR}/inc/medusa/version.hpp.in
  ${CMAKE_SOURCE_DIR}/inc/medusa/version.hpp
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

OPTION(BUILD_TESTING "Build the testing" OFF)
OPTION(ENABLE_CCACHE "Enabled ccache for compilation" OFF)

# Open Graph Drawing Framework
find_package(OGDF)

# We need boost
set(Boost_USE_STATIC_LIBS OFF) # We want to link boost dynamically
if (MSVC)
  add_definitions(-DBOOST_ALL_DYN_LINK)
endif()
find_package(Boost 1.46 COMPONENTS system filesystem thread date_time REQUIRED)

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

if(ENABLE_CCACHE)
MESSAGE(INFO ": Enable ccache for compilation option.Try found ccache")
find_package(CCache REQUIRED)
  if (CCACHE_FOUND)
    MESSAGE(INFO ": CCache found")
    SET(CCACHE "ccache" CACHE STRING "ccache")
    SET(CMAKE_C_COMPILER_ARG1 ${CMAKE_C_COMPILER})
    SET(CMAKE_C_COMPILER ${CCACHE})
    SET(CMAKE_CXX_COMPILER_ARG1 ${CMAKE_CXX_COMPILER})
    SET(CMAKE_CXX_COMPILER ${CCACHE})
    SET(CMAKE_C_FLAGS "${CCACHE_FLAGS} ${CMAKE_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CCACHE_FLAGS} ${CMAKE_CXX_FLAGS}")
  else(CCACHE_FOUND)
    MESSAGE(ERROR ": Error.CCache no found.")
  endif(CCACHE_FOUND)
endif(ENABLE_CCACHE)
#TODO Add MSVC support
if(BUILD_TESTING)
enable_testing()
MESSAGE(INFO ": Build the testing.Tests will be compiled")
find_package(GMock REQUIRED)
if (GMOCK_FOUND)
   MESSAGE(INFO ": GMOCK_FOUND")
   MESSAGE(INFO ": GMOCK_INCLUDE_DIRS = ${GMOCK_INCLUDE_DIRS}")
   include_directories(${GMOCK_INCLUDE_DIRS})
   LINK_DIRECTORIES(${GMOCK_BOTH_LIBRARIES})
else(GMOCK_FOUND)
   MESSAGE(ERROR ": Error.GMOCK no found.")
endif(GMOCK_FOUND)

find_package(GTest REQUIRED)
if (GTEST_FOUND)
   MESSAGE(INFO ": GTest_FOUND")
   MESSAGE(INFO ": GMOCK_INCLUDE_DIRS = ${GTEST_INCLUDE_DIRS}")
   include_directories(${GTEST_INCLUDE_DIRS})
   LINK_DIRECTORIES(${GTEST_BOTH_LIBRARIES})
else(GTEST_FOUND)
   MESSAGE(ERROR ": Error.GTest no found.")
endif(GTEST_FOUND)


ENDIF(BUILD_TESTING)

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
