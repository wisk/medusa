if (NOT DEFINED OGDF_ROOT)
  message(FATAL_ERROR "You must defined OGDF_ROOT")
endif()

set(OGDF_INCLUDE_DIRS ${OGDF_ROOT}/include)

find_library(COIN_LIBRARY_RELEASE
  NAMES coin COIN
  PATHS ${OGDF_ROOT}
  PATH_SUFFIXES lib build build/Release Release Win32/Release x64/Release _release
  )

find_library(OGDF_LIBRARY_RELEASE
  NAMES ogdf OGDF
  PATHS ${OGDF_ROOT}
  PATH_SUFFIXES lib build build/Release Release Win32/Release x64/Release _release
  )

find_library(COIN_LIBRARY_DEBUG
  NAMES coind COINd
  PATHS ${OGDF_ROOT}
  PATH_SUFFIXES lib build build/Debug Debug Win32/Debug x64/Debug _debug
  )

find_library(OGDF_LIBRARY_DEBUG
  NAMES ogdfd OGDFd
  PATHS ${OGDF_ROOT}
  PATH_SUFFIXES lib build build/Debug Debug Win32/Debug x64/Debug _debug
  )

# NOTE(wisk): it seems it doesn't work anymore
set(OGDF_LIBRARIES optimized ${OGDF_LIBRARY_RELEASE} optimized ${COIN_LIBRARY_RELEASE} debug ${OGDF_LIBRARY_DEBUG} debug ${COIN_LIBRARY_DEBUG})
set(OGDF_FOUND TRUE)

add_library(COIN STATIC IMPORTED)
add_library(OGDF STATIC IMPORTED)
set_target_properties(COIN PROPERTIES IMPORTED_LOCATION_DEBUG   "${COIN_LIBRARY_DEBUG}  ")
set_target_properties(OGDF PROPERTIES IMPORTED_LOCATION_DEBUG   "${OGDF_LIBRARY_DEBUG}  ")
set_target_properties(COIN PROPERTIES IMPORTED_LOCATION_RELEASE "${COIN_LIBRARY_RELEASE}")
set_target_properties(OGDF PROPERTIES IMPORTED_LOCATION_RELEASE "${OGDF_LIBRARY_RELEASE}")