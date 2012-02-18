include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(INCROOT  ${CMAKE_SOURCE_DIR}/src/ui/wxwidgets)
set(SRCROOT  ${CMAKE_SOURCE_DIR}/src/ui/wxwidgets)

set(WX_USE_REL_AND_DBG FALSE)

if (MSVC)
  find_package(wxWidgets COMPONENTS scintilla xrc stc richtext aui adv core base REQUIRED)
else()
  find_package(wxWidgets COMPONENTS xrc stc richtext aui adv core base REQUIRED)
endif()
include("${wxWidgets_USE_FILE}")

# wxwidgets ui source files
set(SRC
  ${SRCROOT}/configuration_dialog.cpp
  ${SRCROOT}/disassembly_view_control.cpp
  ${SRCROOT}/medusa_frame.cpp
  ${SRCROOT}/wxmedusa.cpp
)

# wxwidgets ui header files
set(INC
  ${INCROOT}/configuration_dialog.hpp
  ${INCROOT}/disassembly_view_control.hpp
  ${INCROOT}/medusa_frame.hpp
  ${INCROOT}/wxmedusa.hpp
)

if (MSVC)
  # Workaround, for unknown reason "debug" and "optimized" are included in
  # libraries list, we also have to add ".lib" to MS libraries.
  list(REMOVE_ITEM wxWidgets_LIBRARIES "debug" "optimized" "winmm" "comctl32" "rpcrt4" "wsock32")
  list(APPEND wxWidgets_LIBRARIES "winmm.lib" "comctl32.lib" "rpcrt4.lib" "wsock32.lib")

  # Workaround, we must include resource file on visual studio.
  set(RCFILE ${WX_ROOT_DIR}\\include\\wx\\msw\\wx.rc)
  STRING(REGEX REPLACE "\\\\" "/" RCFILE "${RCFILE}")
  set(SRC ${SRC} ${RCFILE})
else()
  # Workaround for clang which can't bear ';'
  STRING(REGEX REPLACE ";" " " wxWidgets_LIBRARIES "${wxWidgets_LIBRARIES}")
endif()

message("DBG: ${wxWidgets_LIBRARIES}")

add_executable(wxMedusa
  ${INC}
  ${SRC}
)
target_link_libraries(wxMedusa Medusa "${wxWidgets_LIBRARIES}")
install(TARGETS wxMedusa RUNTIME DESTINATION .)
