cmake_minimum_required (VERSION 3.14)

###########################
# Project Settings
###########################
set(PROJECT_NAME hello_world)

include(GNUInstallDirs)

###########################
# Source
###########################
# main source
file(GLOB_RECURSE src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)
source_group("src" FILES ${src})

file(GLOB_RECURSE include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
)
source_group("include" FILES ${include})

###########################
# Library
###########################
add_library(${PROJECT_NAME} SHARED
    ${src}
    ${include}
)

target_compile_options(${PROJECT_NAME} PRIVATE -DBUILD_DLL)

###########################
# Include Directory
###########################
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

###########################
# Link Directory
###########################

###########################
# Link Libraries
###########################

###########################
# Install
###########################

###########################
# Custom Command
###########################
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    # Copy .dll to install folder
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)/${PROJECT_NAME}.dll ${PROJECT_SOURCE_DIR}/install/hello_world/$(Configuration)/lib//${PROJECT_NAME}.dll

    # Copy .dll to unit_test executable folder
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)/${PROJECT_NAME}.dll ${PROJECT_SOURCE_DIR}/build/unit_test/$(Configuration)//${PROJECT_NAME}.dll
)
