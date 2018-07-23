find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(bind_python SHARED bind_python.cpp bind_python.hpp)
target_link_libraries(bind_python PRIVATE medusa)
set_target_properties(bind_python PROPERTIES
  FOLDER "Bindings"
  PREFIX "")

if(WIN32)
  install(TARGETS bind_python RUNTIME DESTINATION ${MEDUSA_MODULE_PATH})
else()
  install(TARGETS bind_python LIBRARY DESTINATION ${MEDUSA_MODULE_PATH})
endif()
