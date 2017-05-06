# Author: Ben Kane

cmake_minimum_required(VERSION 3.0)

# Name the Project that holds all other projects
# Prefix with underscore for easy finding :)
project(_Algorithms)

# add warnings to all projects
if(MSVC)
    add_compile_options(/W4 /wd4100)
else(MSVC)
    add_compile_options(-Wall -Wextra -pedantic -Wno-unused-parameter)
    if(APPLE)
    	add_compile_options(-std=c++1y)
    else()
        add_compile_options(-std=c++14)
    endif()
endif(MSVC)

# set output for all projects
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)

# Usage: create_unmade <file1>...
# creates files if they don't exist. Starts looking in the directory of the CMakeLists.txt file it's called in
function(create_unmade)
    foreach(word ${ARGV}) # ${ARGV} expands to all arguments
        if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${word})
            message("---- creating: ${word} ----")
            file(WRITE ${word} "")
        endif()
    endforeach()
endfunction()

# Usage: add_proj <name> <file1>...
# creates a simple project from files. Files are placed in a folder called name
function(add_proj name)
    foreach(word ${ARGN})  # ${ARGN} expands to all unexpected arguments
        set(word src/${name}/${word}) # prefix every filename with the project name
        set(Files_To_Make ${Files_To_Make} ${word})
    endforeach()
    create_unmade(${Files_To_Make})
    source_group(${name}_Files FILES ${Files_To_Make})
    add_executable(${name} ${Files_To_Make})
endfunction()

add_proj(Dijkstra main.cpp)
add_proj(tmp main.cpp)
