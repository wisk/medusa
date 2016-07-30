set(INCROOT ${CMAKE_SOURCE_DIR}/inc/medusa)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/core)

# all source files
set(SRC
  ${CMAKE_BINARY_DIR}/inc/medusa/version.hpp

  ${INCROOT}/address.hpp
  ${INCROOT}/analyzer.hpp
  ${INCROOT}/architecture.hpp
  ${INCROOT}/array.hpp
  ${INCROOT}/basic_block.hpp
  ${INCROOT}/binary_stream.hpp
  ${INCROOT}/binding.hpp
  ${INCROOT}/bits.hpp
  ${INCROOT}/cell.hpp
  ${INCROOT}/cell_action.hpp
  ${INCROOT}/cell_data.hpp
  ${INCROOT}/cell_text.hpp
  ${INCROOT}/calling_convention.hpp
  ${INCROOT}/character.hpp
  ${INCROOT}/configuration.hpp
  ${INCROOT}/control_flow_graph.hpp
  ${INCROOT}/context.hpp
  ${INCROOT}/database.hpp
  ${INCROOT}/detail.hpp
  ${INCROOT}/disassembly_view.hpp
  ${INCROOT}/document.hpp
  ${INCROOT}/emulation.hpp
  ${INCROOT}/endian.hpp
  ${INCROOT}/event_handler.hpp
  ${INCROOT}/event_queue.hpp
  ${INCROOT}/exception.hpp
  ${INCROOT}/execution.hpp
  ${INCROOT}/expression.hpp
  ${INCROOT}/expression_filter.hpp
  ${INCROOT}/expression_operation.hpp
  ${INCROOT}/expression_simplifier.hpp
  ${INCROOT}/expression_visitor.hpp
  ${INCROOT}/extend.hpp
  ${INCROOT}/function.hpp
  ${INCROOT}/graph.hpp
  ${INCROOT}/information.hpp
  ${INCROOT}/instruction.hpp
  ${INCROOT}/label.hpp
  ${INCROOT}/loader.hpp
  ${INCROOT}/log.hpp
  ${INCROOT}/medusa.hpp
  ${INCROOT}/memory_area.hpp
  ${INCROOT}/module.hpp
  ${INCROOT}/multicell.hpp
  ${INCROOT}/namespace.hpp
  ${INCROOT}/os.hpp
  ${INCROOT}/plugin.hpp
  ${INCROOT}/string.hpp
  ${INCROOT}/structure.hpp
  ${INCROOT}/symbolic.hpp
  ${INCROOT}/task.hpp
  ${INCROOT}/types.hpp
  ${INCROOT}/user_configuration.hpp
  ${INCROOT}/util.hpp
  ${INCROOT}/value.hpp
  ${INCROOT}/view.hpp
  ${INCROOT}/xref.hpp

  ${SRCROOT}/address.cpp
  ${SRCROOT}/analyzer.cpp
  ${SRCROOT}/analyzer_disassemble.cpp
  ${SRCROOT}/analyzer_instruction.cpp
  ${SRCROOT}/analyzer_basic_block.cpp
  ${SRCROOT}/analyzer_function.cpp
  ${SRCROOT}/analyzer_string.cpp
  ${SRCROOT}/architecture.cpp
  ${SRCROOT}/array.cpp
  ${SRCROOT}/basic_block.cpp
  ${SRCROOT}/binding.cpp
  ${SRCROOT}/calling_convention.cpp
  ${SRCROOT}/cell.cpp
  ${SRCROOT}/cell_action.cpp
  ${SRCROOT}/cell_data.cpp
  ${SRCROOT}/cell_text.cpp
  ${SRCROOT}/character.cpp
  ${SRCROOT}/configuration.cpp
  ${SRCROOT}/control_flow_graph.cpp
  ${SRCROOT}/context.cpp
  ${SRCROOT}/database.cpp
  ${SRCROOT}/detail.cpp
  ${SRCROOT}/disassembly_view.cpp
  ${SRCROOT}/document.cpp
  ${SRCROOT}/emulation.cpp
  ${SRCROOT}/endian.cpp
  ${SRCROOT}/event_queue.cpp
  ${SRCROOT}/exception.cpp
  ${SRCROOT}/execution.cpp
  ${SRCROOT}/expression.cpp
  ${SRCROOT}/expression_filter.cpp
  ${SRCROOT}/expression_operation.cpp
  ${SRCROOT}/expression_simplifier.cpp
  ${SRCROOT}/expression_visitor.cpp
  ${SRCROOT}/function.cpp
  ${SRCROOT}/graph.cpp
  ${SRCROOT}/instruction.cpp
  ${SRCROOT}/information.cpp
  ${SRCROOT}/label.cpp
  ${SRCROOT}/log.cpp
  ${SRCROOT}/medusa.cpp
  ${SRCROOT}/memory_area.cpp
  ${SRCROOT}/module.cpp
  ${SRCROOT}/multicell.cpp
  ${SRCROOT}/os.cpp
  ${SRCROOT}/string.cpp
  ${SRCROOT}/structure.cpp
  ${SRCROOT}/symbolic.cpp
  ${SRCROOT}/task.cpp
  ${SRCROOT}/types.cpp
  ${SRCROOT}/user_configuration.cpp
  ${SRCROOT}/util.cpp
  ${SRCROOT}/value.cpp
  ${SRCROOT}/view.cpp
  ${SRCROOT}/xref.cpp
  )

if (WIN32)
  set(SRC
    ${SRC}
    ${SRCROOT}/windows_binary_stream.cpp
    ${SRCROOT}/windows_module.cpp
    )
else()
  set(SRC
    ${SRC}
    ${SRCROOT}/unix_binary_stream.cpp
    ${SRCROOT}/unix_module.cpp
    )
endif()

include(GenerateExportHeader)

# define medusa architecture target
add_library(medusa SHARED ${SRC})

# generate export file
generate_export_header(medusa
  EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/inc/medusa/export.hpp
)

if (MEDUSA_BUILD_WITH_OGDF)
  add_dependencies(medusa ogdf)
endif()

if (MSVC)
  target_link_libraries(medusa ${MEDUSA_NEEDED_LIBRARIES})
endif()

if (APPLE)
  target_link_libraries(medusa ${MEDUSA_NEEDED_LIBRARIES})
endif()

# Thank ogre, I would never find this myself
find_library(DL_LIBRARY NAMES dl dlsym)
mark_as_advanced(DL_LIBRARY)
if (DL_LIBRARY)
  target_link_libraries(medusa dl)
endif ()

if (MEDUSA_CMAKE_USE_COTIRE)
  cotire(Medusa)
endif()

if (WIN32)
  install(TARGETS medusa RUNTIME DESTINATION ${MEDUSA_EXECUTABLE_PATH})
  file(GLOB boost_system_dll     "${Boost_LIBRARY_DIRS}/boost_system-*-mt-1*.dll")
  file(GLOB boost_filesystem_dll "${Boost_LIBRARY_DIRS}/boost_filesystem-*-mt-1*.dll")
  install(FILES "${boost_system_dll}" "${boost_filesystem_dll}" DESTINATION ${MEDUSA_EXECUTABLE_PATH})
else ()
  install(TARGETS medusa LIBRARY DESTINATION ${MEDUSA_LIBRARY_PATH})
endif ()
