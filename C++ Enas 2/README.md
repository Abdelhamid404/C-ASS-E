# NCTU Student Information System

A complete desktop Student Information System for New Cairo Technological University, built with C++ and WebView2.

## Features

- **Four User Roles**: Student, Professor, Student Affairs, Super Admin
- **Grade Management**: Automatic calculation of GPA, percentage, and letter grades
- **Attendance Tracking**: Record and view attendance for course sessions
- **Course Management**: View and manage courses, registrations
- **Student Management**: CRUD operations for student records
- **Modern UI**: Light-themed Bootstrap 5 interface with WebView2

## Technology Stack

- **Backend**: C++17/20 (MSVC)
- **Database**: MySQL 8.x with MySQL Connector/C++
- **UI Framework**: Microsoft WebView2 (HTML5/CSS3/JavaScript)
- **Frontend**: Bootstrap 5, Manrope/Fraunces fonts
- **JSON**: nlohmann/json

## Quick Start

1. **Install Dependencies** (see BUILD_INSTRUCTIONS.md)
   - MySQL Connector/C++ via vcpkg
   - WebView2 SDK
   - nlohmann/json.hpp

2. **Setup Database**
   ```bash
   mysql -u root -padmin < sql/schema_v3.sql
   mysql -u root -padmin < sql/sample_data.sql
   ```

3. **Build Project**
   - Open in Visual Studio 2026
   - Configure include/library paths
   - Build and run

4. **Test Login**
   - Student: `2024001` / `student123`
   - Professor: `PROF001` / `prof123`
   - Student Affairs: `affairs` / `affairs123`
   - Super Admin: `superadmin` / `admin123`

## Project Structure

- `src/` - C++ backend source files
- `web/` - Frontend HTML/JS/CSS files
- `sql/` - Database schema and sample data
- `external/` - Third-party libraries (json.hpp)

## Documentation

See `BUILD_INSTRUCTIONS.md` for detailed setup and build instructions.

## License

This is a university project for educational purposes.


