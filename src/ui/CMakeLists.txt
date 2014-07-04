# add the ui directories

add_subdirectory(text)

add_subdirectory(dummy) # ui for testing purpose

add_subdirectory(semantic-test)

if (IS_DIRECTORY ${QT5_CMAKE_PATH})
  message("INFO: Package qt5 found, qMedusa will be compiled")
  add_subdirectory(qt) # Qt5
endif()

# add python binding directory
find_package(Boost 1.46 COMPONENTS python)
if (Boost_PYTHON_FOUND)
  message("INFO: Package boost.python found, pydusa will be compiled")
  add_subdirectory(python)
endif()
