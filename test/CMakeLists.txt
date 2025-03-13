 ##############################################################################
 #                                                                            #
 # Copyright (C) 2024 MachineWare GmbH                                        #
 # All Rights Reserved                                                        #
 #                                                                            #
 # This is work is licensed under the terms described in the LICENSE file     #
 # found in the root directory of this source tree.                           #
 #                                                                            #
 ##############################################################################

find_package(Python3 "3.3" REQUIRED)

if(NOT TARGET gtest OR NOT TARGET gmock)
    option(INSTALL_GTEST OFF)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    add_subdirectory(googletest EXCLUDE_FROM_ALL)
endif()

if(CMAKE_BUILD_TYPE MATCHES "ASAN")
    string(APPEND env "ASAN_OPTIONS=suppressions=${CMAKE_CURRENT_SOURCE_DIR}/sanitizer/asan.suppress:detect_leaks=1:fast_unwind_on_malloc=0;")
    string(APPEND env "LSAN_OPTIONS=suppressions=${CMAKE_CURRENT_SOURCE_DIR}/sanitizer/lsan.suppress;")
elseif(CMAKE_BUILD_TYPE MATCHES "TSAN")
    string(APPEND env "TSAN_OPTIONS=suppressions=${CMAKE_CURRENT_SOURCE_DIR}/sanitizer/tsan.suppress:halt_on_error=1;")
elseif(CMAKE_BUILD_TYPE MATCHES "UBSAN")
    string(APPEND env "UBSAN_OPTIONS=suppressions=${CMAKE_CURRENT_SOURCE_DIR}/sanitizer/ubsan.suppress:print_stacktrace=1:halt_on_error=1;")
endif()

add_library(testing STATIC testing.cpp)
target_include_directories(testing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(testing vcml-silkit gtest gmock)

add_subdirectory(unit)
