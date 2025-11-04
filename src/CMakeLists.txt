cmake_minimum_required(VERSION 3.10.0)

set(INC ${CMAKE_SOURCE_DIR}/include)
add_executable(search_engine main.cpp ${INC}/converterJSON.h converterJSON.cpp 
    exceptions.cpp ${INC}/exceptions.h invertedIndex.cpp ${INC}/invertedIndex.h
    searchServer.cpp ${INC}/searchServer.h)
target_include_directories(search_engine PUBLIC ${INC})

file(COPY config.json requests.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)