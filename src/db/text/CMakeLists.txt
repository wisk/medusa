include(${CMAKE_SOURCE_DIR}/cmake/medusa.cmake)
set(INCROOT ${CMAKE_SOURCE_DIR}/src/db/text)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/db/text)

# all source files
set(HDR
	${INCROOT}/text_db.hpp
)
set(SRC
  ${SRCROOT}/main.cpp
  ${SRCROOT}/text_db.cpp
)

medusa_add_module(db text "${HDR}" "${SRC}")
