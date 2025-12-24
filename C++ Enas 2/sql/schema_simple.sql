-- ================================================================================
-- NCTU Student Information System - Simplified Schema
-- Run this to create the database from scratch
-- ================================================================================

DROP DATABASE IF EXISTS nctu_sis;

CREATE DATABASE nctu_sis
CHARACTER SET utf8mb4
COLLATE utf8mb4_unicode_ci;

USE nctu_sis;

-- =============================================================
-- ROLES & PERMISSIONS
-- =============================================================
CREATE TABLE roles (
    id VARCHAR(30) PRIMARY KEY,
    name_en VARCHAR(100) NOT NULL,
    name_ar VARCHAR(100),
    description TEXT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE permissions (
    id VARCHAR(30) PRIMARY KEY,
    name VARCHAR(100) NOT NULL UNIQUE,
    description TEXT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE role_permissions (
    role_id VARCHAR(30) NOT NULL,
    permission_id VARCHAR(30) NOT NULL,
    PRIMARY KEY (role_id, permission_id),
    FOREIGN KEY (role_id) REFERENCES roles(id) ON DELETE CASCADE,
    FOREIGN KEY (permission_id) REFERENCES permissions(id) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =============================================================
-- LOOKUP DATA
-- =============================================================
CREATE TABLE colleges (
    id VARCHAR(10) PRIMARY KEY,
    name_ar VARCHAR(200) NOT NULL,
    name_en VARCHAR(200) NOT NULL,
    is_active BOOLEAN DEFAULT TRUE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE departments (
    id VARCHAR(10) PRIMARY KEY,
    college_id VARCHAR(10) NOT NULL,
    name_ar VARCHAR(200) NOT NULL,
    name_en VARCHAR(200) NOT NULL,
    code VARCHAR(10) NOT NULL,
    FOREIGN KEY (college_id) REFERENCES colleges(id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE academic_levels (
    id VARCHAR(10) PRIMARY KEY,
    name_ar VARCHAR(100) NOT NULL,
    name_en VARCHAR(100) NOT NULL,
    year_number INT NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE semesters (
    id VARCHAR(20) PRIMARY KEY,
    name_ar VARCHAR(100),
    name_en VARCHAR(100),
    academic_year VARCHAR(20),
    semester_number INT,
    is_current BOOLEAN DEFAULT FALSE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE course_types (
    id VARCHAR(20) PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    max_marks INT NOT NULL,
    assignment1_max INT DEFAULT 0,
    assignment2_max INT DEFAULT 0,
    year_work_max INT DEFAULT 0,
    final_exam_max INT DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =============================================================
-- USERS
-- =============================================================
CREATE TABLE users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    full_name VARCHAR(150) NOT NULL,
    email VARCHAR(200),
    phone VARCHAR(20),
    role_id VARCHAR(30) NOT NULL,
    is_active BOOLEAN DEFAULT TRUE,
    last_login TIMESTAMP NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (role_id) REFERENCES roles(id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE user_permissions (
    user_id INT NOT NULL,
    permission_id VARCHAR(30) NOT NULL,
    is_granted BOOLEAN NOT NULL,
    PRIMARY KEY (user_id, permission_id),
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
    FOREIGN KEY (permission_id) REFERENCES permissions(id) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =============================================================
-- COURSES
-- =============================================================
CREATE TABLE courses (
    id VARCHAR(30) PRIMARY KEY,
    code VARCHAR(20) NOT NULL,
    name_ar VARCHAR(200),
    name_en VARCHAR(200) NOT NULL,
    description TEXT,
    department_id VARCHAR(10) NOT NULL,
    academic_level_id VARCHAR(10) NOT NULL,
    semester_number INT NOT NULL,
    course_type_id VARCHAR(20) NOT NULL,
    credit_hours INT NOT NULL,
    lecture_hours INT DEFAULT 0,
    tutorial_hours INT DEFAULT 0,
    lab_hours INT DEFAULT 0,
    practical_hours INT DEFAULT 0,
    max_marks INT NOT NULL DEFAULT 100,
    assignment1_marks INT DEFAULT 20,
    assignment2_marks INT DEFAULT 20,
    year_work_marks INT DEFAULT 20,
    final_exam_marks INT DEFAULT 40,
    FOREIGN KEY (department_id) REFERENCES departments(id),
    FOREIGN KEY (academic_level_id) REFERENCES academic_levels(id),
    FOREIGN KEY (course_type_id) REFERENCES course_types(id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =============================================================
-- STUDENTS
-- =============================================================
CREATE TABLE students (
    id VARCHAR(20) PRIMARY KEY,
    user_id INT,
    first_name VARCHAR(100) NOT NULL,
    last_name VARCHAR(100) NOT NULL,
    date_of_birth DATE,
    email VARCHAR(200),
    phone VARCHAR(20),
    gender ENUM('male', 'female') NOT NULL,
    department_id VARCHAR(10) NOT NULL,
    academic_level_id VARCHAR(10) NOT NULL,
    enrollment_date DATE DEFAULT (CURRENT_DATE),
    status ENUM('active', 'inactive', 'graduated') DEFAULT 'active',
    FOREIGN KEY (user_id) REFERENCES users(id),
    FOREIGN KEY (department_id) REFERENCES departments(id),
    FOREIGN KEY (academic_level_id) REFERENCES academic_levels(id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =============================================================
-- PROFESSORS
-- =============================================================
CREATE TABLE professors (
    id VARCHAR(20) PRIMARY KEY,
    user_id INT,
    title VARCHAR(20),
    first_name VARCHAR(100) NOT NULL,
    last_name VARCHAR(100) NOT NULL,
    email VARCHAR(200),
    phone VARCHAR(20),
    department_id VARCHAR(10) NOT NULL,
    hire_date DATE,
    specialization VARCHAR(120),
    FOREIGN KEY (user_id) REFERENCES users(id),
    FOREIGN KEY (department_id) REFERENCES departments(id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =============================================================
-- PROFESSOR-COURSE ASSIGNMENTS
-- =============================================================
CREATE TABLE professor_courses (
    id INT AUTO_INCREMENT PRIMARY KEY,
    professor_id VARCHAR(20) NOT NULL,
    course_id VARCHAR(30) NOT NULL,
    semester_id VARCHAR(20) NOT NULL,
    is_primary BOOLEAN DEFAULT TRUE,
    UNIQUE KEY unique_assignment (professor_id, course_id, semester_id),
    FOREIGN KEY (professor_id) REFERENCES professors(id) ON DELETE CASCADE,
    FOREIGN KEY (course_id) REFERENCES courses(id) ON DELETE CASCADE,
    FOREIGN KEY (semester_id) REFERENCES semesters(id) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =============================================================
-- REGISTRATIONS
-- =============================================================
CREATE TABLE registrations (
    id INT AUTO_INCREMENT PRIMARY KEY,
    student_id VARCHAR(20) NOT NULL,
    course_id VARCHAR(30) NOT NULL,
    semester_id VARCHAR(20) NOT NULL,
    status ENUM('registered', 'dropped', 'completed') DEFAULT 'registered',
    registered_by INT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE,
    FOREIGN KEY (course_id) REFERENCES courses(id) ON DELETE CASCADE,
    FOREIGN KEY (semester_id) REFERENCES semesters(id) ON DELETE CASCADE,
    FOREIGN KEY (registered_by) REFERENCES users(id) ON DELETE SET NULL,
    UNIQUE KEY unique_reg (student_id, course_id, semester_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =============================================================
-- GRADES (Direct mapping - student_id, course_id, semester_id)
-- =============================================================
CREATE TABLE grades (
    id INT AUTO_INCREMENT PRIMARY KEY,
    student_id VARCHAR(20) NOT NULL,
    course_id VARCHAR(30) NOT NULL,
    semester_id VARCHAR(20) NOT NULL,
    assignment1_marks DECIMAL(5,2) DEFAULT 0,
    assignment2_marks DECIMAL(5,2) DEFAULT 0,
    year_work_marks DECIMAL(5,2) DEFAULT 0,
    midterm_marks DECIMAL(5,2) DEFAULT 0,
    final_exam_marks DECIMAL(5,2) DEFAULT 0,
    total_marks DECIMAL(5,2) DEFAULT 0,
    percentage DECIMAL(5,2) DEFAULT 0,
    grade_points DECIMAL(3,2) DEFAULT 0,
    letter_grade VARCHAR(2) DEFAULT 'F',
    evaluation VARCHAR(20) DEFAULT 'Fail',
    updated_by INT,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE,
    FOREIGN KEY (course_id) REFERENCES courses(id) ON DELETE CASCADE,
    FOREIGN KEY (semester_id) REFERENCES semesters(id) ON DELETE CASCADE,
    FOREIGN KEY (updated_by) REFERENCES users(id) ON DELETE SET NULL,
    UNIQUE KEY unique_grade (student_id, course_id, semester_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- Trigger to auto-calculate grades
DELIMITER //
CREATE TRIGGER before_insert_grades
BEFORE INSERT ON grades
FOR EACH ROW
BEGIN
    DECLARE max_marks INT DEFAULT 100;
    
    SELECT COALESCE(c.max_marks, 100) INTO max_marks 
    FROM courses c WHERE c.id = NEW.course_id;
    
    SET NEW.total_marks = NEW.assignment1_marks + NEW.assignment2_marks + 
                          NEW.year_work_marks + NEW.midterm_marks + NEW.final_exam_marks;
    SET NEW.percentage = (NEW.total_marks / max_marks) * 100;
    
    -- Calculate letter grade
    SET NEW.letter_grade = CASE
        WHEN NEW.percentage >= 90 THEN 'A+'
        WHEN NEW.percentage >= 85 THEN 'A'
        WHEN NEW.percentage >= 80 THEN 'A-'
        WHEN NEW.percentage >= 75 THEN 'B+'
        WHEN NEW.percentage >= 70 THEN 'B'
        WHEN NEW.percentage >= 65 THEN 'B-'
        WHEN NEW.percentage >= 60 THEN 'C+'
        WHEN NEW.percentage >= 55 THEN 'C'
        WHEN NEW.percentage >= 50 THEN 'C-'
        WHEN NEW.percentage >= 45 THEN 'D+'
        WHEN NEW.percentage >= 40 THEN 'D'
        ELSE 'F'
    END;
    
    -- Calculate evaluation
    SET NEW.evaluation = CASE
        WHEN NEW.percentage >= 90 THEN 'Excellent'
        WHEN NEW.percentage >= 80 THEN 'Very Good'
        WHEN NEW.percentage >= 70 THEN 'Good'
        WHEN NEW.percentage >= 60 THEN 'Fair'
        WHEN NEW.percentage >= 50 THEN 'Pass'
        WHEN NEW.percentage >= 40 THEN 'Weak'
        ELSE 'Fail'
    END;
    
    -- Calculate grade points
    SET NEW.grade_points = CASE
        WHEN NEW.percentage >= 90 THEN 4.0
        WHEN NEW.percentage >= 85 THEN 3.7
        WHEN NEW.percentage >= 80 THEN 3.3
        WHEN NEW.percentage >= 75 THEN 3.0
        WHEN NEW.percentage >= 70 THEN 2.7
        WHEN NEW.percentage >= 65 THEN 2.3
        WHEN NEW.percentage >= 60 THEN 2.0
        WHEN NEW.percentage >= 55 THEN 1.7
        WHEN NEW.percentage >= 50 THEN 1.3
        WHEN NEW.percentage >= 45 THEN 1.0
        WHEN NEW.percentage >= 40 THEN 0.7
        ELSE 0.0
    END;
END//

CREATE TRIGGER before_update_grades
BEFORE UPDATE ON grades
FOR EACH ROW
BEGIN
    DECLARE max_marks INT DEFAULT 100;
    
    SELECT COALESCE(c.max_marks, 100) INTO max_marks 
    FROM courses c WHERE c.id = NEW.course_id;
    
    SET NEW.total_marks = NEW.assignment1_marks + NEW.assignment2_marks + 
                          NEW.year_work_marks + NEW.midterm_marks + NEW.final_exam_marks;
    SET NEW.percentage = (NEW.total_marks / max_marks) * 100;
    
    SET NEW.letter_grade = CASE
        WHEN NEW.percentage >= 90 THEN 'A+'
        WHEN NEW.percentage >= 85 THEN 'A'
        WHEN NEW.percentage >= 80 THEN 'A-'
        WHEN NEW.percentage >= 75 THEN 'B+'
        WHEN NEW.percentage >= 70 THEN 'B'
        WHEN NEW.percentage >= 65 THEN 'B-'
        WHEN NEW.percentage >= 60 THEN 'C+'
        WHEN NEW.percentage >= 55 THEN 'C'
        WHEN NEW.percentage >= 50 THEN 'C-'
        WHEN NEW.percentage >= 45 THEN 'D+'
        WHEN NEW.percentage >= 40 THEN 'D'
        ELSE 'F'
    END;
    
    SET NEW.evaluation = CASE
        WHEN NEW.percentage >= 90 THEN 'Excellent'
        WHEN NEW.percentage >= 80 THEN 'Very Good'
        WHEN NEW.percentage >= 70 THEN 'Good'
        WHEN NEW.percentage >= 60 THEN 'Fair'
        WHEN NEW.percentage >= 50 THEN 'Pass'
        WHEN NEW.percentage >= 40 THEN 'Weak'
        ELSE 'Fail'
    END;
    
    SET NEW.grade_points = CASE
        WHEN NEW.percentage >= 90 THEN 4.0
        WHEN NEW.percentage >= 85 THEN 3.7
        WHEN NEW.percentage >= 80 THEN 3.3
        WHEN NEW.percentage >= 75 THEN 3.0
        WHEN NEW.percentage >= 70 THEN 2.7
        WHEN NEW.percentage >= 65 THEN 2.3
        WHEN NEW.percentage >= 60 THEN 2.0
        WHEN NEW.percentage >= 55 THEN 1.7
        WHEN NEW.percentage >= 50 THEN 1.3
        WHEN NEW.percentage >= 45 THEN 1.0
        WHEN NEW.percentage >= 40 THEN 0.7
        ELSE 0.0
    END;
END//
DELIMITER ;

-- =============================================================
-- ATTENDANCE
-- =============================================================
CREATE TABLE attendance (
    id INT AUTO_INCREMENT PRIMARY KEY,
    student_id VARCHAR(20) NOT NULL,
    course_id VARCHAR(30) NOT NULL,
    semester_id VARCHAR(20) NOT NULL,
    date DATE NOT NULL,
    status ENUM('present', 'absent', 'late', 'excused') NOT NULL DEFAULT 'present',
    notes TEXT,
    recorded_by INT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE,
    FOREIGN KEY (course_id) REFERENCES courses(id) ON DELETE CASCADE,
    FOREIGN KEY (semester_id) REFERENCES semesters(id) ON DELETE CASCADE,
    FOREIGN KEY (recorded_by) REFERENCES users(id) ON DELETE SET NULL,
    UNIQUE KEY unique_attendance (student_id, course_id, date)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =============================================================
-- DEFAULT DATA
-- =============================================================

-- Roles
INSERT INTO roles (id, name_en, name_ar, description) VALUES
('ROLE_SUPERADMIN', 'Super Admin', 'مسؤول النظام', 'Full system access'),
('ROLE_STUDENT_AFFAIRS', 'Student Affairs', 'شؤون الطلاب', 'Manage students & registrations'),
('ROLE_PROFESSOR', 'Professor', 'عضو هيئة تدريس', 'Enter grades & attendance'),
('ROLE_STUDENT', 'Student', 'طالب', 'View own data');

-- Permissions
INSERT INTO permissions (id, name, description) VALUES
('PERM_USER_MANAGE', 'user.manage', 'Manage users'),
('PERM_STU_VIEW', 'student.view', 'View students'),
('PERM_STU_EDIT', 'student.edit', 'Edit students'),
('PERM_CRS_VIEW', 'course.view', 'View courses'),
('PERM_GRD_VIEW_ALL', 'grade.view.all', 'View all grades'),
('PERM_GRD_VIEW_OWN', 'grade.view.own', 'View own grades'),
('PERM_GRD_ENTER', 'grade.enter', 'Enter grades'),
('PERM_ATT_VIEW_ALL', 'attendance.view.all', 'View all attendance'),
('PERM_ATT_VIEW_OWN', 'attendance.view.own', 'View own attendance'),
('PERM_ATT_RECORD', 'attendance.record', 'Record attendance');

-- Role permissions
INSERT INTO role_permissions (role_id, permission_id)
SELECT 'ROLE_SUPERADMIN', id FROM permissions;

INSERT INTO role_permissions (role_id, permission_id) VALUES
('ROLE_STUDENT_AFFAIRS', 'PERM_STU_VIEW'),
('ROLE_STUDENT_AFFAIRS', 'PERM_STU_EDIT'),
('ROLE_STUDENT_AFFAIRS', 'PERM_CRS_VIEW'),
('ROLE_STUDENT_AFFAIRS', 'PERM_GRD_VIEW_ALL'),
('ROLE_STUDENT_AFFAIRS', 'PERM_ATT_VIEW_ALL'),
('ROLE_PROFESSOR', 'PERM_CRS_VIEW'),
('ROLE_PROFESSOR', 'PERM_GRD_ENTER'),
('ROLE_PROFESSOR', 'PERM_ATT_RECORD'),
('ROLE_STUDENT', 'PERM_GRD_VIEW_OWN'),
('ROLE_STUDENT', 'PERM_ATT_VIEW_OWN');

-- Course types
INSERT INTO course_types (id, name, max_marks, assignment1_max, assignment2_max, year_work_max, final_exam_max) VALUES
('THEO_50', 'Theoretical 50', 50, 10, 10, 10, 20),
('THEO_100', 'Theoretical 100', 100, 20, 20, 20, 40),
('THEO_150', 'Theoretical 150', 150, 30, 45, 30, 45),
('PRAC_100', 'Practical 100', 100, 20, 20, 20, 40),
('PRAC_150', 'Practical 150', 150, 30, 45, 30, 45),
('PRAC_200', 'Practical 200', 200, 40, 60, 40, 60),
('PRAC_250', 'Practical 250', 250, 50, 75, 50, 75),
('PROJECT_150', 'Project 150', 150, 30, 45, 30, 45);

-- College and departments
INSERT INTO colleges (id, name_ar, name_en) VALUES
('COL001', 'كلية تكنولوجيا الصناعة والطاقة', 'College of Industry and Energy Technology');

INSERT INTO departments (id, college_id, name_ar, name_en, code) VALUES
('DEPT001', 'COL001', 'قسم الطاقة الجديدة والمتجددة', 'New and Renewable Energy', 'REN'),
('DEPT002', 'COL001', 'قسم الميكاترونيكس', 'Mechatronics', 'MEC'),
('DEPT003', 'COL001', 'قسم تكنولوجيا المعلومات', 'Information Technology', 'IT'),
('DEPT004', 'COL001', 'قسم الأوتوترونيكس', 'Autotronics', 'AUT'),
('DEPT005', 'COL001', 'قسم تكنولوجيا البترول', 'Petroleum Technology', 'PET');

-- Academic levels
INSERT INTO academic_levels (id, name_ar, name_en, year_number) VALUES
('LEVEL1', 'الفرقة الأولى', 'First Year', 1),
('LEVEL2', 'الفرقة الثانية', 'Second Year', 2),
('LEVEL3', 'الفرقة الثالثة', 'Third Year', 3),
('LEVEL4', 'الفرقة الرابعة', 'Fourth Year', 4);

-- Semesters
INSERT INTO semesters (id, name_ar, name_en, academic_year, semester_number, is_current) VALUES
('SEM-2024-FALL', 'الفصل الأول 2024-2025', 'Fall 2024', '2024-2025', 1, TRUE),
('SEM-2024-SPRING', 'الفصل الثاني 2024-2025', 'Spring 2025', '2024-2025', 2, FALSE),
('SEM-2025-FALL', 'الفصل الأول 2025-2026', 'Fall 2025', '2025-2026', 1, FALSE);

-- Default admin user
INSERT INTO users (username, password_hash, full_name, email, role_id) VALUES
('admin', 'admin123', 'System Administrator', 'admin@nctu.edu.eg', 'ROLE_SUPERADMIN'),
('affairs', 'affairs123', 'Student Affairs Office', 'affairs@nctu.edu.eg', 'ROLE_STUDENT_AFFAIRS');

-- Sample IT courses
INSERT INTO courses (id, code, name_en, department_id, academic_level_id, semester_number, course_type_id, credit_hours, max_marks) VALUES
('IT-Y1S1-01', 'IT111', 'Intro to Cyber Security', 'DEPT003', 'LEVEL1', 1, 'THEO_100', 1, 100),
('IT-Y1S1-02', 'IT112', 'IT Essentials', 'DEPT003', 'LEVEL1', 1, 'PRAC_100', 3, 100),
('IT-Y1S1-03', 'IT113', 'Technical English 1', 'DEPT003', 'LEVEL1', 1, 'THEO_100', 3, 100),
('IT-Y1S1-04', 'IT114', 'Mathematics 1', 'DEPT003', 'LEVEL1', 1, 'THEO_100', 3, 100),
('IT-Y1S1-05', 'IT115', 'Physics', 'DEPT003', 'LEVEL1', 1, 'THEO_100', 3, 100),
('IT-Y1S1-06', 'IT116', 'Programming in Python', 'DEPT003', 'LEVEL1', 1, 'PRAC_100', 3, 100);

-- Sample professor
INSERT INTO users (username, password_hash, full_name, email, role_id) VALUES
('PROF001', 'prof123', 'Dr. Mohamed Ibrahim', 'mohamed@nctu.edu.eg', 'ROLE_PROFESSOR');

INSERT INTO professors (id, user_id, title, first_name, last_name, email, department_id, specialization) VALUES
('PROF001', (SELECT id FROM users WHERE username='PROF001'), 'Dr.', 'Mohamed', 'Ibrahim', 'mohamed@nctu.edu.eg', 'DEPT003', 'Cybersecurity');

-- Assign professor to courses
INSERT INTO professor_courses (professor_id, course_id, semester_id) VALUES
('PROF001', 'IT-Y1S1-01', 'SEM-2024-FALL'),
('PROF001', 'IT-Y1S1-06', 'SEM-2024-FALL');

-- Sample students
INSERT INTO users (username, password_hash, full_name, email, role_id) VALUES
('2024001', 'student123', 'Ahmed Mohamed Ali', 'ahmed@student.nctu.edu.eg', 'ROLE_STUDENT'),
('2024002', 'student123', 'Sara Ibrahim Hassan', 'sara@student.nctu.edu.eg', 'ROLE_STUDENT'),
('2024003', 'student123', 'Mahmoud Khaled Omar', 'mahmoud@student.nctu.edu.eg', 'ROLE_STUDENT');

INSERT INTO students (id, user_id, first_name, last_name, email, gender, department_id, academic_level_id) VALUES
('2024001', (SELECT id FROM users WHERE username='2024001'), 'Ahmed', 'Ali', 'ahmed@student.nctu.edu.eg', 'male', 'DEPT003', 'LEVEL1'),
('2024002', (SELECT id FROM users WHERE username='2024002'), 'Sara', 'Hassan', 'sara@student.nctu.edu.eg', 'female', 'DEPT003', 'LEVEL1'),
('2024003', (SELECT id FROM users WHERE username='2024003'), 'Mahmoud', 'Omar', 'mahmoud@student.nctu.edu.eg', 'male', 'DEPT003', 'LEVEL1');

-- Register students to courses
INSERT INTO registrations (student_id, course_id, semester_id, registered_by) VALUES
('2024001', 'IT-Y1S1-01', 'SEM-2024-FALL', 1),
('2024001', 'IT-Y1S1-02', 'SEM-2024-FALL', 1),
('2024001', 'IT-Y1S1-03', 'SEM-2024-FALL', 1),
('2024001', 'IT-Y1S1-04', 'SEM-2024-FALL', 1),
('2024001', 'IT-Y1S1-05', 'SEM-2024-FALL', 1),
('2024001', 'IT-Y1S1-06', 'SEM-2024-FALL', 1),
('2024002', 'IT-Y1S1-01', 'SEM-2024-FALL', 1),
('2024002', 'IT-Y1S1-02', 'SEM-2024-FALL', 1),
('2024002', 'IT-Y1S1-03', 'SEM-2024-FALL', 1),
('2024003', 'IT-Y1S1-01', 'SEM-2024-FALL', 1),
('2024003', 'IT-Y1S1-02', 'SEM-2024-FALL', 1);

-- Sample grades
INSERT INTO grades (student_id, course_id, semester_id, assignment1_marks, assignment2_marks, year_work_marks, midterm_marks, final_exam_marks, updated_by) VALUES
('2024001', 'IT-Y1S1-01', 'SEM-2024-FALL', 18, 17, 18, 0, 35, 1),
('2024001', 'IT-Y1S1-02', 'SEM-2024-FALL', 19, 18, 19, 0, 38, 1),
('2024002', 'IT-Y1S1-01', 'SEM-2024-FALL', 16, 15, 16, 0, 32, 1);

SELECT 'Database created successfully!' AS status;
SELECT CONCAT('Users: ', COUNT(*)) AS count FROM users;
SELECT CONCAT('Students: ', COUNT(*)) AS count FROM students;
SELECT CONCAT('Professors: ', COUNT(*)) AS count FROM professors;
SELECT CONCAT('Courses: ', COUNT(*)) AS count FROM courses;
SELECT CONCAT('Registrations: ', COUNT(*)) AS count FROM registrations;
SELECT CONCAT('Grades: ', COUNT(*)) AS count FROM grades;


