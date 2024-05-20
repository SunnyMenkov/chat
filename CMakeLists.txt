cmake_minimum_required(VERSION 3.26)
project(chat)
set(CMAKE_CXX_STANDARD 23)

add_executable(chat 
       
       
        imgui/imgui_demo.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_widgets.cpp
        imgui/imgui.cpp
        imgui/backends/imgui_impl_win32.cpp
        imgui/backends/imgui_impl_dx11.cpp

        main.cpp
        globals.cpp
        server.cpp
        clientgui.cpp
        client.cpp

        globals.h
        server.h
        clientgui.h
        client.h


)
find_package(OpenGL REQUIRED)
target_include_directories(chat PUBLIC
        imgui/
        imgui/backends/

)


target_link_libraries(chat -lws2_32 d3d11 d3dcompiler dwmapi)


