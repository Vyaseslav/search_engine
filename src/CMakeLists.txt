cmake_minimum_required (VERSION 3.8)

set(Src
	"ConverterJSON.cpp"
	"SearchServer.cpp"
	"InvertedIndex.cpp"
	"main.cpp"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
add_executable(test_search_engine ${Src})

target_link_libraries(test_search_engine nlohmann_json::nlohmann_json)
target_include_directories(test_search_engine PUBLIC ${MY_INCLUDE_DIR})