/*
 * ============================================
 * APPLICATION.CPP - WebView2 Application
 * ============================================
 */

// Fix Winsock header conflicts - must include winsock2.h before Windows.h
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include "Application.h"
#include <iostream>
#include <filesystem>
#include <cctype>
#include "json.hpp"

using json = nlohmann::json;

using namespace std;

Application::Application() : webView(nullptr), messageHandler(nullptr), webViewInitialized(false) {
}

Application::~Application() {
    if (messageHandler) {
        delete messageHandler;
        messageHandler = nullptr;
    }
    if (webView) {
        webView->terminate();
        delete webView;
        webView = nullptr;
    }
}

string Application::urlEncodePath(const string& path) {
    string out;
    out.reserve(path.size());
    for (unsigned char c : path) {
        switch (c) {
            case ' ': out += "%20"; break;
            case '#': out += "%23"; break;
            case '%': out += "%25"; break;
            default: out += static_cast<char>(c); break;
        }
    }
    return out;
}

string Application::toFileUrl(const string& path) {
    filesystem::path fsPath = filesystem::absolute(path);
    string p = fsPath.generic_string();

    // Windows drive letter needs a leading slash in file URLs: /C:/...
    if (p.size() >= 2 && isalpha(static_cast<unsigned char>(p[0])) && p[1] == ':') {
        p = "/" + p;
    }

    return "file://" + urlEncodePath(p);
}

string Application::getWebPath() {
    // Get the executable directory
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    filesystem::path exeDir = filesystem::path(exePath).parent_path();
    
    // Look for web folder relative to exe
    filesystem::path webPath = exeDir / ".." / ".." / ".." / "C++ Enas 2" / "web";
    
    if (!filesystem::exists(webPath)) {
        // Try relative to project dir
        webPath = exeDir / ".." / ".." / "C++ Enas 2" / "web";
    }
    
    if (!filesystem::exists(webPath)) {
        // Try one level up
        webPath = exeDir / "web";
    }
    
    if (!filesystem::exists(webPath)) {
        // Hardcoded fallback
        webPath = "C:/Users/NITRO/source/repos/C++ Enas 2/C++ Enas 2/web";
    }
    
    return filesystem::absolute(webPath).string();
}

bool Application::initialize() {
    cout << "[App] Initializing application..." << endl;
    
    // Connect to database
    if (!db.connect()) {
        cerr << "[App] Failed to connect to database!" << endl;
        return false;
    }
    cout << "[App] Database connected." << endl;
    
    // Create message handler
    messageHandler = new MessageHandler();
    cout << "[App] Message handler created." << endl;
    
    // Create webview (debug=true for dev tools)
    try {
        webView = new webview::webview(true, nullptr);
        if (!webView) {
            cerr << "[App] Failed to create WebView!" << endl;
            return false;
        }
        
        webView->set_title("NCTU - Student Information System");
        webView->set_size(1280, 800, WEBVIEW_HINT_NONE);
        webViewInitialized = true;
        cout << "[App] WebView window created (1280x800)." << endl;
    } catch (const exception& e) {
        cerr << "[App] Exception creating WebView: " << e.what() << endl;
        return false;
    }
    
    // Bind JavaScript functions
    bindFunctions();
    
    // Navigate to the index page
    string webPath = getWebPath();
    string indexPath = webPath + "/index.html";
    string fileUrl = toFileUrl(indexPath);
    
    cout << "[App] Loading: " << fileUrl << endl;
    webView->navigate(fileUrl);
    
    return true;
}

void Application::bindFunctions() {
    // Store pointer to message handler for lambdas
    MessageHandler* handler = messageHandler;
    
    // Main message handler - routes all requests through MessageHandler
    webView->bind("cpp_sendMessage", [handler](const string& request) -> string {
        try {
            // webview sends params as JSON array string: ["{...}"]
            json args = json::parse(request);
            
            if (args.is_array() && args.size() > 0) {
                // First element is the JSON string of the actual message
                if (args[0].is_string()) {
                    // Parse the inner JSON string
                    return handler->handleMessage(args[0].get<string>());
                } else if (args[0].is_object()) {
                    // Already parsed object
                    return handler->handleMessage(args[0].dump());
                }
            } else if (args.is_object()) {
                // Direct object (fallback)
                return handler->handleMessage(args.dump());
            }
            
            // If request is already a JSON string, use it directly
            return handler->handleMessage(request);
        } catch (const exception& e) {
            cerr << "[App] Error in cpp_sendMessage: " << e.what() << endl;
            json errorResponse;
            errorResponse["success"] = false;
            errorResponse["error"] = string("Parse error: ") + e.what();
            return errorResponse.dump();
        }
    });
    
    // Login function
    webView->bind("cpp_login", [handler](const string& request) -> string {
        try {
            // Parse JSON array: ["username", "password"]
            json args = json::parse(request);
            if (!args.is_array() || args.size() < 2) {
                return "{\"success\":false,\"error\":\"Invalid arguments\"}";
            }
            
            string username = args[0].get<string>();
            string password = args[1].get<string>();
            
            cout << "[App] Login attempt: " << username << endl;
            
            // Create login request JSON
            json loginRequest;
            loginRequest["action"] = "login";
            loginRequest["username"] = username;
            loginRequest["password"] = password;
            
            return handler->handleMessage(loginRequest.dump());
        } catch (const exception& e) {
            cerr << "[App] Login error: " << e.what() << endl;
            return "{\"success\":false,\"error\":\"" + string(e.what()) + "\"}";
        }
    });
    
    // Logout function
    webView->bind("cpp_logout", [handler](const string& request) -> string {
        return handler->handleMessage("{\"action\":\"logout\"}");
    });
    
    // Get current user
    webView->bind("cpp_getCurrentUser", [handler](const string& request) -> string {
        return handler->handleMessage("{\"action\":\"getCurrentUser\"}");
    });
    
    // Get students
    webView->bind("cpp_getStudents", [handler](const string& request) -> string {
        return handler->handleMessage("{\"action\":\"getStudents\"}");
    });
    
    // Get courses
    webView->bind("cpp_getCourses", [handler](const string& request) -> string {
        return handler->handleMessage("{\"action\":\"getCourses\"}");
    });
    
    // Get student dashboard
    webView->bind("cpp_getStudentDashboard", [handler](const string& request) -> string {
        try {
            // Parse studentId from request if provided
            string json = "{\"action\":\"getStudentDashboard\"";
            if (request.find("studentId") != string::npos) {
                json = "{\"action\":\"getStudentDashboard\"," + request.substr(1);
            } else {
                json += "}";
            }
            return handler->handleMessage(json);
        } catch (...) {
            return handler->handleMessage("{\"action\":\"getStudentDashboard\"}");
        }
    });
    
    // Get student grades
    webView->bind("cpp_getStudentGrades", [handler](const string& request) -> string {
        return handler->handleMessage("{\"action\":\"getStudentGrades\"}");
    });
    
    // Get professor courses
    webView->bind("cpp_getProfessorCourses", [handler](const string& request) -> string {
        return handler->handleMessage("{\"action\":\"getProfessorCourses\"}");
    });
    
    cout << "[App] JavaScript functions bound." << endl;
}

void Application::run() {
    if (!webViewInitialized || !webView) {
        cerr << "[App] Cannot run - WebView not initialized!" << endl;
        return;
    }
    
    cout << "[App] Starting main loop..." << endl;
    try {
        webView->run();
    } catch (const exception& e) {
        cerr << "[App] Exception in main loop: " << e.what() << endl;
    }
    cout << "[App] Application terminated." << endl;
}

