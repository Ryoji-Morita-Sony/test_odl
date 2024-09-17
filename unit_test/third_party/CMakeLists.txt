cmake_minimum_required (VERSION 3.14)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(googletest-release-1.12.1)

set_target_properties(gmock PROPERTIES FOLDER googletest)
set_target_properties(gmock_main PROPERTIES FOLDER googletest)
set_target_properties(gtest PROPERTIES FOLDER googletest)
set_target_properties(gtest_main PROPERTIES FOLDER googletest)
