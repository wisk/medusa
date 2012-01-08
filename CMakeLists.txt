cmake_minimum_required(VERSION 2.8)

# We only need Debug and Release configuration types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING
  "only Debug and Release configurations are available" FORCE)

# project name
project(Medusa)

# medusa version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

# We need boost
set(Boost_USE_STATIC_LIBS OFF) # We want to link boost dynamically
if (WIN32)
  add_definitions(-DBOOST_ALL_DYN_LINK)
endif()
find_package(Boost 1.46 COMPONENTS filesystem thread date_time REQUIRED)
message("${Boost_LIB_PREFIX}")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY          ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE  ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG    ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY          ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG    ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY          ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG    ${CMAKE_BINARY_DIR}/bin)

# add include directory
include_directories(
  ${CMAKE_SOURCE_DIR}/inc
  ${Boost_INCLUDE_DIRS}
  )

link_directories(
  ${Boost_LIBRARY_DIRS}
  )

if (CMAKE_CXX_COMPILER MATCHES ".*cl.*")
  message("INFO: Microsoft compiler is used. Disabling native wchar_t type")
  add_definitions(/Zc:wchar_t-)
endif()

if (WIN32)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
  add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
  add_definitions(-D_SCL_SECURE_NO_DEPRECATE)
endif()

# add source directory
add_subdirectory(src)

# add python binding directory
find_package(Boost 1.46 COMPONENTS python)
if (Boost_PYTHON_FOUND)
  message("INFO: Package boost.python found, pydusa will be compiled")
  add_subdirectory(python)
endif()

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
