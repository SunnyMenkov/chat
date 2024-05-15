cmake_minimum_required(VERSION 3.27)
project(chat)
set(CMAKE_CXX_STANDARD 23)

add_executable(chat
        original.cpp
        server.cpp
        clientgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_widgets.cpp
        imgui/imgui.cpp

        imgui/backends/imgui_impl_win32.cpp
        imgui/backends/imgui_impl_dx11.cpp
        server.h
        clientgui.h
        original.h
        main.cpp
        globals.cpp


        #    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/*.cpp"
)
find_package(OpenGL REQUIRED)
target_include_directories(chat PUBLIC
        imgui/
        imgui/backends/

)


target_link_libraries(chat -lws2_32 d3d11 d3dcompiler dwmapi)


