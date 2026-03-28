set(INCROOT ${CMAKE_SOURCE_DIR}/src/emul/llvm)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/emul/llvm)

set(SRC
  ${SRCROOT}/main.cpp
  ${SRCROOT}/llvm_emulator.cpp
  ${INCROOT}/llvm_emulator.hpp
  )

find_package(LLVM CONFIG)

if (LLVM_FOUND)
  message(STATUS "LLVM found: ${LLVM_PACKAGE_VERSION}")

  add_library(emul_llvm SHARED ${SRC})

  llvm_map_components_to_libnames(LLVM_LIBRARIES all)

  target_include_directories(emul_llvm PRIVATE ${LLVM_INCLUDE_DIRS})
  target_compile_definitions(emul_llvm PRIVATE ${LLVM_DEFINITIONS})
  target_link_libraries(emul_llvm medusa ${LLVM_LIBRARIES})

  set_target_properties(emul_llvm PROPERTIES
    FOLDER "Emulators"
    PREFIX ""
    DEBUG_POSTFIX ""
    )

  install(TARGETS emul_llvm
    RUNTIME DESTINATION ${MEDUSA_MODULE_PATH}
    LIBRARY DESTINATION ${MEDUSA_MODULE_PATH}
  )
else()
  message(STATUS "LLVM not found, emul_llvm will not be built")
endif()
