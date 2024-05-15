if(NOT TARGET gtest OR NOT TARGET gmock)
    add_subdirectory(googletest EXCLUDE_FROM_ALL)
endif()

macro(model_test nm)
    add_executable(${nm} ${nm}.cpp testing.cpp)
    target_link_libraries(${nm} SilKit vcml-silkit gtest gmock)
    add_test(NAME ${nm} COMMAND $<TARGET_FILE:${nm}> ${CMAKE_CURRENT_SOURCE_DIR})
    set_tests_properties(${nm} PROPERTIES ENVIRONMENT "${ENVVARS}")
    set_tests_properties(${nm} PROPERTIES TIMEOUT 60)
endmacro()

model_test(participant)
