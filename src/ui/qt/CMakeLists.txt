# We require qt4 library
find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/ui/colorpicker)
include_directories(${CMAKE_SOURCE_DIR}/src/ui/fontpicker)

if(WIN32)
  set(GUI_TYPE WIN32)
endif(WIN32)

set(INCROOT  ${CMAKE_SOURCE_DIR}/src/ui/qt)
set(SRCROOT  ${CMAKE_SOURCE_DIR}/src/ui/qt)
set(UIROOT   ${CMAKE_SOURCE_DIR}/src/ui/qt)
set(RSRCROOT ${CMAKE_SOURCE_DIR}/src/ui/qt)
set(TSROOT	 ${CMAKE_SOURCE_DIR}/src/ui/qt)


# FILES LIST START HERE

###############################
# MEDUSA EDITOR
###############################

#######
# CORE
#######
set (EDITORSRC
  ${SRCROOT}/MedusaEditor.cpp
  ${SRCROOT}/IMedusaItem.cpp
  ${SRCROOT}/IMedusaPrinter.cpp
  ${SRCROOT}/MedusaPrinterPool.cpp
  ${SRCROOT}/MedusaEditorLineCounter.cpp
)

set (EDITORINC
  ${INCROOT}/MedusaEditor.hpp
  ${INCROOT}/IMedusaItem.hpp
  ${INCROOT}/IMedusaPrinter.hpp
  ${INCROOT}/MedusaPrinterPool.hpp
  ${INCROOT}/MedusaEditorLineCounter.hpp
)

#######
# ITEM
#######
set (EDITORITEMSRC
  ${SRCROOT}/Instruction.cpp
  ${SRCROOT}/MemoryAreaHeader.cpp
  ${SRCROOT}/Function.cpp
)

set (EDITORITEMINC
  ${INCROOT}/Instruction.hpp
  ${INCROOT}/MemoryAreaHeader.hpp
  ${INCROOT}/Function.hpp
)

#######
# PRINTER
#######
set (EDITORPRINTERSRC
  ${SRCROOT}/InstructionPrinter.cpp
  ${SRCROOT}/MemoryAreaHeaderPrinter.cpp
  ${SRCROOT}/FunctionPrinter.cpp
)

set (EDITORPRINTERINC
  ${INCROOT}/InstructionPrinter.hpp
  ${INCROOT}/MemoryAreaHeaderPrinter.hpp
  ${INCROOT}/FunctionPrinter.hpp
)

#######
# UTILS
#######
set (EDITORUTILSSRC
  ${SRCROOT}/MedusaFont.cpp
  ${SRCROOT}/MedusaHtml.cpp
  ${SRCROOT}/MedusaCollapse.cpp
)

set (EDITORUTILSINC
  ${INCROOT}/MedusaFont.hpp
  ${INCROOT}/MedusaHtml.hpp
  ${INCROOT}/MedusaCollapse.hpp
  ${INCROOT}/MedusaCollection.hpp
)

#######
# ACTION
#######
set (EDITORACTIONSRC
  ${SRCROOT}/JumpAction.cpp
)

set (EDITORACTIONINC
  ${INCROOT}/JumpAction.hpp
)


###############################
# MEDUSA QT
###############################

# Source files
set(QTSRC
  ${SRCROOT}/main.cpp
  ${SRCROOT}/MainWindow.cpp
  ${SRCROOT}/About.cpp
  ${SRCROOT}/OpenConfirmation.cpp
  ${SRCROOT}/LoaderChooser.cpp
  ${SRCROOT}/Loader.cpp
  ${SRCROOT}/UnLoader.cpp
  ${SRCROOT}/Goto.cpp
  ${SRCROOT}/Settings.cpp
  ${SRCROOT}/SettingsDialog.cpp
)

# Header files
set(QTINC
  ${INCROOT}/MainWindow.hpp
  ${INCROOT}/About.hpp
  ${INCROOT}/OpenConfirmation.hpp
  ${INCROOT}/LoaderChooser.hpp
  ${INCROOT}/Loader.hpp
  ${INCROOT}/UnLoader.hpp
  ${INCROOT}/About.hpp
  ${INCROOT}/OpenConfirmation.hpp
  ${INCROOT}/LoaderChooser.hpp
  ${INCROOT}/Goto.hpp
  ${INCROOT}/Settings.hpp
  ${INCROOT}/SettingsDialog.hpp
)

