# add the modules directories
add_subdirectory(arch)  # Architecture
add_subdirectory(core)  # Core
add_subdirectory(ldr)   # Loader
#add_subdirectory(plg)  # Plugin
add_subdirectory(ui)    # User interface
add_subdirectory(os)    # Operating System
add_subdirectory(emul)  # Emulation
add_subdirectory(db)    # Database
if(BUILD_TESTING)
add_subdirectory(tests)# tests for modules
ENDIF(BUILD_TESTING)
