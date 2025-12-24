# WebView2 GUI Setup Complete

## What Was Implemented

The application has been successfully converted from a console application to a native Windows application with a WebView2-based GUI.

### Changes Made

1. **Project Configuration** (`C++ Enas 2.vcxproj`)
   - Changed from Console to Windows Application
   - Updated preprocessor definitions (`_CONSOLE` → `_WINDOWS`)
   - Changed subsystem from Console to Windows
   - Added webview library include directory

2. **Application Class** (`src/Application.h` / `src/Application.cpp`)
   - Created Application class to manage WebView2 window
   - Handles window creation, WebView2 initialization, and message routing
   - Binds JavaScript functions to C++ backend handlers
   - Manages database connection and MessageHandler

3. **Main Entry Point** (`C++ Enas 2.cpp`)
   - Changed from `main()` to `WinMain()` for Windows application
   - Added console allocation for debug output
   - Initializes Application and runs main loop

4. **JavaScript Bridge** (`web/js/app.js`)
   - Updated to use webview bindings (`cpp_login`, `cpp_logout`, etc.)
   - Handles authentication and API calls
   - Provides fallback for browser testing

5. **WebView Library**
   - Copied webview library from working project
   - Located in `lib/webview/` directory

## How It Works

```
┌─────────────────┐
│   WinMain()     │
│  Entry Point    │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  Application    │
│  - Creates      │
│    WebView2     │
│  - Binds JS     │
│    functions    │
└────────┬────────┘
         │
         ▼
┌─────────────────┐      ┌──────────────────┐
│   WebView2      │◄────►│  HTML/JS UI      │
│   Window        │      │  (web/*.html)    │
└────────┬────────┘      └──────────────────┘
         │
         │ JavaScript calls (cpp_login, etc.)
         ▼
┌─────────────────┐
│ MessageHandler  │
│  - Routes       │
│    requests     │
│  - Calls        │
│    services     │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│   Database      │
│   (MySQL)       │
└─────────────────┘
```

## Building and Running

1. **Open the project** in Visual Studio 2022 or later
2. **Set configuration** to Debug x64 or Release x64
3. **Build** the solution (F7 or Build → Build Solution)
4. **Run** the application (F5 or Debug → Start Debugging)

The application will:
- Connect to MySQL database
- Create a native window (1280x800)
- Load `web/index.html` as the login page
- Display the web UI inside the native window

## Test Accounts

- **Super Admin**: admin / admin123
- **Student Affairs**: affairs / affairs123
- **Professor**: prof001 / prof123
- **Student**: student001 / student123

## Troubleshooting

### WebView2 Not Found
- Ensure Windows 10/11 is up to date
- WebView2 Runtime should be installed automatically
- If missing, download from: https://developer.microsoft.com/en-us/microsoft-edge/webview2/

### Database Connection Failed
- Ensure MySQL Server is running
- Verify database `nctu_sis` exists
- Check connection credentials in `src/Database.h`

### Web Files Not Found
- Ensure `web/` folder is in the project directory
- Check `Application::getWebPath()` for correct path resolution
- Web files should be copied to output directory (or use absolute paths)

### JavaScript Functions Not Working
- Open DevTools (F12) in the WebView window
- Check console for JavaScript errors
- Verify bindings are registered (check console output)

## File Structure

```
C++ Enas 2/
├── C++ Enas 2.cpp          # WinMain entry point
├── src/
│   ├── Application.h       # Application class header
│   ├── Application.cpp     # Application implementation
│   ├── Database.h/cpp      # Database connection
│   ├── MessageHandler.h/cpp # Message routing
│   └── ...
├── lib/
│   └── webview/
│       └── webview.h        # WebView2 library
└── web/
    ├── index.html          # Login page
    ├── dashboard.html      # Dashboard
    ├── grades.html         # Grades page
    ├── js/
    │   ├── app.js          # Main JavaScript bridge
    │   ├── auth.js         # Authentication
    │   └── ui.js           # UI helpers
    └── css/
        └── style.css       # Styles
```

## Next Steps

The application is now ready to use! You can:
- Test login with different user roles
- Navigate through the UI
- View student grades, courses, etc.
- Add more features as needed

