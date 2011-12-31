# add the ui directories
add_subdirectory(dummy) # ui for testing purpose

find_package(Qt4 COMPONENTS QtCore QtGui)

if (QT4_FOUND)
  message("INFO: Package qt4 found, medusa_qt will be compiled")

  add_definitions(-DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED) # Workaround for https://bugreports.qt.nokia.com/browse/QTBUG-22829
  add_subdirectory(qt) # Qt4
  add_subdirectory(colorpicker) # Qt4 Color Picker
  add_subdirectory(fontpicker) # Qt4 Font Picker
endif()
