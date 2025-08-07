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
    set(__libsilkit_dll "SilKit$<$<CONFIG:Debug>:d>.dll")
    set(__libsilkit "SilKit$<$<CONFIG:Debug>:d>.lib")
    set(__silkit_registry "sil-kit-registry.exe")
else()
    set(__libsilkit "libSilKit.so")
    set(__silkit_registry "sil-kit-registry")
endif()

string(TOLOWER ${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR} __silkit_target)
set(__libsilkit "${CMAKE_CURRENT_SOURCE_DIR}/${__silkit_target}/${__libsilkit}")
set(__libsilkit_registry "${CMAKE_CURRENT_SOURCE_DIR}/${__silkit_target}/${__silkit_registry}")
set(__libsilkitinclude "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(libsilkit SHARED IMPORTED GLOBAL)
set_target_properties(libsilkit PROPERTIES IMPORTED_IMPLIB ${__libsilkit})
target_include_directories(libsilkit SYSTEM INTERFACE ${__libsilkitinclude})

if (MSVC)
    install(FILES ${__libsilkit_dll} DESTINATION lib)
endif()
install(FILES ${__libsilkit} DESTINATION lib)
install(FILES ${__libsilkit_registry} DESTINATION lib)
install(DIRECTORY ${__libsilkitinclude}/silkit DESTINATION include)
