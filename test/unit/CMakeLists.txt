 ##############################################################################
 #                                                                            #
 # Copyright (C) 2024 MachineWare GmbH                                        #
 # All Rights Reserved                                                        #
 #                                                                            #
 # This is work is licensed under the terms described in the LICENSE file     #
 # found in the root directory of this source tree.                           #
 #                                                                            #
 ##############################################################################

function(unit_test nm part)
    set(script ${CMAKE_CURRENT_BINARY_DIR}/${nm})
    if(CMAKE_CONFIGURATION_TYPES)
        configure_file(fixture.py.in ${script}.py.in)
        file(GENERATE OUTPUT ${script}-$<CONFIG>.py INPUT ${script}.py.in)
    else()
        configure_file(fixture.py.in ${script}.py)
        file(GENERATE OUTPUT ${script}.py INPUT ${script}.py)
    endif()

    add_executable(${nm} ${nm}.cpp)
    add_dependencies(${nm} sil-kit-registry sil-kit-system-controller)
    target_link_libraries(${nm} testing SilKit vcml-silkit gtest gmock)
    target_compile_options(${nm} PRIVATE ${MWR_COMPILER_WARN_FLAGS})
    set_target_properties(${nm} PROPERTIES CXX_CLANG_TIDY "${VCML_SILKIT_LINTER}")

    if(CMAKE_CONFIGURATION_TYPES)
        add_test(NAME ${nm} COMMAND ${Python3_EXECUTABLE} ${script}-$<CONFIG>.py)
    else()
        add_test(NAME ${nm} COMMAND ${Python3_EXECUTABLE} ${script}.py)
    endif()
    set_tests_properties(${nm} PROPERTIES ENVIRONMENT "${ENVVARS}")
    set_tests_properties(${nm} PROPERTIES TIMEOUT 60)
endfunction()

unit_test(participant vcml_participant)
