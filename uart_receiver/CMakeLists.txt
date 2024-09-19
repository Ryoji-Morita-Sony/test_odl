# CMakeLists.txt for the UART receiver project
cmake_minimum_required(VERSION 3.14)

# Define the executable for the receiver program
add_executable(uart_receiver main.cpp)

###########################
# Custom Command
###########################
add_custom_command(TARGET uart_receiver POST_BUILD
    # Copy .exe to install folder
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)/uart_receiver.exe ${PROJECT_SOURCE_DIR}/install/uart_receiver/$(Configuration)/bin/uart_receiver.exe

    # Copy .exe to unit_test executable folder
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)/uart_receiver.exe ${PROJECT_SOURCE_DIR}/build/unit_test/$(Configuration)/uart_receiver.exe
)
