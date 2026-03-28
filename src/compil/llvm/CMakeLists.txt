set(INCROOT ${CMAKE_SOURCE_DIR}/src/compil/llvm)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/compil/llvm)

set(SRC
  ${SRCROOT}/main.cpp
  ${SRCROOT}/llvm_compiler.cpp
  ${INCROOT}/llvm_compiler.hpp
  )

find_package(LLVM CONFIG)

if (LLVM_FOUND AND LLVM_PACKAGE_VERSION VERSION_LESS "16.0")
  message(STATUS "LLVM found: ${LLVM_PACKAGE_VERSION}")

  add_library(compil_llvm SHARED ${SRC})

  llvm_map_components_to_libnames(LLVM_LIBRARIES all)

  target_include_directories(compil_llvm PRIVATE ${LLVM_INCLUDE_DIRS})
  target_compile_definitions(compil_llvm PRIVATE ${LLVM_DEFINITIONS})
  target_link_libraries(compil_llvm medusa ${LLVM_LIBRARIES})

  set_target_properties(compil_llvm PROPERTIES
    FOLDER "Compilations"
    PREFIX ""
    DEBUG_POSTFIX ""
    )

  install(TARGETS compil_llvm
    RUNTIME DESTINATION ${MEDUSA_MODULE_PATH}
    LIBRARY DESTINATION ${MEDUSA_MODULE_PATH}
  )
else()
  if (LLVM_FOUND)
    message(STATUS "LLVM ${LLVM_PACKAGE_VERSION} found but too new (need <16), compil_llvm will not be built")
  else()
    message(STATUS "LLVM not found, compil_llvm will not be built")
  endif()
endif()
