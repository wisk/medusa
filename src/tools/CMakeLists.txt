add_subdirectory(text)

add_subdirectory(pydusa)

find_package(Qt5Widgets)
if (Qt5Widgets_FOUND)
  message(STATUS "Qt found, qMedusa will be compiled")
  add_subdirectory(qt)
endif()
