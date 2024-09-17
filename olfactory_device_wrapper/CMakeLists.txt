cmake_minimum_required (VERSION 3.14)

###########################
# Project Settings
###########################
set(PROJECT_NAME olfactory_device_wrapper)

include(GNUInstallDirs)

###########################
# Source
###########################
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
add_library(${PROJECT_NAME} STATIC
    ${src}
    ${include}
)

###########################
# Include Directory
###########################
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/olfactory_device/include
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
    # for static link
    # Output header and lib to install folder
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/install/olfactory_device_wrapper_lib/$(Configuration)/olfactory_device/include
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/olfactory_device/include ${CMAKE_SOURCE_DIR}/install/olfactory_device_wrapper_lib/$(Configuration)/olfactory_device/include
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)/${PROJECT_NAME}.lib ${CMAKE_SOURCE_DIR}/install/olfactory_device_wrapper_lib/$(Configuration)/olfactory_device/lib/${PROJECT_NAME}.lib

    # for placing the wrapper directly
    # Output header and source to install folder
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/install/olfactory_device_wrapper_src/$(Configuration)/olfactory_device/include
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/olfactory_device/include ${CMAKE_SOURCE_DIR}/install/olfactory_device_wrapper_src/$(Configuration)/olfactory_device/include
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/install/olfactory_device_wrapper_src/$(Configuration)/olfactory_device/src
)
