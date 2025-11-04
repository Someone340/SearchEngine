cmake_minimum_required(VERSION 3.10.0)

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
  cmake_policy(SET CMP0135 NEW)
endif()

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(INC ${CMAKE_SOURCE_DIR}/include)
set(SRC ${CMAKE_SOURCE_DIR}/src)

add_executable(
  hello_test
  tests.cc ${INC}/invertedIndex.h ${SRC}/invertedIndex.cpp
  ${INC}/searchServer.h ${SRC}/searchServer.cpp
)
target_link_libraries(
  hello_test
  GTest::gtest_main
)
target_include_directories(hello_test PUBLIC ${INC} ${SRC})

include(GoogleTest)
gtest_discover_tests(hello_test)