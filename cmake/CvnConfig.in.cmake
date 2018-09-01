if (CMAKE_VERSION VERSION_LESS 2.8.9)
    message(FATAL_ERROR "Cvn requires at least CMake version 2.8.9")
endif()

if (NOT Cvn_FIND_COMPONENTS)
    set(Cvn_NOT_FOUND_MESSAGE "The Cvn package requires at least one component")
    set(Cvn_FOUND False)
    return()
endif()

set(_Cvn_FIND_PARTS_REQUIRED)
if (Cvn_FIND_REQUIRED)
    set(_Cvn_FIND_PARTS_REQUIRED REQUIRED)
endif()
set(_Cvn_FIND_PARTS_QUIET)
if (Cvn_FIND_QUIETLY)
    set(_Cvn_FIND_PARTS_QUIET QUIET)
endif()

get_filename_component(_Cvn_install_prefix "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

set(_Cvn_NOTFOUND_MESSAGE)

# Let components find each other, but don't overwrite CMAKE_PREFIX_PATH
set(_Cvn_CMAKE_PREFIX_PATH_old ${CMAKE_PREFIX_PATH})
set(CMAKE_PREFIX_PATH ${_Cvn_install_prefix})

foreach(module ${Cvn_FIND_COMPONENTS})
    find_package(Cvn${module}
        ${_Cvn_FIND_PARTS_QUIET}
        ${_Cvn_FIND_PARTS_REQUIRED}
        PATHS "${_Cvn_install_prefix}" NO_DEFAULT_PATH
    )
    if (NOT Cvn${module}_FOUND)
        if (Cvn_FIND_REQUIRED_${module})
            set(_Cvn_NOTFOUND_MESSAGE "${_Cvn_NOTFOUND_MESSAGE}Failed to find Cvn component \"${module}\" config file at \"${_Cvn_install_prefix}/Cvn${module}/Cvn${module}Config.cmake\"\n")
        elseif(NOT Cvn_FIND_QUIETLY)
            message(WARNING "Failed to find Cvn component \"${module}\" config file at \"${_Cvn_install_prefix}/Cvn${module}/Cvn${module}Config.cmake\"")
        endif()
    endif()

    # For backward compatibility set the LIBRARIES variable
    list(APPEND Cvn_LIBRARIES "Cvn::${module}")
endforeach()

# Restore the original CMAKE_PREFIX_PATH value
set(CMAKE_PREFIX_PATH ${_Cvn_CMAKE_PREFIX_PATH_old})

if (_Cvn_NOTFOUND_MESSAGE)
    set(Cvn_NOT_FOUND_MESSAGE "${_Cvn_NOTFOUND_MESSAGE}")
    set(Cvn_FOUND False)
endif()

