set(PACKAGE_NAME medusa)
set(IGNORE_PATTERNS "/\\\\.git")
set(PACKAGE_FILE_NAME PACKAGE_NAME)

set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ".")
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")

if (UNIX)
  set(CPACK_GENERATOR "TGZ;DEB")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "wisk")
  set(CPACK_SOURCE_GENERATOR "TGZ")
  set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS TRUE)
endif (UNIX)

if (WIN32)
  set(CPACK_GENERATOR "WIX")
  set(CPACK_SOURCE_GENERATOR "ZIP")
  set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}\\\\medusa.ico")
  set(CPACK_WIX_PRODUCT_ICON "${CMAKE_CURRENT_SOURCE_DIR}\\\\medusa.ico")
  set(CPACK_PACKAGE_VENDOR "wisk")
  set(CPACK_PACKAGE_CONTACT "medusa.disasm@gmail.com")
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "An open source disassembler")
  set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")
  set(CPACK_WIX_LICENSE_RTF "${CMAKE_SOURCE_DIR}/LICENSE.rtf")
  set(CPACK_PACKAGE_EXECUTABLES "qMedusa.exe;qMedusa")
  set(CPACK_WIX_UPGRADE_GUID "B511341A-C5DA-4403-A09D-D50C2DDE1F44")
endif (WIN32)

if (TARGET pydusa)
  message(STATUS "pydusa found, generate python package")
  set(PYDUSA_PACKAGE_PATH "${CMAKE_BINARY_DIR}/medusa-python-package")
  set(PYDUSA_PACKAGE_NAME "pydusa-python.tgz")
  install(
    FILES
      "${CMAKE_CURRENT_SOURCE_DIR}/python/__init__.py"
      $<TARGET_FILE:pydusa>
    DESTINATION
      "${PYDUSA_PACKAGE_PATH}/pydusa")
  install(
    FILES
      "${CMAKE_CURRENT_SOURCE_DIR}/python/setup.py"
      "${CMAKE_CURRENT_SOURCE_DIR}/python/MANIFEST.in"
    DESTINATION
      "${PYDUSA_PACKAGE_PATH}")
  add_custom_command(
    OUTPUT "${PYDUSA_PACKAGE_NAME}"
    COMMAND ${CMAKE_COMMAND} -E tar "cf" "pydusa.tgz" --format=zip "${PYDUSA_PACKAGE_PATH}"
    DEPENDS pydusa
)
endif()

set(CPACK_PACKAGE_EXECUTABLES "medusa" "medusa")

include(CPack)
