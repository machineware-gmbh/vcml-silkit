 ##############################################################################
 #                                                                            #
 # Copyright 2025 MachineWare GmbH                                            #
 # All Rights Reserved                                                        #
 #                                                                            #
 # This is unpublished proprietary work and may not be used or disclosed to   #
 # third parties, copied or duplicated in any form, in whole or in part,      #
 # without prior written permission of the authors.                           #
 #                                                                            #
 ##############################################################################

string(TOLOWER ${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR} __silkit_target)
set(__silkit_target "${CMAKE_CURRENT_SOURCE_DIR}/${__silkit_target}")
set(__libsilkitinclude "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(libsilkit SHARED IMPORTED GLOBAL)
target_include_directories(libsilkit SYSTEM INTERFACE ${__libsilkitinclude})
if (MSVC)
    set_target_properties(libsilkit PROPERTIES
        IMPORTED_LOCATION_DEBUG   "${__silkit_target}/SilKitd.dll"
        IMPORTED_IMPLIB_DEBUG     "${__silkit_target}/SilKitd.lib"
        IMPORTED_LOCATION_RELEASE "${__silkit_target}/SilKit.dll"
        IMPORTED_IMPLIB_RELEASE   "${__silkit_target}/SilKit.lib"
        MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release
        MAP_IMPORTED_CONFIG_MINSIZEREL     Release
    )
else()
    set_target_properties(libsilkit PROPERTIES
        IMPORTED_LOCATION "${__silkit_target}/libSilKit.so")
endif()

add_executable(silkit_registry IMPORTED GLOBAL)
set_target_properties(silkit_registry PROPERTIES
    IMPORTED_LOCATION "${__silkit_target}/sil-kit-registry${CMAKE_EXECUTABLE_SUFFIX}")

add_executable(silkit_system_controller IMPORTED GLOBAL)
set_target_properties(silkit_system_controller PROPERTIES
    IMPORTED_LOCATION "${__silkit_target}/sil-kit-system-controller${CMAKE_EXECUTABLE_SUFFIX}")

install(FILES "$<TARGET_FILE:libsilkit>" DESTINATION lib)
install(PROGRAMS "$<TARGET_FILE:silkit_registry>" "$<TARGET_FILE:silkit_system_controller>" DESTINATION lib)
install(DIRECTORY ${__libsilkitinclude}/silkit DESTINATION include)
