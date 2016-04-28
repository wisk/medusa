# add architecture directories
if (NOT MEDUSA_BUILD_X86_ARCH_AND_LOADER_ONLY)
 add_subdirectory(arm)	# ARM
 #add_subdirectory(avr8)	# AVR 8-bit
 add_subdirectory(z80)  # Zilog 80
 add_subdirectory(st62)
endif()
add_subdirectory(x86)	# Intel




