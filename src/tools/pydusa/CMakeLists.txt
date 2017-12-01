include(ExternalProject)

ExternalProject_add(pybind11
  PREFIX            "pybind11"

  GIT_REPOSITORY    "https://github.com/pybind/pybind11"
  GIT_TAG           "v2.0.0-rc1"

  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  INSTALL_COMMAND   ""
)
ExternalProject_get_property(pybind11 SOURCE_DIR)
include_directories("${SOURCE_DIR}/include")

# src: https://github.com/pybind/pybind11/blob/master/docs/compiling.rst

set(MEDUSA_PYTHON_VERSION "" CACHE STRING
  "Python version to use"
)

set(Python_ADDITIONAL_VERSIONS 2.7 3.4 3.5 3.6 3.7)
if (NOT "${MEDUSA_PYTHON_VERSION}" STREQUAL "")
  find_package(PythonLibs "${MEDUSA_PYTHON_VERSION}" EXACT)
  if (NOT PYTHONLIBS_FOUND)
    find_package(PythonLibs "${MEDUSA_PYTHON_VERSION}" REQUIRED)
  endif()
else()
  find_package(PythonLibs REQUIRED)
endif()

string(REPLACE "+" "" PYTHONLIBS_VERSION_STRING "+${PYTHONLIBS_VERSION_STRING}")

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  if (NOT ${CMAKE_BUILD_TYPE} MATCHES DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")

    CHECK_CXX_COMPILER_FLAG("-flto" HAS_LTO_FLAG)
    if (HAS_LTO_FLAG)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
    endif()
  endif()
endif()

include_directories("${PYTHON_INCLUDE_DIR}")

set(SRC
  pydusa.cpp
  py_address.cpp
  py_label.cpp
  py_binary_stream.cpp
  py_expression.cpp
  py_cell.cpp
  py_compiler.cpp
  py_configuration.cpp
  py_memory_area.cpp
  py_database.cpp
  py_exception.cpp
  py_architecture.cpp
  py_loader.cpp
  py_logger.cpp
  py_instruction.cpp
  py_document.cpp
  py_medusa.cpp
  py_context.cpp
  py_execution.cpp
  py_symbolic.cpp
  py_cpu_information.cpp
  py_module.cpp
)

add_library(pydusa SHARED ${SRC})
target_link_libraries(pydusa medusa ${PYTHON_LIBRARY} ${Boost_LIBRARIES})
set_target_properties(
  pydusa
  PROPERTIES
    FOLDER "Tools"
    DEBUG_POSTFIX ""
)
add_dependencies(pydusa pybind11)

set_target_properties(pydusa PROPERTIES PREFIX "")

if(WIN32)
  if (MSVC)
    set_target_properties(pydusa PROPERTIES SUFFIX ".pyd")
  endif()
  install(TARGETS pydusa RUNTIME DESTINATION ${MEDUSA_MODULE_PATH})
else()
  install(TARGETS pydusa LIBRARY DESTINATION ${MEDUSA_MODULE_PATH})
endif()