###############################
# OTHER
###############################

# Source files
set(SRC
  ${QTSRC}
  ${EDITORSRC}
  ${EDITORUTILSSRC}
  ${EDITORITEMSRC}
  ${EDITORPRINTERSRC}
  ${EDITORACTIONSRC}
)

# Header files
set(INC
  ${QTINC}
  ${EDITORINC}
  ${EDITORUTILSINC}
  ${EDITORITEMINC}
  ${EDITORPRINTERINC}
  ${EDITORACTIONINC}
)

# Qt header files (Q_OBJECT files)
set(MOCINC
  ${INCROOT}/MainWindow.hpp
  ${INCROOT}/SettingsDialog.hpp
  ${INCROOT}/MedusaEditor.hpp
  ${INCROOT}/Settings.hpp
  ${INCROOT}/LoaderChooser.hpp
  ${INCROOT}/MedusaCollapse.hpp
  ${INCROOT}/IMedusaPrinter.hpp
  ${EDITORPRINTERINC}
)

# QT user interface files
set(UI
  ${UIROOT}/MainWindow.ui
  ${UIROOT}/About.ui
  ${UIROOT}/OpenConfirmation.ui
  ${UIROOT}/LoaderChooser.ui
  ${UIROOT}/Goto.ui
  ${UIROOT}/SettingsDialog.ui
)

# Qt resource files
set(RSRC ${RSRCROOT}/medusa_qt.qrc)

# Qt translation files
set(TS  ${TSROOT}/medusa_fr.ts)


# FILES LIST END HERE

qt4_wrap_cpp(MOC ${MOCINC})
qt4_wrap_ui(UI_INC ${UI})
qt4_add_resources(RSRC_SRC ${RSRC})
qt4_create_translation(QM ${SRCROOT} ${INCROOT} ${TS})

# Grouping files
source_group("Header Files\\UI" FILES ${QTINC})
source_group("Source Files\\UI" FILES ${QTSRC})

source_group("Header Files\\MedusaEditor\\Core" FILES ${EDITORINC})
source_group("Source Files\\MedusaEditor\\Core" FILES ${EDITORSRC})

source_group("Header Files\\MedusaEditor\\Items\\Items" FILES ${EDITORITEMINC})
source_group("Source Files\\MedusaEditor\\Items\\Items" FILES ${EDITORITEMSRC})

source_group("Header Files\\MedusaEditor\\Items\\Printers" FILES ${EDITORPRINTERINC})
source_group("Source Files\\MedusaEditor\\Items\\Printers" FILES ${EDITORPRINTERSRC})

source_group("Header Files\\MedusaEditor\\Utils" FILES ${EDITORUTILSINC})
source_group("Source Files\\MedusaEditor\\Utils" FILES ${EDITORUTILSSRC})

source_group("Header Files\\MedusaEditor\\Actions" FILES ${EDITORACTIONINC})
source_group("Source Files\\MedusaEditor\\Actions" FILES ${EDITORACTIONSRC})

source_group("Forms" FILES ${UI})
source_group("Ressources" FILES ${RSRC})

source_group("Generated Files\\ui" FILES ${UI_INC})
source_group("Generated Files\\moc" FILES ${MOC})
source_group("Generated Files\\ressource" FILES ${RSRC_SRC})
source_group("Generated Files\\translation" FILES ${QM})

add_executable(medusa_qt ${GUI_TYPE}
  ${INC}
  ${SRC}
  ${MOC}
  ${UI_INC}
  ${RSRC_SRC}
  ${QM}
)

target_link_libraries(medusa_qt ${QT_LIBRARIES} Medusa ColorPicker)
target_link_libraries(medusa_qt ${QT_LIBRARIES} Medusa FontPicker)
install(TARGETS medusa_qt RUNTIME DESTINATION .)

if (WIN32)
  foreach (qt_dep QtCore QtGui)
    install(FILES "${QT_LIBRARY_DIR}/${qt_dep}${QT_VERSION_MAJOR}.dll" DESTINATION .)
  endforeach ()
endif ()
