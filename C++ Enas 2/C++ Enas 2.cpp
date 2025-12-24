/*
 * ============================================
 * NCTU Student Information System
 * Main Entry Point (Windows Application)
 * ============================================
 */

// Fix Winsock header conflicts - must include winsock2.h before windows.h
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include "src/Application.h"

// For debug console output
#ifdef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#else
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#endif

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    // Unused parameters
    (void)hInstance;
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;

#ifdef _DEBUG
    // Allocate console for debug output
    AllocConsole();
    FILE* pFile;
    freopen_s(&pFile, "CONOUT$", "w", stdout);
    freopen_s(&pFile, "CONOUT$", "w", stderr);
#endif

    std::cout << "========================================" << std::endl;
    std::cout << "  NCTU Student Information System" << std::endl;
    std::cout << "  New Cairo Technological University" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    Application app;
    
    if (!app.initialize()) {
        std::cerr << "[Main] Failed to initialize application!" << std::endl;
        MessageBoxW(NULL, L"Failed to initialize application.\nCheck if MySQL is running and database exists.", 
                   L"NCTU SIS - Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    app.run();

#ifdef _DEBUG
    FreeConsole();
#endif

    return 0;
}

// Alternative entry point for console mode testing
#ifdef _DEBUG
int main() {
    return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOW);
}
#endif
