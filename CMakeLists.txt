cmake_minimum_required(VERSION 3.22)

project(NanoDegree VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

set(SRC_DIR src/)

list(APPEND INCLUDE_DIRS include)

set(CMAKE_SOURCE_DIR src)
set(CMAKE_CURRENT_SOURCE_DIR src)

add_executable(${PROJECT_NAME}
    main.cpp src/temp.cpp
    include/utils.h
)

target_include_directories(${PROJECT_NAME} PUBLIC $INCLUDE_DIRS)
