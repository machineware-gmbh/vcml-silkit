 ##############################################################################
 #                                                                            #
 # Copyright (C) 2024 MachineWare GmbH                                        #
 # All Rights Reserved                                                        #
 #                                                                            #
 # This is work is licensed under the terms described in the LICENSE file     #
 # found in the root directory of this source tree.                           #
 #                                                                            #
 ##############################################################################

cmake_minimum_required(VERSION 3.16)
project(vcml-silkit VERSION 2025.08.19 LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(FindGithubRepo)
include(GenVersionInfo)
include(Sanitizer)

find_package(Git REQUIRED)

option(VCML_SILKIT_TESTS "Build unit tests" OFF)
option(VCML_SILKIT_COVERAGE "Generate coverage data" OFF)
set(VCML_SILKIT_LINTER "" CACHE STRING "Code linter to use")

find_github_repo(vcml "machineware-gmbh/vcml")

set(src ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(inc ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(ext ${CMAKE_CURRENT_SOURCE_DIR}/external)
set(gen ${CMAKE_CURRENT_BINARY_DIR}/gen)

add_subdirectory(${ext}/silkit-4.0.56)

configure_file(${src}/vcml/silkit/version.h.in
               ${gen}/vcml/silkit/version.h @ONLY)

add_library(vcml-silkit STATIC
            ${src}/vcml/silkit/participant.cpp
            ${src}/vcml/silkit/service.cpp
            ${src}/vcml/silkit/service_eth.cpp
            ${src}/vcml/silkit/service_can.cpp)

target_compile_options(vcml-silkit PRIVATE ${MWR_COMPILER_WARN_FLAGS})
target_compile_definitions(vcml-silkit PRIVATE cxx_std_17)

target_include_directories(vcml-silkit PUBLIC ${inc})
target_include_directories(vcml-silkit PRIVATE ${src})
target_include_directories(vcml-silkit PUBLIC ${gen})
target_include_directories(vcml-silkit SYSTEM PRIVATE ${SILKIT_INCLUDE_DIR})

target_link_libraries(vcml-silkit PUBLIC mwr)
target_link_libraries(vcml-silkit PUBLIC vcml)
target_link_libraries(vcml-silkit PRIVATE libsilkit)

set_target_properties(vcml-silkit PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(vcml-silkit PROPERTIES CXX_CLANG_TIDY "${VCML_SILKIT_LINTER}")

if(VCML_SILKIT_COVERAGE)
    target_compile_options(vcml-silkit PUBLIC --coverage)
    target_link_libraries(vcml-silkit PUBLIC -lgcov)
endif()

if(VCML_SILKIT_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

install(TARGETS vcml-silkit DESTINATION lib)
install(DIRECTORY ${inc}/ DESTINATION include)

install(FILES ${LIBSILKIT_LIB} DESTINATION bin)
install(PROGRAMS ${LIBSILKIT_PROGRAMS} DESTINATION bin)
install(DIRECTORY ${LIBSILKIT_HEADERS} DESTINATION include)
