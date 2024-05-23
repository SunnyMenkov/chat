// Dear ImGui: standalone example application for DirectX 11

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp
#include "server.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <fstream>
#include "globals.h"
#include <windows.h>
#include "server.h"

INFO users_info[1000];
all_logins data_inf[1000];

// Data
static ID3D11Device*            g_pd3dDevice = nullptr;
static ID3D11DeviceContext*     g_pd3dDeviceContext = nullptr;
static IDXGISwapChain*          g_pSwapChain = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView*  g_mainRenderTargetView = nullptr;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Main code
int maingui()
{
    SetConsoleOutputCP(CP_UTF8);

    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"ImGui Chat", WS_OVERLAPPEDWINDOW, 100, 100, 1200, 1200, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls




    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);
    ImFont* font2 = io.Fonts->AddFontFromFileTTF("regular.otf", 18.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    IM_ASSERT(font2 != NULL);


    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;

    std::string isLoginSuccess="";
    std::string isLoginSuccess2="";

    std::ifstream prev_out("history.txt", std::ifstream::app);
    std::string line;
    std::string prev_history="";

    while(getline(prev_out, line))
    {
        //std::cout << "line:" << line << std::endl;
        if (line!="\r\n"  && line.length()>0)
            prev_history +='\n'+ line;
    }


    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }


        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();


        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);


