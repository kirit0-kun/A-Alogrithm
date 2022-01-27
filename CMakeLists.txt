cmake_minimum_required(VERSION 3.21)

project(NanoDegree VERSION 1.0)

# set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
# set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED 20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

set(SRC_DIR src/)

list(APPEND INCLUDE_DIRS include)

#set(CMAKE_SOURCE_DIR src)
#set(CMAKE_CURRENT_SOURCE_DIR src)


add_executable(${PROJECT_NAME}
    "main.cpp" 
    "src/DoubleDispatch.cpp"
    "include/utils.h"
    "include/DoubleDispatch.h")

target_include_directories(${PROJECT_NAME} PUBLIC $INCLUDE_DIRS)