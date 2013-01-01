set(INCROOT ${CMAKE_SOURCE_DIR}/src/emul/llvm)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/emul/llvm)

set(LLVM_ROOT $ENV{LLVM_BUILD_ROOT})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${LLVM_ROOT}/share/llvm/cmake")
include(LLVMConfig)

include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})
llvm_map_components_to_libraries(REQ_LLVM_LIBRARIES jit native)

set(SRC
  ${SRCROOT}/main.cpp
  ${SRCROOT}/llvm_emulator.cpp
  ${INCROOT}/llvm_emulator.hpp
  )

add_library(emul_llvm SHARED ${SRC})
target_link_libraries(emul_llvm Medusa ${REQ_LLVM_LIBRARIES})

if(WIN32)
  install(TARGETS emul_llvm RUNTIME DESTINATION .)
else()
  install(TARGETS emul_llvm LIBRARY DESTINATION .)
endif()
