 ##############################################################################
 #                                                                            #
 # Copyright (C) 2024 MachineWare GmbH                                        #
 # All Rights Reserved                                                        #
 #                                                                            #
 # This is work is licensed under the terms described in the LICENSE file     #
 # found in the root directory of this source tree.                           #
 #                                                                            #
 ##############################################################################

function(unit_test nm mode)
    set(script ${CMAKE_CURRENT_BINARY_DIR}/${nm}-${mode})
    if(CMAKE_CONFIGURATION_TYPES)
        configure_file(fixture.py.in ${script}.py.in)
        file(GENERATE OUTPUT ${script}-$<CONFIG>.py INPUT ${script}.py.in)
    else()
        configure_file(fixture.py.in ${script}.py)
        file(GENERATE OUTPUT ${script}.py INPUT ${script}.py)
    endif()

    if(NOT TARGET ${nm})
        add_executable(${nm} ${nm}.cpp)
        target_link_libraries(${nm} testing libsilkit vcml-silkit gtest gmock)
        target_compile_options(${nm} PRIVATE ${MWR_COMPILER_WARN_FLAGS})
        set_target_properties(${nm} PROPERTIES CXX_CLANG_TIDY "${VCML_SILKIT_LINTER}")
        if (WIN32)
            add_custom_command(TARGET ${nm} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:libsilkit> $<TARGET_FILE_DIR:${nm}>)
        endif()
    endif()

    set(test ${nm}-${mode})
    if(CMAKE_CONFIGURATION_TYPES)
        add_test(NAME ${test} COMMAND ${Python3_EXECUTABLE} ${script}-$<CONFIG>.py)
    else()
        add_test(NAME ${test} COMMAND ${Python3_EXECUTABLE} ${script}.py)
    endif()
    set_tests_properties(${test} PROPERTIES ENVIRONMENT "${ENVVARS}")
    set_tests_properties(${test} PROPERTIES TIMEOUT 60)
endfunction()

foreach(mode IN ITEMS coordinated time_sync autonomous)
    unit_test(participant ${mode} vcml_participant)
    unit_test(service_eth ${mode} vcml_participant0 vcml_participant1)
endforeach()
