function(medusa_add_core include_files source_files)
endfunction()

macro(medusa_add_module type name include_files source_files)

  set(MEDUSA_FOLDER "")
  if ("${type}" STREQUAL "arch")
    set(MEDUSA_FOLDER "Architectures")
  elseif ("${type}" STREQUAL "bind")
    set(MEDUSA_FOLDER "Bindings")
  elseif ("${type}" STREQUAL "compil")
    set(MEDUSA_FOLDER "Compilations")
  elseif ("${type}" STREQUAL "db")
    set(MEDUSA_FOLDER "Databases")
  elseif ("${type}" STREQUAL "emul")
    set(MEDUSA_FOLDER "Emulators")
  elseif ("${type}" STREQUAL "ldr")
    set(MEDUSA_FOLDER "Loaders")
  elseif ("${type}" STREQUAL "os")
    set(MEDUSA_FOLDER "OperatingSystems")
  else()
    message(FATAL_ERROR "unknown module type: ${type}")
  endif()

  set(MEDUSA_MODULE "${type}_${name}")
  message(STATUS "Build ${MEDUSA_MODULE} (${MEDUSA_FOLDER})")

  add_library(${MEDUSA_MODULE} SHARED ${source_files} ${include_files})
# target_include_directories(${MEDUSA_MODULE} PRIVATE ${include_files})
  target_link_libraries(${MEDUSA_MODULE} medusa)
  set_target_properties(${MEDUSA_MODULE} PROPERTIES
    LINKER_LANGUAGE CXX
    FOLDER ${MEDUSA_FOLDER}
    PREFIX ""
    DEBUG_POSTFIX ""
  )

  install(TARGETS ${MEDUSA_MODULE}
    RUNTIME DESTINATION ${MEDUSA_MODULE_PATH}
    LIBRARY DESTINATION ${MEDUSA_MODULE_PATH}
  )

  if (MEDUSA_CMAKE_USE_COTIRE)
    cotire(${MEDUSA_MODULE})
  endif()

endmacro()

function(medusa_add_tool target include_files source_files)
endfunction()

function(medusa_add_test target include_files source_files)
endfunction()
