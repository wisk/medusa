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
target_link_libraries(ldr_elf Medusa)

if(WIN32)
  install(TARGETS ldr_elf RUNTIME DESTINATION .)
else()
  install(TARGETS ldr_elf LIBRARY DESTINATION .)
endif()
