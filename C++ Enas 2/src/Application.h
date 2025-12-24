/*
 * ============================================
 * APPLICATION.H - WebView2 Application Window
 * ============================================
 * Creates a native window with embedded WebView2
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <functional>
#include "../lib/webview.h"
#include "Database.h"
#include "MessageHandler.h"

using namespace std;

class Application {
private:
    webview::webview* webView;
    Database db;
    MessageHandler* messageHandler;
    bool webViewInitialized;
    
    // Convert path to file:// URL
    static string toFileUrl(const string& path);
    
    // URL encode a path
    static string urlEncodePath(const string& path);

public:
    Application();
    ~Application();
    
    // Initialize the application
    bool initialize();
    
    // Run the main message loop
    void run();
    
    // Bind C++ functions to JavaScript
    void bindFunctions();
    
    // Get the web folder path
    string getWebPath();
};

#endif // APPLICATION_H


