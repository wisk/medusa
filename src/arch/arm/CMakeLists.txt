set(INCROOT ${CMAKE_SOURCE_DIR}/src/arch/arm)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/arch/arm)

# all source files
set(SRC
  ${SRCROOT}/main.cpp

  ${INCROOT}/arm_architecture.hpp
  ${INCROOT}/arm_def.hpp
  ${INCROOT}/arm_calling_convention.hpp
  ${INCROOT}/arm_helper.hpp
  ${SRCROOT}/arm_architecture.cpp
  ${SRCROOT}/arm_opcode.cpp
  ${SRCROOT}/arm_calling_convention.cpp
  ${SRCROOT}/arm_helper.cpp
  )

# define arm architecture target
add_library(arch_arm SHARED ${SRC})
target_link_libraries(arch_arm medusa)
set_target_properties(arch_arm PROPERTIES
  FOLDER "Architectures"
  PREFIX "")

if(WIN32)
  install(TARGETS arch_arm RUNTIME DESTINATION ${MEDUSA_MODULE_PATH})
else()
  install(TARGETS arch_arm LIBRARY DESTINATION ${MEDUSA_MODULE_PATH})
endif()
