# add_subdirectory(text)

add_subdirectory(pydusa)

find_package(Qt5 COMPONENTS Core Widgets)
if (Qt5_FOUND)
  message(STATUS "Qt found, qMedusa will be compiled")
  add_subdirectory(qt)
endif()
