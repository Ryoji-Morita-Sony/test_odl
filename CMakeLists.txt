cmake_minimum_required (VERSION 3.14)

# Set available solution configurations.
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

# Set compiler options common to all target platform.
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Don't use compiler extensions such as gnu++11

# Enable project filtering
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Project name
project ("OlfactoryDeviceLibrary")

add_subdirectory(olfactory_device)
add_subdirectory(olfactory_device_wrapper)
add_subdirectory(unit_test)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT unit_test)
