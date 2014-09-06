# add the ui directories

add_subdirectory(text)

add_subdirectory(emulator)

if (IS_DIRECTORY ${QT5_CMAKE_PATH})
  message(STATUS "Package qt5 found, qMedusa will be compiled")
  add_subdirectory(qt) # Qt5
endif()

# add python binding directory
find_package(Boost 1.46 COMPONENTS python) ## TODO: we only need to find if boost.python is available
if (Boost_PYTHON_FOUND)
  message(STATUS "Package boost.python found, pydusa will be compiled")
  add_subdirectory(python)
endif()
