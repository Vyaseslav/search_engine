cmake_minimum_required(VERSION 3.25)
project(test_search_engine)

set(CMAKE_CXX_STANDARD 17)

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(nlohmann_json)

add_executable(test_search_engine main.cpp include/ConventerJSON.h include/InvertedIndex.h include/SearchServer.h)

target_link_libraries(test_search_engine PRIVATE nlohmann_json::nlohmann_json)