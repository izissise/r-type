cmake_minimum_required(VERSION 2.8.8)

project(BlorbMonster)

set(NAME_LIB_MONSTER "blorbMonster")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build")
set(LIBRARY_OUTPUT_PATH ${MONSTERDIR})
set(EXECUTABLE_OUTPUT_PATH ${MONSTERDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${MONSTERDIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${MONSTERDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${MONSTERDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${MONSTERDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${MONSTERDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${MONSTERDIR}")

# cmake modules
# set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

INCLUDE(CheckIncludeFile)
include (GenerateExportHeader)
INCLUDE(CheckApplyCompilerFlags)

# --- global compiler flags ---
if (UNIX)
  COMPILER_FLAGS(FLAGS "-pipe" "-Wall" "-Wextra"
			   "--std=c++11"
			   "-Wchar-subscripts" "-Winit-self" "-Wswitch-enum"
			   "-Wstrict-aliasing=3"
			   "-Wundef" "-Wno-endif-labels"
			   "-Wshadow" "-Wno-builtin-macro-redefined"
			   "-Wmissing-declarations"
			   "-Wdocumentation"
			   "-Wno-switch-enum"
		FLAGS_RELEASE  "-march=native" "-O2"
		FLAGS_DEBUG    "-DNDEBUG" "-g3"
			   )
elseif (MSVC)
  COMPILER_FLAGS(FLAGS "/EHsc")
endif()

include_directories(${INCLUDE_DIRECTORY})

set(libmonster_source_files "Monster.cpp")
set(libmonster_include_files "Monster.hpp")

add_library(
# executable name
  ${NAME_LIB_MONSTER} SHARED
# source files
  ${libmonster_source_files}
  ${libmonster_include_files}
)

GENERATE_EXPORT_HEADER( ${NAME_LIB_MONSTER}
             BASE_NAME ${NAME_LIB_MONSTER}
             EXPORT_MACRO_NAME ${NAME_LIB_MONSTER}_EXPORT
             EXPORT_FILE_NAME ${NAME_LIB_MONSTER}_Export.h
             STATIC_DEFINE ${NAME_LIB_MONSTER}_BUILT_AS_STATIC
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})


