# We require qt4 library
find_package(Qt4 COMPONENTS QtCore QtGui QtDesigner REQUIRED)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(INCROOT  ${CMAKE_SOURCE_DIR}/src/ui/fontpicker)
set(SRCROOT  ${CMAKE_SOURCE_DIR}/src/ui/fontpicker)
set(UIROOT   ${CMAKE_SOURCE_DIR}/src/ui/fontpicker)
set(RSRCROOT ${CMAKE_SOURCE_DIR}/src/ui/fontpicker)

# Source files
set(SRC
  ${SRCROOT}/FontPicker.cpp
  ${SRCROOT}/FontPickerPlugin.cpp
)

# Header files
set(INC
  ${INCROOT}/FontPicker.h
  ${INCROOT}/FontPickerPlugin.h
)

# Qt header files (Q_OBJECT files)
set(MOCINC
  ${INCROOT}/FontPicker.h
  ${INCROOT}/FontPickerPlugin.h
)

# QT user interface files
set(UI
)

# Qt resource files
set(RSRC

)

qt4_wrap_cpp(MOC ${MOCINC})
qt4_wrap_ui(UI_INC ${UI})
qt4_add_resources(RSRC_SRC ${RSRC})

source_group("Forms" FILES ${UI})
source_group("Ressources" FILES ${RSRC})

source_group("Generated Files\\ui" FILES ${UI_INC})
source_group("Generated Files\\moc" FILES ${MOC})
source_group("Generated Files\\ressource" FILES ${RSRC_SRC})

# define libColorPicker architecture target
add_library(FontPicker SHARED
  ${INC}
  ${SRC}
  ${MOC}
  ${UI_INC}
  ${RSRC_SRC}
)

set_target_properties(FontPicker PROPERTIES DEFINE_SYMBOL QDESIGNER_EXPORT_WIDGETS)
target_link_libraries(FontPicker ${QT_LIBRARIES})

if(WIN32)
  install(TARGETS FontPicker RUNTIME DESTINATION .)
else()
  install(TARGETS FontPicker LIBRARY DESTINATION .)
endif()

#file(TO_NATIVE_PATH "${QT_PLUGINS_DIR}/designer" PLUGINDIR)
#file(TO_NATIVE_PATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/FontPicker.dll" PLUGINFILE)

#if (EXISTS ${PLUGINDIR})
#	message("INFO: The FontPicker Designer Plugin will be installed in ${PLUGINDIR} after build")
#
#	if (WIN32)
#		add_custom_command(TARGET FontPicker POST_BUILD COMMAND copy /Y ${PLUGINFILE} ${PLUGINDIR})
#	elseif (UNIX)
#		add_custom_command(TARGET FontPicker POST_BUILD COMMAND cp -f ${PLUGINFILE} ${PLUGINDIR})
#	endif()
#endif()
