set(SRC
  pydusa.cpp
  py_address.cpp
  py_label.cpp
  py_binary_stream.cpp
  py_expression.cpp
  py_cell.cpp
  py_compiler.cpp
  py_configuration.cpp
  py_memory_area.cpp
  py_database.cpp
  py_exception.cpp
  py_architecture.cpp
  py_loader.cpp
  py_logger.cpp
  py_instruction.cpp
  py_document.cpp
  py_medusa.cpp
  py_context.cpp
  py_execution.cpp
  py_symbolic.cpp
  py_cpu_information.cpp
  py_module.cpp
)

find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_package(nanobind CONFIG REQUIRED)

nanobind_add_module(pydusa ${SRC})
target_link_libraries(pydusa PRIVATE medusa)
set_target_properties(
  pydusa
  PROPERTIES
    FOLDER "Tools"
    DEBUG_POSTFIX ""
)

set_target_properties(pydusa PROPERTIES PREFIX "")

install(TARGETS pydusa LIBRARY DESTINATION ${MEDUSA_MODULE_PATH})
