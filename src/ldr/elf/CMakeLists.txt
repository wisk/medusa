set(INCROOT ${CMAKE_SOURCE_DIR}/src/ldr/elf)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/ldr/elf)

# all source files
set(SRC
  ${SRCROOT}/main.cpp
  ${INCROOT}/elf.h
  ${INCROOT}/elf_traits.hpp
  ${INCROOT}/elf_loader.hpp
  ${SRCROOT}/elf_loader.cpp
  )

# define elf loader target
add_library(ldr_elf SHARED ${SRC})
target_link_libraries(ldr_elf medusa)
set_target_properties(ldr_elf PROPERTIES
  FOLDER "Loaders"
  PREFIX "")

if(WIN32)
  install(TARGETS ldr_elf RUNTIME DESTINATION ${MEDUSA_MODULE_PATH})
else()
  install(TARGETS ldr_elf LIBRARY DESTINATION ${MEDUSA_MODULE_PATH})
endif()
