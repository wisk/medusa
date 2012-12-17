# add the ui directories
add_subdirectory(dummy) # ui for testing purpose

add_subdirectory(semantic-test)

find_package(Qt4 COMPONENTS QtCore QtGui)
if (QT4_FOUND)
  message("INFO: Package qt4 found, medusa_qt will be compiled")

  add_definitions(-DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED) # Workaround for https://bugreports.qt.nokia.com/browse/QTBUG-22829
  add_subdirectory(qt) # Qt4
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
