# add_subdirectory(text) # TODO: update to current ModuleManager/Medusa API

find_package(Python COMPONENTS Interpreter Development QUIET)
find_package(nanobind CONFIG QUIET)
if (Python_FOUND AND nanobind_FOUND)
  message(STATUS "Python and nanobind found, pydusa will be compiled")
  add_subdirectory(pydusa)
else()
  message(STATUS "Python or nanobind not found, pydusa will NOT be compiled")
endif()

find_package(Qt5 COMPONENTS Core Widgets)
if (Qt5_FOUND)
  message(STATUS "Qt found, qMedusa will be compiled")
  add_subdirectory(qt)
endif()
