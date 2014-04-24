if (NOT DEFINED OGDF_PATH)
  message(FATAL_ERROR "You must defined OGDF_PATH")
endif()

set(OGDF_INCLUDE_DIRS ${OGDF_PATH}/include)

find_library(COIN_LIBRARY_RELEASE
  NAMES coin COIN
  PATHS ${OGDF_PATH}
  PATH_SUFFIXES build build/Release Release Win32/Release x64/Release _release
  )

find_library(OGDF_LIBRARY_RELEASE
  NAMES ogdf OGDF
  PATHS ${OGDF_PATH}
  PATH_SUFFIXES build build/Release Release Win32/Release x64/Release _release
  )

find_library(COIN_LIBRARY_DEBUG
  NAMES coin COIN
  PATHS ${OGDF_PATH}
  PATH_SUFFIXES build build/Debug Debug Win32/Debug x64/Debug _debug
  )

find_library(OGDF_LIBRARY_DEBUG
  NAMES ogdf OGDF
  PATHS ${OGDF_PATH}
  PATH_SUFFIXES build build/Debug Debug Win32/Debug x64/Debug _debug
  )

set(OGDF_LIBRARIES optimized ${OGDF_LIBRARY_RELEASE} optimized ${COIN_LIBRARY_RELEASE} debug ${OGDF_LIBRARY_DEBUG} debug ${COIN_LIBRARY_DEBUG})
