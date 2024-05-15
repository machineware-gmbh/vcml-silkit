cmake_minimum_required(VERSION 3.11)
project(vcml-silkit VERSION 2024.04.29 LANGUAGES C CXX)

include(cmake/common.cmake)

find_package(Git REQUIRED)

set(VCML_SILKIT_TESTS OFF CACHE BOOL "Build unit tests")

option(SILKIT_BUILD_TESTS "" OFF)
option(SILKIT_BUILD_DEMOS "" OFF)
# ToDo: Fix errorneos error msg "Could NOT find SILKIT..."
find_github_repo(SilKit "vectorgrp/sil-kit" "sil-kit/v4.0.48")
if(NOT TARGET SilKit)
    message(FATAL_ERROR "SilKit not found!")
endif()
find_github_repo(vcml "machineware-gmbh/vcml")

set(src ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(inc ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(vcml-silkit STATIC
            ${src}/vcml/models/silkit/participant.cpp
            ${src}/vcml/models/silkit/service.cpp
            ${src}/vcml/models/silkit/service_eth.cpp)

target_compile_options(vcml-silkit PRIVATE -Wall -Werror)
target_compile_definitions(vcml-silkit PRIVATE cxx_std_17)

target_include_directories(vcml-silkit PUBLIC ${inc})
target_include_directories(vcml-silkit PRIVATE ${src})
target_include_directories(vcml-silkit PRIVATE SilKit)

target_link_libraries(vcml-silkit PUBLIC mwr)
target_link_libraries(vcml-silkit PUBLIC vcml)
target_link_libraries(vcml-silkit PUBLIC SilKit)

set_target_properties(vcml-silkit PROPERTIES DEBUG_POSTFIX "d")

if(VCML_SILKIT_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

install(TARGETS vcml-silkit DESTINATION lib)
install(DIRECTORY ${inc}/ DESTINATION include)
