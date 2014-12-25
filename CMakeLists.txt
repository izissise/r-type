cmake_minimum_required(VERSION 2.8.8)

project(R-Type)

set(NAME_LIB "common")
set(NAME_CLIENT "client")
set(NAME_SERVER "server")
set(TEST_EXEC "UnitTester")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build")
set(INCLUDE_DIRECTORY "${PROJECT_SOURCE_DIR}/${NAME_LIB}/inc/"
					  "${PROJECT_SOURCE_DIR}/${NAME_DLL}/inc/"
					  "${PROJECT_SOURCE_DIR}/${NAME_CLIENT}/inc/"
					  "${PROJECT_SOURCE_DIR}/${NAME_SERVER}/inc/")
set(SRC_DIRECTORY "${PROJECT_SOURCE_DIR}/${NAME_LIB}/src/"
				  "${PROJECT_SOURCE_DIR}/${NAME_DLL}/src/"
				  "${PROJECT_SOURCE_DIR}/${NAME_CLIENT}/src/"
				  "${PROJECT_SOURCE_DIR}/${NAME_SERVER}/src/")
set(DOC_PATH "${PROJECT_SOURCE_DIR}/doc")
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

# cmake modules
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)
find_package(Threads REQUIRED)
find_package(SFML 2 COMPONENTS system window graphics audio)

INCLUDE(CMakeDependentOption)
INCLUDE(CheckIncludeFile)
INCLUDE(InstallFilesRecursive)
INCLUDE(EnableCoverageReport)
INCLUDE(EnableSlocCount)
INCLUDE(GenerateDoxygen)
INCLUDE(GenerateCppcheck)
INCLUDE(GenerateCccc)
INCLUDE(GenerateValgrind)
INCLUDE(DefineProjectVersion)
INCLUDE(CurrentDate)
INCLUDE(DemangleCppABI)
INCLUDE(CheckApplyCompilerFlags)
INCLUDE(GenerateExportHeader)

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
  COMPILER_FLAGS(FLAGS "/EHsc" "/MD" "/MT")
endif()

# --- user options ---
OPTION(BUILD_TESTS "Decide if the test suite shall be built or not." OFF)
CMAKE_DEPENDENT_OPTION(BUILD_TESTS_COVERAGE "Decide if a coverage report should be generated." ON "BUILD_TESTS" OFF)
CMAKE_DEPENDENT_OPTION(VALGRIND_CHECK "Decide if valgrind should be executed with the test binary to check for memory leaks." ON "BUILD_TESTS" OFF)
OPTION(CODE_METRICS "Decide if code's metrics should be generated." OFF)
OPTION(CODE_CHECK   "Decide if code should be checked for common errors." OFF)
OPTION(GENERATE_DOC "Decide if documentation should be generated." OFF)
OPTION(BUILD_SERVER "Does the compilation will build the server ?." ON)
OPTION(BUILD_CLIENT "Does the compilation will build the client ?." ON)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/common/inc")

# NetworkLib
# Build it here in ${CMAKE_CURRENT_BINARY_DIR}/network
set(NetworkLib_DIR "${CMAKE_CURRENT_BINARY_DIR}/network")
file(MAKE_DIRECTORY ${NetworkLib_DIR})
execute_process(COMMAND ${CMAKE_COMMAND} "${PROJECT_SOURCE_DIR}/network" "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}" "-G" "${CMAKE_GENERATOR}" WORKING_DIRECTORY "${NetworkLib_DIR}")
execute_process(COMMAND ${CMAKE_COMMAND} "--build" "${NetworkLib_DIR}" WORKING_DIRECTORY "${NetworkLib_DIR}")
find_package(NetworkLib REQUIRED)

