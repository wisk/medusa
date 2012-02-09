include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(INCROOT  ${CMAKE_SOURCE_DIR}/src/ui/wxwidgets)
set(SRCROOT  ${CMAKE_SOURCE_DIR}/src/ui/wxwidgets)

set(WX_USE_REL_AND_DBG FALSE)
find_package(wxWidgets COMPONENTS base core stc scintilla aui REQUIRED)
include("${wxWidgets_USE_FILE}")

# wxwidgets ui source files
set(SRC
  ${SRCROOT}/disassembly_view_control.cpp
  ${SRCROOT}/medusa_frame.cpp
  ${SRCROOT}/wxmedusa.cpp
)

# wxwidgets ui header files
set(INC
  ${INCROOT}/disassembly_view_control.hpp
  ${INCROOT}/medusa_frame.hpp
  ${INCROOT}/wxmedusa.hpp
)

# Workaround #1, for unknown reason "debug" and "optimized" are included in
# libraries list, we also have to add ".lib" to MS libraries.
list(REMOVE_ITEM wxWidgets_LIBRARIES "debug" "optimized" "winmm" "comctl32" "rpcrt4" "wsock32")
list(APPEND wxWidgets_LIBRARIES "winmm.lib" "comctl32.lib" "rpcrt4.lib" "wsock32.lib")

# Workaround #2, we must include resource file on visual studio.
if (MSVC)
  set(RCFILE ${WX_ROOT_DIR}\\include\\wx\\msw\\wx.rc)
  STRING(REGEX REPLACE "\\\\" "/" RCFILE "${RCFILE}")
  message("DBG: ${RCFILE}")
  set(SRC ${SRC} ${RCFILE})
endif()

add_executable(wxMedusa
  ${INC}
  ${SRC}
)
target_link_libraries(wxMedusa "${wxWidgets_LIBRARIES}")
install(TARGETS wxMedusa RUNTIME DESTINATION .)
