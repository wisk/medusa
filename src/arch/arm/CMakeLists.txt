include(${CMAKE_SOURCE_DIR}/cmake/medusa.cmake)
set(INCROOT ${CMAKE_SOURCE_DIR}/src/arch/arm)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/arch/arm)

# all source files
set(HDR
  ${INCROOT}/arm_architecture.hpp
  ${INCROOT}/arm_def.hpp
  ${INCROOT}/arm_calling_convention.hpp
  ${INCROOT}/arm_helper.hpp
)
set(SRC
  ${SRCROOT}/main.cpp

  ${SRCROOT}/arm_architecture.cpp
  ${SRCROOT}/arm_opcode.cpp
  ${SRCROOT}/arm_calling_convention.cpp
  ${SRCROOT}/arm_helper.cpp
)

medusa_add_module(arch arm "${HDR}" "${SRC}")