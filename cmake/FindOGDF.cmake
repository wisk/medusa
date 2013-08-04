if (NOT DEFINED OGDF_DIR)
  message(FATAL_ERROR "You must defined OGDF_DIR")
endif()

set(OGDF_INCLUDE_DIRS ${OGDF_DIR}/include)

find_library(COIN_LIBRARY_RELEASE
  NAMES coin.lib libCOIN.a
  PATHS ${OGDF_DIR}/Win32/Release ${OGDF_DIR}/x64/Release ${OGDF_DIR}/_release
  )

find_library(OGDF_LIBRARY_RELEASE
  NAMES ogdf.lib libOGDF.a
  PATHS ${OGDF_DIR}/Win32/Release ${OGDF_DIR}/x64/Release ${OGDF_DIR}/_release
  )

find_library(COIN_LIBRARY_DEBUG
  NAMES coin.lib libCOIN.a
  PATHS ${OGDF_DIR}/Win32/Debug ${OGDF_DIR}/x64/Debug ${OGDF_DIR}/_debug
  )

find_library(OGDF_LIBRARY_DEBUG
  NAMES ogdf.lib libOGDF.a
  PATHS ${OGDF_DIR}/Win32/Debug ${OGDF_DIR}/x64/Debug ${OGDF_DIR}/_debug
  )

set(OGDF_LIBRARIES optimized ${COIN_LIBRARY_RELEASE} optimized ${OGDF_LIBRARY_RELEASE} debug ${COIN_LIBRARY_DEBUG} debug ${OGDF_LIBRARY_DEBUG})
