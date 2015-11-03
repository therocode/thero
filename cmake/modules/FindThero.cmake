set( THERO_FOUND false)

find_path(
    THERO_INCLUDE_DIR
    thero/assert.hpp
    PATH_SUFFIXES
    include
    PATHS
    ${THERO_ROOT}
    ${THERODIR}
    $ENV{THERODIR}
    $ENV{THERO_ROOT}
    /usr
    /usr/local
    ~/Library/Frameworks
    /Library/Frameworks
    /sw
    /opt/local
    /opt/csw
    /opt
    "C:/Program Files (x86)"
    "C:/Program Files (x86)/thero"
)

set(THERO_FOUND)
set(THERO_INCLUDE_DIR)

if(NOT THERO_INCLUDE_DIR OR FIND_Thero_MISSING)
    set(THERO_FOUND)
    set(THERO_INCLUDE_DIRS)
    if(Thero_FIND_REQUIRED)
        message(FATAL_ERROR "Thero not found.")
    elseif(NOT Thero_FIND_QUIETLY)
        message(STATUS "Thero not found.")
    endif()
else()
    set(THERO_FOUND true)
    if(NOT Thero_FIND_QUIETLY)
        message(STATUS "Thero found: ${THERO_INCLUDE_DIR}")
    endif()
    set(THERO_INCLUDE_DIRS ${THERO_INCLUDE_DIR})
endif()