# All source files using globs
file(
  GLOB
  lib_source_files
  ${NAME_LIB}/src/*
  ${NAME_LIB}/src/Packet/*
)

file(
  GLOB_RECURSE
  lib_include_files
  ${NAME_LIB}/inc/*
)

file(
  GLOB_RECURSE
  client_source_files
  ${NAME_CLIENT}/src/*
)

file(
  GLOB_RECURSE
  client_include_files
  ${NAME_CLIENT}/inc/*
)

file(
  GLOB_RECURSE
  server_source_files
  ${NAME_SERVER}/src/*
)

file(
  GLOB_RECURSE
  server_include_files
  ${NAME_SERVER}/inc/*
)

if (WIN32)
 set(BUILDTARGET "WIN32")
 file(
   GLOB
   lib_source_files_win
   ${NAME_LIB}/src/Win/*
 )
 set(lib_source_files ${lib_source_files} ${lib_source_files_win})
elseif(UNIX)
 set(BUILDTARGET "UNIX")
 file(
   GLOB
   lib_source_files_unix
   ${NAME_LIB}/src/Unix/*
 )
 set(lib_source_files ${lib_source_files} ${lib_source_files_unix})
 find_package(DL REQUIRED)
 if(APPLE)
  set(BUILDTARGET "APPLE")
 endif()
endif()

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "common/inc/Config.h.in"
  "common/inc/RTypeConfig.h"
)

add_library(
# executable name
  ${NAME_LIB} STATIC
# source files
  ${lib_source_files}
  ${lib_include_files}
)

if (UNIX)
 target_link_libraries(${NAME_LIB} ${DL_LIBRARIES})
 include_directories(${DL_INCLUDE_DIR})
endif()

include_directories("${NETWORKLIB_INCLUDE_DIRS}")
set_property(TARGET ${NAME_LIB} APPEND PROPERTY INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/${NAME_LIB}/inc")

target_link_libraries(${NAME_LIB} NETWORKLIB_network)
target_link_libraries(${NAME_LIB} ${CMAKE_THREAD_LIBS_INIT})

if(BUILD_CLIENT)
    add_executable(
  # executable name
    ${NAME_CLIENT}
 # source files
    ${client_source_files}
    ${client_include_files}
  )

  set_property(TARGET ${NAME_CLIENT} APPEND PROPERTY INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/${NAME_CLIENT}/inc")
  set_property(TARGET ${NAME_CLIENT} APPEND PROPERTY INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/${NAME_LIB}/inc")
  target_link_libraries(${NAME_CLIENT} ${NAME_LIB})
endif()

if (BUILD_SERVER)
  add_executable(
    ${NAME_SERVER}
  # source files
    ${server_source_files}
    ${server_include_files}
  )

  set_property(TARGET ${NAME_SERVER} APPEND PROPERTY INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/${NAME_SERVER}/inc")
  set_property(TARGET ${NAME_SERVER} APPEND PROPERTY INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/${NAME_LIB}/inc")
  target_link_libraries(${NAME_SERVER} ${NAME_LIB})
endif()

file(
  GLOB_RECURSE
  test_files
  test/*
)

if (THREADS_FOUND)
  target_link_libraries (${NAME_LIB} ${CMAKE_THREAD_LIBS_INIT})
endif()

if (SFML_FOUND)
  include_directories("${SFML_INCLUDE_DIR}")
  target_link_libraries (${NAME_CLIENT} ${SFML_LIBRARIES})
endif()


#Compile Monsters
add_subdirectory("monsters")


# --- enable unit tests if desired ---
if(BUILD_TESTS)
  if(NOT CPPUNIT_FOUND)
    MESSAGE(WARNING "CppUnit library can't be found, Unit test won't be compiled")
  else()
    add_executable(
    # executable name
      ${TEST_EXEC}
    # source files
     ${test_files}
    )

    if(BUILD_TESTS_COVERAGE)
      # --- coverage report ---
      ENABLE_COVERAGE_REPORT(TARGETS ${TEST_EXEC} TESTS ${test_files})
    endif()

    if(VALGRIND_CHECK)
      # --- Valgrind --- #
      GENERATE_VALGRIND(BIN "${CMAKE_CURRENT_BINARY_DIR}/${TEST_EXEC}")
    endif()

    set(TEST_REPORT_FILE "cpptestresults.xml")
    # Execute ${CMAKE_CURRENT_BINARY_DIR}/${TEST_EXEC} binary to execute tests
    ADD_CUSTOM_COMMAND(OUTPUT ${TEST_REPORT_FILE}
                           DEPENDS ${TEST_EXEC}
                           COMMAND "${CMAKE_CURRENT_BINARY_DIR}/${TEST_EXEC}" ARGS "--xml-file=${TEST_REPORT_FILE}"
                           WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    ADD_CUSTOM_TARGET(launchtest DEPENDS ${TEST_REPORT_FILE})

    include_directories(${CPPUNIT_INCLUDE_DIRS})
    target_link_libraries (${TEST_EXEC} ${CPPUNIT_LIBRARY} ${NAME_LIB})

  endif()
endif()

#Extra targets:
if(CODE_CHECK)
  # --- cppcheck ---
  GENERATE_CPPCHECK(SOURCES ${SRC_DIRECTORY}
                            "${PROJECT_SOURCE_DIR}/test"
                            ENABLE_IDS style
                            INLINE_SUPPRESSION)
endif()

if(CODE_METRICS)
  # --- cccc ---
  GENERATE_CCCC(SOURCES ${SRC_DIRECTORY} ${INCLUDE_DIRECTORY}
                        "${PROJECT_SOURCE_DIR}/test")
  # --- SlocCount --- #
  ENABLE_SLOCCOUNT(FOLDERS ${SRC_DIRECTORY} ${INCLUDE_DIRECTORY}
                        "${PROJECT_SOURCE_DIR}/test")
endif()

if(GENERATE_DOC)
  # --- Doxygen --- #
  GENERATE_DOXYGEN(SOURCES ${SRC_DIRECTORY} ${INCLUDE_DIRECTORY}
                        "${PROJECT_SOURCE_DIR}/test" ${DOC_PATH}
                        VERSION "${PROJECTVERSION}")
endif()
