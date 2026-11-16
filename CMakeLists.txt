cmake_minimum_required(VERSION 3.20)
project(QuantumBogoSort VERSION 0.1.0 LANGUAGES CXX)

add_library(qbs INTERFACE)
target_include_directories(qbs INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(qbs INTERFACE cxx_std_20)

add_executable(qbs_tests tests/test.cpp)
target_link_libraries(qbs_tests PRIVATE qbs)

add_executable(qbs_benchmark benchmarks/benchmark.cpp)
target_link_libraries(qbs_benchmark PRIVATE qbs)

enable_testing()
add_test(NAME qbs_tests COMMAND qbs_tests)
