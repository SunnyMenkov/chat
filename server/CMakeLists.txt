cmake_minimum_required(VERSION 3.26)
project(server)
set(CMAKE_CXX_STANDARD 23)

add_executable(server
        main.cpp
        server.cpp
        globals.cpp

        server.h
        globals.h

)

target_link_libraries(server -lws2_32)


