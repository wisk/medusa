set(INCROOT ${CMAKE_SOURCE_DIR}/inc/medusa)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/core)

# all source files
set(SRC
  ${INCROOT}/address.hpp
  ${INCROOT}/analyzer.hpp
  ${INCROOT}/architecture.hpp
  ${INCROOT}/array.hpp
  ${INCROOT}/basic_block.hpp
  ${INCROOT}/binary_stream.hpp
  ${INCROOT}/bits.hpp
  ${INCROOT}/cell.hpp
  ${INCROOT}/cell_action.hpp
  ${INCROOT}/cell_data.hpp
  ${INCROOT}/character.hpp
  ${INCROOT}/configuration.hpp
  ${INCROOT}/control_flow_graph.hpp
  ${INCROOT}/context.hpp
  ${INCROOT}/database.hpp
  ${INCROOT}/disassembly_view.hpp
  ${INCROOT}/document.hpp
  ${INCROOT}/emulation.hpp
  ${INCROOT}/endian.hpp
  ${INCROOT}/event_handler.hpp
  ${INCROOT}/event_queue.hpp
  ${INCROOT}/exception.hpp
  ${INCROOT}/execution.hpp
  ${INCROOT}/export.hpp
  ${INCROOT}/expression.hpp
  ${INCROOT}/extend.hpp
  ${INCROOT}/function.hpp
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
  ${INCROOT}/operand.hpp
  ${INCROOT}/os.hpp
  ${INCROOT}/plugin.hpp
  ${INCROOT}/printer.hpp
  ${INCROOT}/string.hpp
  ${INCROOT}/struct.hpp
  ${INCROOT}/types.hpp
  ${INCROOT}/value.hpp
  ${INCROOT}/view.hpp
  ${INCROOT}/xref.hpp

  ${SRCROOT}/address.cpp
  ${SRCROOT}/analyzer.cpp
  ${SRCROOT}/architecture.cpp
  ${SRCROOT}/array.cpp
  ${SRCROOT}/basic_block.cpp
  ${SRCROOT}/cell.cpp
  ${SRCROOT}/cell_action.cpp
  ${SRCROOT}/cell_data.cpp
  ${SRCROOT}/character.cpp
  ${SRCROOT}/configuration.cpp
  ${SRCROOT}/control_flow_graph.cpp
  ${SRCROOT}/context.cpp
  ${SRCROOT}/database.cpp
  ${SRCROOT}/disassembly_view.cpp
  ${SRCROOT}/document.cpp
  ${SRCROOT}/emulation.cpp
  ${SRCROOT}/endian.cpp
  ${SRCROOT}/event_queue.cpp
  ${SRCROOT}/exception.cpp
  ${SRCROOT}/execution.cpp
  ${SRCROOT}/expression.cpp
  ${SRCROOT}/function.cpp
  ${SRCROOT}/instruction.cpp
  ${SRCROOT}/information.cpp
  ${SRCROOT}/label.cpp
  ${SRCROOT}/log.cpp
  ${SRCROOT}/main.cpp
  ${SRCROOT}/medusa.cpp
  ${SRCROOT}/memory_area.cpp
  ${SRCROOT}/multicell.cpp
  ${SRCROOT}/operand.cpp
  ${SRCROOT}/os.cpp
  ${SRCROOT}/printer.cpp
  ${SRCROOT}/string.cpp
  ${SRCROOT}/struct.cpp
  ${SRCROOT}/value.cpp
  ${SRCROOT}/view.cpp
  ${SRCROOT}/xref.cpp
  )

if(WIN32)
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

# define libMedusa architecture target
add_library(Medusa SHARED ${SRC})
target_link_libraries(Medusa ${Boost_LIBRARIES})

# Thank ogre, I would never find this myself
find_library(DL_LIBRARY NAMES dl dlsym)
mark_as_advanced(DL_LIBRARY)
if (DL_LIBRARY)
  target_link_libraries(Medusa dl)
endif ()

if(WIN32)
  install(TARGETS Medusa RUNTIME DESTINATION .)
  file(GLOB boost_filesystem_dll "${Boost_LIBRARY_DIRS}/boost_filesystem-*-mt-1*.dll")
  install(FILES "${boost_filesystem_dll}" DESTINATION .)
else()
  install(TARGETS Medusa LIBRARY DESTINATION .)
endif()
