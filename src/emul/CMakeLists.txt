if (LLVM_BUILD_ROOT OR (LLVM_BUILD_ROOT_DEBUG AND LLVM_BUILD_ROOT_RELEASE))
  message(STATUS "LLVM found, compile emulation with LLVM")
  #add_subdirectory(llvm)
endif()

add_subdirectory(interpreter)
