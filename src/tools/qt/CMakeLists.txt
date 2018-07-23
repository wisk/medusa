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

target_link_libraries(qMedusa medusa Qt5::Widgets)
set_target_properties(qMedusa PROPERTIES FOLDER "Tools")

# src: https://stackoverflow.com/questions/41193584/deploy-all-qt-dependencies-when-building
if (Qt5_FOUND AND WIN32 AND TARGET Qt5::qmake AND NOT TARGET Qt5::windeployqt)
    get_target_property(_qt5_qmake_location Qt5::qmake IMPORTED_LOCATION)

    execute_process(
        COMMAND "${_qt5_qmake_location}" -query QT_INSTALL_PREFIX
        RESULT_VARIABLE return_code
        OUTPUT_VARIABLE qt5_install_prefix
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    set(imported_location "${qt5_install_prefix}/bin/windeployqt.exe")

    if (EXISTS ${imported_location})
        add_executable(Qt5::windeployqt IMPORTED)

        set_target_properties(Qt5::windeployqt PROPERTIES
            IMPORTED_LOCATION ${imported_location}
        )
    endif()
endif()

if (TARGET Qt5::windeployqt)
    # execute windeployqt in a tmp directory after build
    add_custom_command(TARGET qMedusa
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_CURRENT_BINARY_DIR}/windeployqt"
        COMMAND set PATH=%PATH%$<SEMICOLON>${qt5_install_prefix}/bin
        COMMAND Qt5::windeployqt --dir "${CMAKE_CURRENT_BINARY_DIR}/windeployqt" "$<TARGET_FILE_DIR:qMedusa>/$<TARGET_FILE_NAME:qMedusa>"
    )

    # copy deployment directory during installation
    install(
        DIRECTORY
        "${CMAKE_CURRENT_BINARY_DIR}/windeployqt/"
        DESTINATION ${MEDUSA_EXECUTABLE_PATH}
    )
endif()

install(TARGETS qMedusa RUNTIME DESTINATION ${MEDUSA_EXECUTABLE_PATH})
