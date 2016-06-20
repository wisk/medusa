# add the ui directories

add_subdirectory(text)

add_subdirectory(pydusa)

if (IS_DIRECTORY ${QT5_CMAKE_PATH})
  message(STATUS "Package qt5 found, qMedusa will be compiled")
  add_subdirectory(qt) # Qt5
endif()
