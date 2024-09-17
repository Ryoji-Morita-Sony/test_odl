cmake_minimum_required (VERSION 3.14)

###########################
# Project Settings
###########################
set(PROJECT_NAME unit_test)

add_subdirectory(third_party)

###########################
# Source code
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
# Exe
###########################
add_executable(${PROJECT_NAME}
    ${src}
    ${include}
)

###########################
# Include Directory
###########################
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/hello_world/include

    third_party/googletest-release-1.12.1/googletest/include
)

###########################
# Link Directory
###########################

###########################
# Link Libraries
###########################
add_dependencies(${PROJECT_NAME}
    hello_world
    gmock
    gmock_main
    gtest
    gtest_main
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    hello_world
    gmock
    gmock_main
    gtest
    gtest_main
)

###########################
# Custom command
###########################
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/build/hello_world/$(Configuration)/hello_world.dll ${CMAKE_SOURCE_DIR}/build/${PROJECT_NAME}/$(Configuration)/hello_world.dll
)
