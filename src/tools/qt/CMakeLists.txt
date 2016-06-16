set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QT5_CMAKE_PATH})

# We require qt5 widgets
find_package(Qt5Widgets)

find_package(Threads REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(WIN32)
  set(GUI_TYPE WIN32)
endif(WIN32)

set(INCROOT  ${CMAKE_SOURCE_DIR}/src/tools/qt)
set(SRCROOT  ${CMAKE_SOURCE_DIR}/src/tools/qt)
set(UIROOT   ${CMAKE_SOURCE_DIR}/src/tools/qt)
set(RSRCROOT ${CMAKE_SOURCE_DIR}/src/tools/qt)

set (INC
  ${INCROOT}/About.hpp
  ${INCROOT}/Action.hpp
  ${INCROOT}/BasicBlockItem.hpp
  ${INCROOT}/BindingView.hpp
  ${INCROOT}/CommentDialog.hpp
  ${INCROOT}/ConfigureDialog.hpp
  ${INCROOT}/ControlFlowGraphScene.hpp
  ${INCROOT}/ControlFlowGraphView.hpp
  ${INCROOT}/DisassemblyView.hpp
  ${INCROOT}/EdgeItem.hpp
  ${INCROOT}/Goto.hpp
  ${INCROOT}/GraphScene.hpp
  ${SRCROOT}/GraphView.hpp
  ${INCROOT}/LabelDialog.hpp
  ${INCROOT}/LabelView.hpp
  ${INCROOT}/MainWindow.hpp
  ${INCROOT}/MemoryAreaView.hpp
  ${INCROOT}/Proxy.hpp
  ${INCROOT}/ScrollbarAddress.hpp
  ${INCROOT}/SemanticView.hpp
  ${INCROOT}/SettingsDialog.hpp
  ${INCROOT}/VertexItem.hpp
)

set (SRC
  ${SRCROOT}/About.cpp
  ${SRCROOT}/Action.cpp
  ${SRCROOT}/BasicBlockItem.cpp
  ${SRCROOT}/BindingView.cpp
  ${SRCROOT}/CommentDialog.cpp
  ${SRCROOT}/ConfigureDialog.cpp
  ${SRCROOT}/ControlFlowGraphScene.cpp
  ${SRCROOT}/ControlFlowGraphView.cpp
  ${SRCROOT}/DisassemblyView.cpp
  ${SRCROOT}/EdgeItem.cpp
  ${SRCROOT}/Goto.cpp
  ${SRCROOT}/GraphScene.cpp
  ${SRCROOT}/GraphView.cpp
  ${SRCROOT}/LabelDialog.cpp
  ${SRCROOT}/LabelView.cpp
  ${SRCROOT}/MainWindow.cpp
  ${SRCROOT}/MemoryAreaView.cpp
  ${SRCROOT}/Proxy.cpp
  ${SRCROOT}/ScrollbarAddress.cpp
  ${SRCROOT}/SemanticView.cpp
  ${SRCROOT}/SettingsDialog.cpp
  ${SRCROOT}/VertexItem.cpp
  ${SRCROOT}/main.cpp
)

# Qt user interface files
set(UI
  ${UIROOT}/About.ui
  ${UIROOT}/BindingView.ui
  ${UIROOT}/Comment.ui
  ${UIROOT}/ConfigureDialog.ui
  ${UIROOT}/Goto.ui
  ${UIROOT}/Label.ui
  ${UIROOT}/MainWindow.ui
  ${UIROOT}/SettingsDialog.ui
)

# Qt resource files
set(RSRC ${RSRCROOT}/Resources.qrc)

qt5_wrap_ui(UI_INC ${UI})
qt5_add_resources(RSRC_SRC ${RSRC})

source_group("Forms"               FILES ${UI})
source_group("Resources"           FILES ${RSRC})
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

# TODO(wisk): ogdf must be only used by medusa itself, not by tools
if (MEDUSA_BUILD_WITH_OGDF)
  add_dependencies(qMedusa ogdf)
  include_directories("${OGDF_INCLUDE_DIRS}")
  target_link_libraries(qMedusa medusa ${CMAKE_THREAD_LIBS_INIT} ${OGDF_LIBRARIES})
else()
  target_link_libraries(qMedusa medusa ${CMAKE_THREAD_LIBS_INIT})
endif()
set_target_properties(qMedusa PROPERTIES FOLDER "Tools")

# I can't believe how hard it is to deploy qt5...
# Inspired from http://pastebin.com/an3dyt51
# ref: http://qt-project.org/doc/qt-5/windows-deployment.html#creating-the-application-package
# ref: http://qt-project.org/wiki/Deploy_an_Application_on_Windows
if (WIN32)

  set(QT5_BIN_PATH ${QT5_CMAKE_PATH}/../../bin)
  file(TO_CMAKE_PATH ${QT5_BIN_PATH} QT5_BIN_PATH)
  set(QT5_PPF_PATH ${QT5_CMAKE_PATH}/../../plugins/platforms)
  file(TO_CMAKE_PATH ${QT5_PPF_PATH} QT5_PPF_PATH)

  list(APPEND QT5_NEEDED_DLL ${QT5_BIN_PATH}/Qt5Gui.dll)
  list(APPEND QT5_NEEDED_DLL ${QT5_BIN_PATH}/Qt5Core.dll)
  list(APPEND QT5_NEEDED_DLL ${QT5_BIN_PATH}/Qt5Widgets.dll)
  if (EXISTS "${QT5_BIN_PATH}/libGLESv2.dll")
    list(APPEND QT5_NEEDED_DLL ${QT5_BIN_PATH}/libGLESv2.dll)
  endif()
  if (EXISTS "${QT5_BIN_PATH}/libEGL.dll")
    list(APPEND QT5_NEEDED_DLL ${QT5_BIN_PATH}/libEGL.dll)
  endif()

  file(GLOB icuin_dll ${QT5_BIN_PATH}/icuin*.dll)
  list(APPEND QT5_NEEDED_DLL ${icuin_dll})
  file(GLOB icuuc_dll ${QT5_BIN_PATH}/icuuc*.dll)
  list(APPEND QT5_NEEDED_DLL ${icuuc_dll})
  file(GLOB icudt_dll ${QT5_BIN_PATH}/icudt*.dll)
  list(APPEND QT5_NEEDED_DLL ${icudt_dll})
  file(GLOB d3dcompiler_dll ${QT5_BIN_PATH}/d3dcompiler_*.dll)
  list(APPEND QT5_NEEDED_DLL ${d3dcompiler_dll})

  foreach (lib ${QT5_NEEDED_DLL})
    install(FILES ${lib} DESTINATION .)
  endforeach ()

  install(FILES ${QT5_PPF_PATH}/qwindows.dll DESTINATION platforms)

endif ()

install(TARGETS qMedusa RUNTIME DESTINATION bin)
