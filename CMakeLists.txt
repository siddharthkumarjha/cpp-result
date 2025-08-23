cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
project(cpp-result 
    VERSION 0.1
    LANGUAGES CXX
)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND SRCS ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

add_executable(${PROJECT_NAME} ${SRCS})
