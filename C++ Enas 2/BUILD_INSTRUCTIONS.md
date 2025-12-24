# NCTU Student Information System - Build Instructions

## Prerequisites
1. **MySQL Server 9.5** installed at: `C:\Program Files\MySQL\MySQL Server 9.5`
2. **Visual Studio 2022** or later

## Step 1: Create the Database

Open MySQL Workbench or command line and run:

```sql
SOURCE C:/Users/NITRO/source/repos/C++ Enas 2/C++ Enas 2/sql/schema_simple.sql;
```

Or copy and paste the contents of `sql/schema_simple.sql` into MySQL Workbench and execute.

## Step 2: Build the Project

1. Open `C++ Enas 2.sln` in Visual Studio
2. Make sure configuration is **x64 Debug**
3. Build the solution (Ctrl+Shift+B)

## Step 3: Run the Application

1. Press F5 or run without debugging (Ctrl+F5)
2. The application will test the database connection

## Default Login Credentials

| Role | Username | Password |
|------|----------|----------|
| Super Admin | admin | admin123 |
| Student Affairs | affairs | affairs123 |
| Professor | PROF001 | prof123 |
| Student | 2024001 | student123 |
| Student | 2024002 | student123 |
| Student | 2024003 | student123 |

## Database Configuration

If your MySQL settings are different, edit `src/Database.h`:

```cpp
host("localhost"),
user("root"),
password("admin"),  // Change this to your MySQL password
database("nctu_sis"),
port(3306)
```

## Troubleshooting

### Error: "libmysql.dll not found"
Copy `libmysql.dll` from `C:\Program Files\MySQL\MySQL Server 9.5\lib` to the output folder:
`C:\Users\NITRO\source\repos\C++ Enas 2\x64\Debug\`

### Error: "Cannot open mysql.h"
Make sure the include path is correct in project settings:
`C:\Program Files\MySQL\MySQL Server 9.5\include`

### Error: "Connection failed"
1. Make sure MySQL Server is running
2. Check username/password in Database.h
3. Make sure the database `nctu_sis` exists

## Project Structure

```
C++ Enas 2/
├── C++ Enas 2.cpp          # Main entry point
├── src/
│   ├── Database.h          # MySQL database connection
│   ├── AuthService.*       # Login/authentication
│   ├── GradeService.*      # Grade calculations
│   ├── MessageHandler.*    # JSON message handling
│   ├── json.hpp            # JSON library
│   └── repositories/       # Data access layer
│       ├── StudentRepository.*
│       ├── ProfessorRepository.*
│       ├── CourseRepository.*
│       ├── RegistrationRepository.*
│       ├── GradeRepository.*
│       └── AttendanceRepository.*
├── sql/
│   ├── schema_simple.sql   # Database schema (USE THIS)
│   ├── schema_v3.sql       # Full schema with all courses
│   └── sample_data.sql     # Additional sample data
└── web/                    # Frontend files (for WebView2)
```
