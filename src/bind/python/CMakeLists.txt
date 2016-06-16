set(MEDUSA_PYTHON_VERSION "" CACHE STRING
  "Python version to use for compiling pydusa"
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

add_library(bind_python SHARED bind_python.cpp bind_python.hpp)
target_link_libraries(bind_python medusa ${PYTHON_LIBRARY})
set_target_properties(bind_python PROPERTIES
  FOLDER "Bindings"
  PREFIX "")

if(WIN32)
  install(TARGETS bind_python RUNTIME DESTINATION lib)
else()
  install(TARGETS bind_python LIBRARY DESTINATION lib)
endif()
