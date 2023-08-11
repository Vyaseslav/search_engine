cmake_minimum_required(VERSION 3.2)
project(search_engine)

set(CMAKE_CXX_STANDARD 17)

set(JSON_BuildTests OFF CACHE INTERNAL "")

include_directories("nlohmann_json/include")

add_executable(test_search_engine src/main.cpp include/ConverterJSON.h include/InvertedIndex.h include/SearchServer.h src/test/Gtest.cpp)

# 'Google_test' is the subproject name
project(Google_tests)

# 'lib' is the folder with Google Test sources
add_subdirectory(Google_tests/lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# 'Google_Tests_run' is the target name
# 'test1.cpp tests2.cpp' are source files with tests
add_executable(Google_Tests_run src/main.cpp )
target_link_libraries(Google_Tests_run gtest Google_tests)

set(gtest_disable_pthreads on)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

enable_testing()

target_link_libraries(test_search_engine PRIVATE gtest_main)
include(GoogleTest)
gtest_discover_tests(test_search_engine)
