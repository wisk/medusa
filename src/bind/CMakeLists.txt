# add the binding directories

# add python binding directory
find_package(Boost 1.46 COMPONENTS python) ## TODO: we only need to find if boost.python is available
if (Boost_PYTHON_FOUND)
  message(STATUS "Package boost.python found, pydusa will be compiled")
  add_subdirectory(python)
endif()
