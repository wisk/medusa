set(INCROOT ${CMAKE_SOURCE_DIR}/inc/medusa)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/core)

# all source files
set(SRC
  ${INCROOT}/address.hpp
  ${INCROOT}/architecture.hpp
  ${INCROOT}/array.hpp
  ${INCROOT}/binary_stream.hpp
  ${INCROOT}/cell.hpp
  ${INCROOT}/character.hpp
  ${INCROOT}/configuration.hpp
  ${INCROOT}/database.hpp
  ${INCROOT}/disassembler.hpp
  ${INCROOT}/endian.hpp
  ${INCROOT}/exception.hpp
  ${INCROOT}/export.hpp
  ${INCROOT}/function.hpp
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
  ${INCROOT}/plugin.hpp
  ${INCROOT}/serialize.hpp
  ${INCROOT}/string.hpp
  ${INCROOT}/struct.hpp
  ${INCROOT}/types.hpp
  ${INCROOT}/value.hpp
  ${INCROOT}/xref.hpp

  ${SRCROOT}/address.cpp
  ${SRCROOT}/architecture.cpp
  ${SRCROOT}/array.cpp
  ${SRCROOT}/cell.cpp
  ${SRCROOT}/character.cpp
  ${SRCROOT}/configuration.cpp
  ${SRCROOT}/database.cpp
  ${SRCROOT}/disassembler.cpp
  ${SRCROOT}/endian.cpp
  ${SRCROOT}/exception.cpp
  ${SRCROOT}/function.cpp
  ${SRCROOT}/instruction.cpp
  ${SRCROOT}/log.cpp
  ${SRCROOT}/main.cpp
  ${SRCROOT}/medusa.cpp
  ${SRCROOT}/memory_area.cpp
  ${SRCROOT}/multicell.cpp
  ${SRCROOT}/operand.cpp
  ${SRCROOT}/string.cpp
  ${SRCROOT}/struct.cpp
  ${SRCROOT}/value.cpp
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
