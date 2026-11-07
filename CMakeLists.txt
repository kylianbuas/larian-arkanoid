cmake_minimum_required(VERSION 3.20)

project(arkanoid)

if (MSVC)
    add_compile_options(/MP)
endif()

set(RELEASE "Release")
set(DEBUG "Debug")
set(CMAKE_CONFIGURATION_TYPES "${RELEASE};${DEBUG}" CACHE STRING "" FORCE)
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

if(MSVC)
	set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG" CACHE STRING "Debug linker flags" FORCE)
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/OPT:REF /OPT:ICF" CACHE STRING "Release linker flags" FORCE)
endif()

# Global settings
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER CMakePredefinedTargets)

add_compile_definitions($<$<CONFIG:Debug>:_DEBUG>)

# Third-party libraries

# Include modules
include(arkanoid_lib/arkanoid_lib.cmake)

# Libraries
set_property(TARGET arkanoid_lib PROPERTY FOLDER "lib")

# Platform-specific modules (Win32 only)
if(WIN32)
	include(arkanoid_game/arkanoid_game.cmake)
	set_property(TARGET arkanoid_game PROPERTY FOLDER "executables")

	include(arkanoid_level_editor/arkanoid_level_editor.cmake)
	set_property(TARGET arkanoid_level_editor PROPERTY FOLDER "executables")
endif()