//        static bool _scrollToBottom = false;
//        static ImGuiTextBuffer _textBuffer;
//        static int _number = 0;
//
//        ImGui::Begin("Test control", nullptr, ImGuiWindowFlags_NoSavedSettings);
//        if (ImGui::Button("Add line")) {
//            _textBuffer.appendf("Number: %d\n", ++_number);
//            _scrollToBottom = true;
//        }
//        ImGui::End();
//
//        ImGui::Begin("Test", nullptr, ImGuiWindowFlags_NoSavedSettings);
//        ImGui::TextUnformatted(_textBuffer.begin());
//        ImGui::SetScrollHereY(1.0f);
//        if (_scrollToBottom) {
//            ImGui::SetScrollHereY(1.0f);
//            _scrollToBottom = false;
//        }
//        ImGui::End();
        static bool _scrollToBottom = false;


        {

            static float f = 0.0f;
            static int counter = 0;


            ImGui::PushFont(font2);
            ImGui::Begin("Chat History");                          // Create a window called "Hello, world!" and append into it.
            std::ifstream prev_out("history.txt", std::ifstream::app);
            std::string line;
            string new_history;

            while(getline(prev_out, line))
            {
                //std::cout << "line:" << line << std::endl;
                if (line!="\r\n"  && line.length()>0)
                    new_history +='\n'+ line;
            }
            if (new_history!= prev_history) {_scrollToBottom = true; prev_history=new_history;}

            ImGui::Text(new_history.c_str());
            if (_scrollToBottom) {
                ImGui::SetScrollHereY(1.0f);
                _scrollToBottom = false;
            }


            ImGui::End();
            ImGui::PopFont();

        }


        {
            static float f = 0.0f;
            ImGui::PushFont(font2);
            ImGui::Begin("Users", nullptr,ImGuiWindowFlags_NoResize);                          // Create a window called "Hello, world!" and append into it.
            ImGui::TextColored(ImVec4(0.0f, 0.5f, 0.0, 1.0f),
                               "Choose user to send private message:");           // Display some text (you can use a format strings too)
            static bool selection[100] = {false, false, false, false, false};

            for (int i = 0; i < cnt_users_inf; i++) {
                string selectable = to_string(master.fd_array[i]) + " " + users_info[i].login;

                if (i==0) selectable+=" server";
                else if (GUIsock==0) GUIsock=master.fd_array[i];
                else if (GUIsock2==0 && master.fd_array[i]!=GUIsock) GUIsock2=master.fd_array[i];

     //cout << GUIsock2 << " guiscokets "  << GUIsock<<endl;
                ImGui::Selectable(selectable.c_str(), &selection[i], ImGuiSelectableFlags_AllowDoubleClick);
                if (selectable[i] == 1) {
//                    _isMainHistory = false;
//                    index = i;
                }

            }

            ImGui::End();
            ImGui::PopFont();
        }

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;
            ImGui::PushFont(font2);
            ImGui::Begin("User1");
            if (ImGui::TreeNode("Password Input")) {
                static char password[64] = "";
                static char login[64] = "";

                ImGui::InputTextWithHint("login", "<login>", login, IM_ARRAYSIZE(login));
                ImGui::InputTextWithHint("password", "<password>", password, IM_ARRAYSIZE(password),
                                         ImGuiInputTextFlags_Password);

                if (ImGui::Button("Login") && string(login).length() > 0 && string(password).length() > 0) {
                    isLoginSuccess = user_login("", std::string(login), std::string(password), master.fd_array[1]);


                    if (isLoginSuccess == "Успешная авторизация.\r\n") {
                        users_info[1].user_socket = to_string(master.fd_array[1]);
                        users_info[1].flag_user_name = 1;
                        users_info[1].login = std::string(login);
                        users_info[1].password = std::string(password);
                    }

                    //  GUIsock = sock;
                    for (int i = 0; i < master.fd_count+6; i++) {
                        cout << users_info[i].login << " ? " << users_info[i].user_socket << endl;
                    }
                }
                ImGui::Text(isLoginSuccess.c_str());
                ImGui::TreePop();
            }

            static char text[1024] = "";
               std::string nullthetext = "";
             strcpy(text, nullthetext.c_str());

            ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CtrlEnterForNewLine;
            ImGui::Text("Chat");
            if (ImGui::InputText("##send", text, IM_ARRAYSIZE(text), ImGuiInputTextFlags_EnterReturnsTrue)) {
                users_info[1].user_socket = to_string(master.fd_array[1]);
                send_message(master, listening, master.fd_array[1], text);
                _scrollToBottom = true;
            }
            ImGui::End();
            ImGui::PopFont();
        }

        {
            static float f = 0.0f;
            ImGui::PushFont(font2);
            ImGui::Begin("User2");
            if (ImGui::TreeNode("Password Input"))
            {
                static char password[64] = "";
                static char login[64] = "";

                ImGui::InputTextWithHint("login", "<login>",  login, IM_ARRAYSIZE(login));
                ImGui::InputTextWithHint("password", "<password>", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);

                if (ImGui::Button("Login") && string(login).length()>0 && string(password).length()>0){
                    isLoginSuccess2 = user_login("", std::string(login), std::string(password),master.fd_array[2]);


                    if (isLoginSuccess2 == "Успешная авторизация.\r\n") {
                        users_info[2].user_socket = to_string(master.fd_array[2]);
                        users_info[2].flag_user_name = 1;
                        users_info[2].login = std::string(login);
                        users_info[2].password = std::string(password);
                    }
                    //  GUIsock2 = sock;
                }
                ImGui::Text(isLoginSuccess2.c_str());
                ImGui::TreePop();

            }
            static char text2[1024] = "";
            std::string nullthetext = "";

            strcpy(text2, nullthetext.c_str());




            ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CtrlEnterForNewLine;
            ImGui::Text("Chat");
            if (ImGui::InputText("##send", text2, IM_ARRAYSIZE(text2), ImGuiInputTextFlags_EnterReturnsTrue)  && (string(text2).length()>0)) {
                users_info[2].user_socket = to_string(master.fd_array[2]);
                send_message(master, listening, master.fd_array[2], text2);
                _scrollToBottom = true;
            }
            ImGui::End();
            ImGui::PopFont();
        }

        // Rendering
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0); // Present with vsync
        //g_pSwapChain->Present(0, 0); // Present without vsync
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
        case WM_SIZE:
            if (wParam == SIZE_MINIMIZED)
                return 0;
            g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
            g_ResizeHeight = (UINT)HIWORD(lParam);
            return 0;
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                return 0;
            break;
        case WM_DESTROY:
            ::PostQuitMessage(0);
            return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}