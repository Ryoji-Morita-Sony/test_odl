cmake_minimum_required (VERSION 3.14)

###########################
# Project Settings
###########################
set(PROJECT_NAME olfactory_device)

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

file(GLOB_RECURSE oscpack
    ${CMAKE_SOURCE_DIR}/third_party/oscpack/osc/*.h
    ${CMAKE_SOURCE_DIR}/third_party/oscpack/ip/*.h
)
source_group("oscpack" FILES ${oscpack})

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
    ${CMAKE_SOURCE_DIR}/third_party/oscpack/
)

###########################
# Link Directory
###########################

###########################
# Link Libraries
###########################
# Link the spdlog
#find_package(spdlog CONFIG REQUIRED)
#target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
find_package(log-settings CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE log-settings::log-settings)

# Link the oscpack library
target_link_libraries(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/third_party/oscpack/build/Debug/oscpack.lib>
    $<$<CONFIG:Release>:${CMAKE_SOURCE_DIR}/third_party/oscpack/build/Release/oscpack.lib>
)

# Link libs used in oscpack
target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32 winmm)

###########################
# Install
###########################

###########################
# Custom Command
###########################
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    # Copy .dll to install folder
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)/${PROJECT_NAME}.dll ${PROJECT_SOURCE_DIR}/install/olfactory_device/$(Configuration)/lib//${PROJECT_NAME}.dll

    # Copy .dll to unit_test executable folder
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)/${PROJECT_NAME}.dll ${PROJECT_SOURCE_DIR}/build/unit_test/$(Configuration)//${PROJECT_NAME}.dll
)
