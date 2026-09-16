cmake_minimum_required(VERSION 3.15)
project(CO2203_LMS CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB LMS_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# One library holding every class from all three members' subsystems.
add_library(lms_core STATIC ${LMS_SOURCES})
target_include_directories(lms_core PUBLIC ${CMAKE_SOURCE_DIR}/include)

# The real program.
add_executable(lms main.cpp)
target_link_libraries(lms PRIVATE lms_core)
