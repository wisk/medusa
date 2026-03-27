include(${CMAKE_SOURCE_DIR}/cmake/medusa.cmake)
set(INCROOT ${CMAKE_SOURCE_DIR}/src/arch/lr35902)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/arch/lr35902)

# all source files
set(HDR
  ${INCROOT}/lr35902_architecture.hpp
)
set(SRC
  ${SRCROOT}/main.cpp
  ${SRCROOT}/lr35902_architecture.cpp
  ${SRCROOT}/lr35902_opcode.cpp
)

medusa_add_module(arch lr35902 "${HDR}" "${SRC}")
