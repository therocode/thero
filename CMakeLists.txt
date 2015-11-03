#cmake ver req
cmake_minimum_required(VERSION 2.8.3)

#project name
set(project_name thero)
project(${project_name})

if(WIN32)
    set(THERO_MISC_DIR .)
    set(WINFIX SYSTEM)
else()
    set(THERO_MISC_DIR share/thero)
endif()

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/modules/")

set(thero_header_files
    include/thero/assert.hpp
    include/thero/copyablepolymmorphic.hpp
    include/thero/moduleset.hpp
)

install(
    DIRECTORY include
    DESTINATION .
)

install(
    FILES README.md LICENSE
    DESTINATION ${THERO_MISC_DIR}
)
