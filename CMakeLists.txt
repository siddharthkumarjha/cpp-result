cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
project(cpp-result 
    VERSION 0.1
    LANGUAGES CXX
)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND SRCS ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
list(APPEND TEST_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/test_result.cpp)
list(APPEND BENCHMARK_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/benchmark_result.cpp)

add_executable(${PROJECT_NAME} ${SRCS})
add_executable(${PROJECT_NAME}_tests ${TEST_SRCS})
add_executable(${PROJECT_NAME}_benchmark ${BENCHMARK_SRCS})

# Enable testing
enable_testing()
add_test(NAME result_tests COMMAND ${PROJECT_NAME}_tests)

# Custom targets for easy building
add_custom_target(test_all 
    COMMAND ${PROJECT_NAME}_tests
    DEPENDS ${PROJECT_NAME}_tests
    COMMENT "Running all tests"
)

add_custom_target(benchmark 
    COMMAND ${PROJECT_NAME}_benchmark
    DEPENDS ${PROJECT_NAME}_benchmark
    COMMENT "Running performance benchmarks"
)
