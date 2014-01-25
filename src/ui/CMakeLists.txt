# add the ui directories
add_subdirectory(dummy) # ui for testing purpose

add_subdirectory(semantic-test)

if (IS_DIRECTORY ${QT5_CMAKE_PATH})
  message("INFO: Package qt5 found, qMedusa will be compiled")
  add_subdirectory(qt) # Qt5
endif()

#find_package(wxWidgets COMPONENTS stc richtext aui xrc adv core base)
#if (wxWidgets_FOUND)
#  message("INFO: Package wxWidgets found, wxMedusa will be compiled")
#
#  add_subdirectory(wxwidgets)
#endif()

# add python binding directory
#find_package(Boost 1.46 COMPONENTS python)
#if (Boost_PYTHON_FOUND)
#  message("INFO: Package boost.python found, pydusa will be compiled")
#  add_subdirectory(python)
#endif()
