cmake_minimum_required(VERSION 3.14)
project(mesh-analysis-tool)

set(CMAKE_CXX_STANDARD 17)

find_package(Manifold REQUIRED)
find_package(GTest REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(obj_analysis STATIC 
  src/obj_analysis.cpp
)

target_include_directories(obj_analysis PUBLIC include)
target_link_libraries(obj_analysis PUBLIC Manifold::Manifold)

add_executable(test_obj_analysis 
  test/test_obj_analysis.cpp
)

target_link_libraries(test_obj_analysis
  obj_analysis
  GTest::GTest
  GTest::Main
)

add_executable(example_analysis 
  examples/example_usage.cpp
)

target_link_libraries(example_analysis
  obj_analysis
)

enable_testing()
add_test(NAME test_obj_analysis COMMAND test_obj_analysis) 