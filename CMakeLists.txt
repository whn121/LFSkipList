cmake_minimum_required(VERSION 3.14)
project(LFSkipList)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(skiplist src/main.cpp)
target_link_libraries(skiplist pthread)

add_executable(test_skiplist test/test_skiplist.cpp)
target_link_libraries(test_skiplist pthread)
