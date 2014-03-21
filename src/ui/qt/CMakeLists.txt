set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QT5_CMAKE_PATH})

# We require qt5 widgets
find_package(Qt5Widgets)

find_package(Threads REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${OGDF_INCLUDE_DIRS})

if(WIN32)
  set(GUI_TYPE WIN32)
endif(WIN32)

set(INCROOT  ${CMAKE_SOURCE_DIR}/src/ui/qt)
set(SRCROOT  ${CMAKE_SOURCE_DIR}/src/ui/qt)
set(UIROOT   ${CMAKE_SOURCE_DIR}/src/ui/qt)
set(RSRCROOT ${CMAKE_SOURCE_DIR}/src/ui/qt)

set (SRC
  ${SRCROOT}/About.cpp
  ${SRCROOT}/BasicBlockItem.cpp
  ${SRCROOT}/CommentDialog.cpp
  ${SRCROOT}/ConfigureDialog.cpp
  ${SRCROOT}/ControlFlowGraphScene.cpp
  ${SRCROOT}/ControlFlowGraphView.cpp
  ${SRCROOT}/DisassemblyPrinter.cpp
  ${SRCROOT}/DisassemblyView.cpp
  ${SRCROOT}/EdgeItem.cpp
  ${SRCROOT}/Goto.cpp
  ${SRCROOT}/LabelDialog.cpp
  ${SRCROOT}/LabelView.cpp
  ${SRCROOT}/MainWindow.cpp
  ${SRCROOT}/MemoryAreaView.cpp
  ${SRCROOT}/OpenConfirmation.cpp
  ${SRCROOT}/Proxy.cpp
  ${SRCROOT}/ScrollbarAddress.cpp
  ${SRCROOT}/SemanticView.cpp
  ${SRCROOT}/Settings.cpp
  ${SRCROOT}/SettingsDialog.cpp
  ${SRCROOT}/main.cpp
)

set (INC
  ${INCROOT}/About.hpp
  ${INCROOT}/BasicBlockItem.hpp
  ${INCROOT}/CommentDialog.hpp
  ${INCROOT}/ConfigureDialog.hpp
  ${INCROOT}/ControlFlowGraphScene.hpp
  ${INCROOT}/ControlFlowGraphView.hpp
  ${INCROOT}/DisassemblyPrinter.hpp
  ${INCROOT}/DisassemblyView.hpp
  ${INCROOT}/EdgeItem.hpp
  ${INCROOT}/Goto.hpp
  ${INCROOT}/LabelDialog.hpp
  ${INCROOT}/LabelView.hpp
  ${INCROOT}/MainWindow.hpp
  ${INCROOT}/MemoryAreaView.hpp
  ${INCROOT}/OpenConfirmation.hpp
  ${INCROOT}/Proxy.hpp
  ${INCROOT}/ScrollbarAddress.hpp
  ${INCROOT}/SemanticView.hpp
  ${INCROOT}/Settings.hpp
  ${INCROOT}/SettingsDialog.hpp
)

# Qt user interface files
set(UI
  ${UIROOT}/About.ui
  ${UIROOT}/Comment.ui
  ${UIROOT}/ConfigureDialog.ui
  ${UIROOT}/Goto.ui
  ${UIROOT}/Label.ui
  ${UIROOT}/MainWindow.ui
  ${UIROOT}/OpenConfirmation.ui
  ${UIROOT}/SettingsDialog.ui
)

# Qt resource files
set(RSRC ${RSRCROOT}/Resources.qrc)

qt5_wrap_ui(UI_INC ${UI})
qt5_add_resources(RSRC_SRC ${RSRC})

source_group("Forms"               FILES ${UI})
source_group("Ressources"          FILES ${RSRC})
source_group("Generated Files\\ui" FILES ${UI_INC})

add_executable(qMedusa ${GUI_TYPE}
  ${INC}
  ${SRC}
  ${MOC}
  ${UI_INC}
  ${RSRC_SRC}
  ${QM}
)

qt5_use_modules(qMedusa Widgets)
target_link_libraries(qMedusa Medusa ${CMAKE_THREAD_LIBS_INIT} ${OGDF_LIBRARIES})

install(TARGETS qMedusa RUNTIME DESTINATION .)
