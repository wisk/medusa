if (NOT DEFINED OGDF_PATH)
  message(FATAL_ERROR "You must defined OGDF_PATH")
endif()

set(OGDF_INCLUDE_DIRS ${OGDF_PATH}/include)

find_library(COIN_LIBRARY_RELEASE
  NAMES coin
  PATHS ${OGDF_PATH} ${OGDF_PATH}/Win32/Release ${OGDF_PATH}/x64/Release ${OGDF_PATH}/_release
  )

find_library(OGDF_LIBRARY_RELEASE
  NAMES ogdf
  PATHS ${OGDF_PATH} ${OGDF_PATH}/Win32/Release ${OGDF_PATH}/x64/Release ${OGDF_PATH}/_release
  )

find_library(COIN_LIBRARY_DEBUG
  NAMES coin
  PATHS ${OGDF_PATH} ${OGDF_PATH}/Win32/Debug ${OGDF_PATH}/x64/Debug ${OGDF_PATH}/_debug
  )

find_library(OGDF_LIBRARY_DEBUG
  NAMES ogdf
  PATHS ${OGDF_PATH} ${OGDF_PATH}/Win32/Debug ${OGDF_PATH}/x64/Debug ${OGDF_PATH}/_debug
  )

set(OGDF_LIBRARIES optimized ${OGDF_LIBRARY_RELEASE} optimized ${COIN_LIBRARY_RELEASE} debug ${OGDF_LIBRARY_DEBUG} debug ${COIN_LIBRARY_DEBUG})
