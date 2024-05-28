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

if(CMAKE_BUILD_TYPE MATCHES "ASAN")
    string(APPEND env "ASAN_OPTIONS=suppressions=${CMAKE_CURRENT_SOURCE_DIR}/sanitizer/asan.suppress:detect_leaks=1:fast_unwind_on_malloc=0;")
    string(APPEND env "LSAN_OPTIONS=suppressions=${CMAKE_CURRENT_SOURCE_DIR}/sanitizer/lsan.suppress;")
elseif(CMAKE_BUILD_TYPE MATCHES "TSAN")
    string(APPEND env "TSAN_OPTIONS=suppressions=${CMAKE_CURRENT_SOURCE_DIR}/sanitizer/tsan.suppress:halt_on_error=1;")
elseif(CMAKE_BUILD_TYPE MATCHES "UBSAN")
    string(APPEND env "UBSAN_OPTIONS=suppressions=${CMAKE_CURRENT_SOURCE_DIR}/sanitizer/ubsan.suppress:print_stacktrace=1;")
endif()

if(NOT TARGET gtest OR NOT TARGET gmock)
    option(INSTALL_GTEST OFF)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    add_subdirectory(googletest EXCLUDE_FROM_ALL)
endif()

set(script ${CMAKE_CURRENT_BINARY_DIR}/test_fixture)
if(CMAKE_CONFIGURATION_TYPES)
    configure_file(test_fixture.py.in ${script}.py.in)
    file(GENERATE OUTPUT ${script}-$<CONFIG>.py INPUT ${script}.py.in)
else()
    configure_file(test_fixture.py.in ${script}.py)
    file(GENERATE OUTPUT ${script}.py INPUT ${script}.py)
endif()

macro(model_test nm)
    add_executable(${nm} ${nm}.cpp testing.cpp)
    add_dependencies(${nm} sil-kit-registry sil-kit-system-controller)
    target_link_libraries(${nm} SilKit vcml-silkit gtest gmock)
    target_compile_options(${nm} PRIVATE ${MWR_COMPILER_WARN_FLAGS})
    set_target_properties(${nm} PROPERTIES CXX_CLANG_TIDY "${VCML_ARM_LINTER}")
    if(CMAKE_CONFIGURATION_TYPES)
        add_test(NAME ${nm} COMMAND ${Python3_EXECUTABLE} ${script}-$<CONFIG>.py $<TARGET_FILE:${nm}>)
    else()
        add_test(NAME ${nm} COMMAND ${Python3_EXECUTABLE} ${script}.py $<TARGET_FILE:${nm}>)
    endif()
    set_tests_properties(${nm} PROPERTIES ENVIRONMENT "${ENVVARS}")
    set_tests_properties(${nm} PROPERTIES TIMEOUT 60)
endmacro()

model_test(participant)
