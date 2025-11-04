cmake_minimum_required(VERSION 3.10)
project(search_engine VERSION 0.1.0 LANGUAGES C CXX)

set(JSON_BuildTests OFF CACHE INTERNAL "")

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
  cmake_policy(SET CMP0135 NEW)
endif()

add_subdirectory(nlohmann_json)
add_subdirectory(src)
file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_subdirectory(testing)

enable_testing()
