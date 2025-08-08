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

if (MSVC)
    set(__silkit_dll "SilKit$<$<CONFIG:Debug>:d>.dll")
    set(__libsilkit "SilKit$<$<CONFIG:Debug>:d>.lib")
    set(__silkit_registry "sil-kit-registry.exe")
    set(__silkit_system_controller "sil-kit-system-controller.exe")
else()
    set(__libsilkit "libSilKit.so")
    set(__silkit_registry "sil-kit-registry")
    set(__silkit_system_controller "sil-kit-system-controller")
endif()

string(TOLOWER ${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR} __silkit_target)
set(__libsilkit "${CMAKE_CURRENT_SOURCE_DIR}/${__silkit_target}/${__libsilkit}")
set(__silkit_dll "${CMAKE_CURRENT_SOURCE_DIR}/${__silkit_target}/${__silkit_dll}")
set(__silkit_registry "${CMAKE_CURRENT_SOURCE_DIR}/${__silkit_target}/${__silkit_registry}")
set(__silkit_system_controller "${CMAKE_CURRENT_SOURCE_DIR}/${__silkit_target}/${__silkit_system_controller}")
set(__libsilkitinclude "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(libsilkit SHARED IMPORTED GLOBAL)
target_include_directories(libsilkit SYSTEM INTERFACE ${__libsilkitinclude})
if (MSVC)
    set_target_properties(libsilkit PROPERTIES
        IMPORTED_IMPLIB "${__libsilkit}"
        IMPORTED_LOCATION "${__silkit_dll}"
    )
else()
    set_target_properties(libsilkit PROPERTIES IMPORTED_LOCATION ${__libsilkit})
endif()

add_executable(silkit_registry IMPORTED GLOBAL)
set_target_properties(silkit_registry PROPERTIES IMPORTED_LOCATION "${__silkit_registry}")

add_executable(silkit_system_controller IMPORTED GLOBAL)
set_target_properties(silkit_system_controller PROPERTIES IMPORTED_LOCATION "${__silkit_system_controller}")

if (MSVC)
    install(FILES ${__silkit_dll} DESTINATION lib)
else()
    install(FILES ${__libsilkit} DESTINATION lib)
endif()

install(FILES ${__silkit_registry} DESTINATION lib)
install(FILES ${__silkit_system_controller} DESTINATION lib)
install(DIRECTORY ${__libsilkitinclude}/silkit DESTINATION include)
