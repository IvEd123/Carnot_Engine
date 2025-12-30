cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0079 NEW)
cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0091 NEW)

set(PROJECT_NAME Carnot)

project(${PROJECT_NAME})


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(libs)
add_subdirectory(src)

