cmake_minimum_required(VERSION 3.29)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(
    FATAL_ERROR
      "Do not build in-source. Please remove CMakeCache.txt and the CMakeFiles/ directory. Then build out-of-source."
  )
endif()

project(
  2d-game-physics-engine
  VERSION 1.0.0
  LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find SDL2 packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_GFX REQUIRED SDL2_gfx)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

# Fallback for systems without pkg-config or if SDL2 not found
if(NOT SDL2_FOUND)
  if(APPLE)
    # For Apple MacOS (homebrew)
    set(SDL2_INCLUDE_DIRS /opt/homebrew/include/SDL2)
    set(SDL2_LIBRARIES SDL2)
    set(SDL2_LIBRARY_DIRS /opt/homebrew/lib)
  else()
    message(FATAL_ERROR "SDL2 not found. Please install SDL2 development libraries.")
  endif()
endif()

# Fallback for SDL2_gfx if not found via pkg-config
if(NOT SDL2_GFX_FOUND)
  if(APPLE)
    set(SDL2_GFX_INCLUDE_DIRS /opt/homebrew/include/SDL2)
    set(SDL2_GFX_LIBRARIES SDL2_gfx)
    set(SDL2_GFX_LIBRARY_DIRS /opt/homebrew/lib)
  else()
    message(FATAL_ERROR "SDL2_gfx not found. Please install SDL2_gfx development libraries.")
  endif()
endif()

# Fallback for SDL2_image if not found via pkg-config
if(NOT SDL2_IMAGE_FOUND)
  if(APPLE)
    set(SDL2_IMAGE_INCLUDE_DIRS /opt/homebrew/include/SDL2)
    set(SDL2_IMAGE_LIBRARIES SDL2_image)
    set(SDL2_IMAGE_LIBRARY_DIRS /opt/homebrew/lib)
  else()
    message(FATAL_ERROR "SDL2_image not found. Please install SDL2_image development libraries.")
  endif()
endif()

set(CMAKE_C_COMPILER gcc)
if(MSVC)
  # warning level 4
  add_compile_options(/W4)
else()
  # additional warnings
  add_compile_options(-Wall -Wextra -Werror -g -O1)
endif()

if(APPLE)
  add_compile_options(-mmacosx-version-min=10.15)
endif()

# ##############################################################################
# Modules and scripts   ##
# ##############################################################################

# Standard CMake modules
include(CTest) # Must be called before adding tests but after calling project().
               # This automatically calls enable_testing() and configures ctest
               # targets when using Make/Ninja
include(CMakeDependentOption) # This is a really useful scripts that creates
                              # options that depends on other options. It can
                              # even be used with generator expressions !
include(GNUInstallDirs) # This will define the default values for installation
                        # directories (all platforms even if named GNU)
include(InstallRequiredSystemLibraries) # Tell CMake that the `install` target
                                        # needs to install required system
                                        # libraries (eg: Windows SDK)
include(CMakePackageConfigHelpers) # Helper to create relocatable packages

# Custom modules and scripts
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake") # Make our
                                                                 # cmake scripts
                                                                 # available

# ##############################################################################
# Options  ##
# ##############################################################################

option(
  ENABLE_INSTALL
  "Should ${PROJECT_NAME} be added to the install list? Useful if included using add_subdirectory."
  ON)
option(ENABLE_TESTING "Should unit tests be compiled." OFF)

set(${PROJECT_NAME}_INSTALL_CMAKEDIR
    "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    CACHE STRING "Path to install ${PROJECT_NAME} Config*.cmake files to.")

# ##############################################################################
# Dependencies  ##
# ##############################################################################

if(ENABLE_TESTING)
  find_package(GTest REQUIRED)
  include(GoogleTest)
  enable_testing()
endif()

# ##############################################################################
# Targets  ##
# ##############################################################################

# Use the project root to find includes
include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(src)
# add_subdirectory(tests)