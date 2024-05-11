cmake_minimum_required(VERSION 3.27)
project(untitled)

set(CMAKE_CXX_STANDARD 23)


ADD_LIBRARY(LibsModule
        main.cpp
)

target_link_libraries(LibsModule -lws2_32)

add_executable(untitled main.cpp)
target_link_libraries(untitled LibsModule)
