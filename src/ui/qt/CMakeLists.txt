# We require qt4 library
find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)

add_subdirectory(colorpicker)
add_subdirectory(fontpicker)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/ui/qt/colorpicker)
include_directories(${CMAKE_SOURCE_DIR}/src/ui/qt/fontpicker)

if(WIN32)
  set(GUI_TYPE WIN32)
endif(WIN32)

set(INCROOT  ${CMAKE_SOURCE_DIR}/src/ui/qt)
set(SRCROOT  ${CMAKE_SOURCE_DIR}/src/ui/qt)
set(UIROOT   ${CMAKE_SOURCE_DIR}/src/ui/qt)
set(RSRCROOT ${CMAKE_SOURCE_DIR}/src/ui/qt)
set(TSROOT   ${CMAKE_SOURCE_DIR}/src/ui/qt)

set (SRC
  ${SRCROOT}/About.cpp
  ${SRCROOT}/DisassemblyView.cpp
  ${SRCROOT}/Goto.cpp
  ${SRCROOT}/LoaderChooser.cpp
  ${SRCROOT}/MainWindow.cpp
  ${SRCROOT}/OpenConfirmation.cpp
  ${SRCROOT}/Proxy.cpp
  ${SRCROOT}/Settings.cpp
  ${SRCROOT}/SettingsDialog.cpp
  ${SRCROOT}/main.cpp
)

set (INC
  ${INCROOT}/About.hpp
  ${INCROOT}/DisassemblyView.hpp
  ${INCROOT}/Goto.hpp
  ${INCROOT}/LoaderChooser.hpp
  ${INCROOT}/MainWindow.hpp
  ${INCROOT}/OpenConfirmation.hpp
  ${INCROOT}/Proxy.hpp
  ${INCROOT}/Settings.hpp
  ${INCROOT}/SettingsDialog.hpp
)

# Qt header files (Q_OBJECT files)
set(MOCINC
  ${INCROOT}/DisassemblyView.hpp
  ${INCROOT}/LoaderChooser.hpp
  ${INCROOT}/MainWindow.hpp
  ${INCROOT}/Settings.hpp
  ${INCROOT}/SettingsDialog.hpp
)

# Qt user interface files
set(UI
  ${UIROOT}/About.ui
  ${UIROOT}/Goto.ui
  ${UIROOT}/LoaderChooser.ui
  ${UIROOT}/MainWindow.ui
  ${UIROOT}/OpenConfirmation.ui
  ${UIROOT}/SettingsDialog.ui
)

# Qt resource files
set(RSRC ${RSRCROOT}/MedusaRsrc.qrc)

qt4_wrap_cpp(MOC ${MOCINC})
qt4_wrap_ui(UI_INC ${UI})
qt4_add_resources(RSRC_SRC ${RSRC})
qt4_create_translation(QM ${SRCROOT} ${INCROOT} ${TS})

source_group("Forms"                        FILES ${UI})
source_group("Ressources"                   FILES ${RSRC})
source_group("Generated Files\\ui"          FILES ${UI_INC})
source_group("Generated Files\\moc"         FILES ${MOC})
source_group("Generated Files\\translation" FILES ${QM})

add_executable(qMedusa ${GUI_TYPE}
  ${INC}
  ${SRC}
  ${MOC}
  ${UI_INC}
  ${RSRC_SRC}
  ${QM}
)

target_link_libraries(qMedusa ${QT_LIBRARIES} Medusa ColorPicker)
target_link_libraries(qMedusa ${QT_LIBRARIES} Medusa FontPicker)
install(TARGETS qMedusa RUNTIME DESTINATION .)

if (WIN32)
  foreach (qt_dep QtCore QtGui)
    install(FILES "${QT_LIBRARY_DIR}/${qt_dep}${QT_VERSION_MAJOR}.dll" DESTINATION .)
  endforeach ()
endif ()
