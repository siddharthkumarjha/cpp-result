cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
project(cpp-result 
    VERSION 0.1
    LANGUAGES CXX
)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND SRCS_1 ${CMAKE_CURRENT_SOURCE_DIR}/examples/main.cpp)
list(APPEND SRCS_2 ${CMAKE_CURRENT_SOURCE_DIR}/examples/example.cpp)

list(APPEND TEST_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/examples/test_result.cpp)
list(APPEND BENCHMARK_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/examples/benchmark_result.cpp)

list(APPEND INC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME}_1 ${SRCS_1})
add_executable(${PROJECT_NAME}_2 ${SRCS_2})

add_executable(${PROJECT_NAME}_tests ${TEST_SRCS})
add_executable(${PROJECT_NAME}_benchmark ${BENCHMARK_SRCS})

target_include_directories(${PROJECT_NAME}_1 PRIVATE ${INC})
target_include_directories(${PROJECT_NAME}_2 PRIVATE ${INC})

target_include_directories(${PROJECT_NAME}_tests PRIVATE ${INC})
target_include_directories(${PROJECT_NAME}_benchmark PRIVATE ${INC})

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
