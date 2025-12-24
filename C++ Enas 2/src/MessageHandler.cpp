#include "MessageHandler.h"
#include <algorithm>

MessageHandler::MessageHandler() {
    db = new Database();
    db->connect();
    
    authService = new AuthService(db);
    studentRepo = new StudentRepository(db);
    professorRepo = new ProfessorRepository(db);
    courseRepo = new CourseRepository(db);
    registrationRepo = new RegistrationRepository(db);
    gradeRepo = new GradeRepository(db);
    attendanceRepo = new AttendanceRepository(db);
    gradeService = new GradeService(db, gradeRepo);
}

MessageHandler::~MessageHandler() {
    delete gradeService;
    delete attendanceRepo;
    delete gradeRepo;
    delete registrationRepo;
    delete courseRepo;
    delete professorRepo;
    delete studentRepo;
    delete authService;
    delete db;
}

string MessageHandler::handleMessage(const string& messageJson) {
    try {
        json request = json::parse(messageJson);
        string action = request.value("action", "");
        json response;

        if (action == "login") {
            response = handleLogin(request);
        } else if (action == "logout") {
            response = handleLogout(request);
        } else if (action == "getCurrentUser") {
            response = handleGetCurrentUser(request);
        } else if (action == "getStudentDashboard") {
            response = handleGetStudentDashboard(request);
        } else if (action == "getStudentGrades") {
            response = handleGetStudentGrades(request);
        } else if (action == "getStudentAttendance") {
            response = handleGetStudentAttendance(request);
        } else if (action == "getStudentCourses") {
            response = handleGetStudentCourses(request);
        } else if (action == "getProfessorCourses") {
            response = handleGetProfessorCourses(request);
        } else if (action == "getCourseStudents") {
            response = handleGetCourseStudents(request);
        } else if (action == "updateGrade") {
            response = handleUpdateGrade(request);
        } else if (action == "recordAttendance") {
            response = handleRecordAttendance(request);
        } else if (action == "getCourseAttendance") {
            response = handleGetCourseAttendance(request);
        } else if (action == "getCourseGrades") {
            response = handleGetCourseGrades(request);
        } else if (action == "getStudents") {
            response = handleGetStudents(request);
        } else if (action == "createStudent") {
            response = handleCreateStudent(request);
        } else if (action == "updateStudent") {
            response = handleUpdateStudent(request);
        } else if (action == "getCourses") {
            response = handleGetCourses(request);
        } else if (action == "createCourse") {
            response = handleCreateCourse(request);
        } else if (action == "updateCourse") {
            response = handleUpdateCourse(request);
        } else if (action == "deleteCourse") {
            response = handleDeleteCourse(request);
        } else if (action == "registerStudentToCourse") {
            response = handleRegisterStudentToCourse(request);
        } else if (action == "getRegistrations") {
            response = handleGetRegistrations(request);
        } else if (action == "updateRegistration") {
            response = handleUpdateRegistration(request);
        } else if (action == "deleteRegistration") {
            response = handleDeleteRegistration(request);
        } else if (action == "getProfessors") {
            response = handleGetProfessors(request);
        } else if (action == "createProfessor") {
            response = handleCreateProfessor(request);
        } else if (action == "updateProfessor") {
            response = handleUpdateProfessor(request);
        } else if (action == "deleteProfessor") {
            response = handleDeleteProfessor(request);
        } else if (action == "getFacilities") {
            response = handleGetFacilities(request);
        } else if (action == "createLectureHall") {
            response = handleCreateLectureHall(request);
        } else if (action == "updateLectureHall") {
            response = handleUpdateLectureHall(request);
        } else if (action == "deleteLectureHall") {
            response = handleDeleteLectureHall(request);
        } else if (action == "createLaboratory") {
            response = handleCreateLaboratory(request);
        } else if (action == "updateLaboratory") {
            response = handleUpdateLaboratory(request);
        } else if (action == "deleteLaboratory") {
            response = handleDeleteLaboratory(request);
        } else if (action == "deleteStudent") {
            response = handleDeleteStudent(request);
        } else if (action == "deleteGrade") {
            response = handleDeleteGrade(request);
        } else if (action == "updateAttendanceRecord") {
            response = handleUpdateAttendanceRecord(request);
        } else if (action == "deleteAttendance") {
            response = handleDeleteAttendance(request);
        } else if (action == "getCourseAssignments") {
            response = handleGetCourseAssignments(request);
        } else if (action == "createCourseAssignment") {
            response = handleCreateCourseAssignment(request);
        } else if (action == "deleteCourseAssignment") {
            response = handleDeleteCourseAssignment(request);
        } else if (action == "getDepartments") {
            response = handleGetDepartments(request);
        } else if (action == "createDepartment") {
            response = handleCreateDepartment(request);
        } else if (action == "updateDepartment") {
            response = handleUpdateDepartment(request);
        } else if (action == "deleteDepartment") {
            response = handleDeleteDepartment(request);
        } else if (action == "getColleges") {
            response = handleGetColleges(request);
        } else if (action == "createCollege") {
            response = handleCreateCollege(request);
        } else if (action == "updateCollege") {
            response = handleUpdateCollege(request);
        } else if (action == "deleteCollege") {
            response = handleDeleteCollege(request);
        } else if (action == "getAcademicLevels") {
            response = handleGetAcademicLevels(request);
        } else if (action == "createAcademicLevel") {
            response = handleCreateAcademicLevel(request);
        } else if (action == "updateAcademicLevel") {
            response = handleUpdateAcademicLevel(request);
        } else if (action == "deleteAcademicLevel") {
            response = handleDeleteAcademicLevel(request);
        } else if (action == "getSemesters") {
            response = handleGetSemesters(request);
        } else if (action == "createSemester") {
            response = handleCreateSemester(request);
        } else if (action == "updateSemester") {
            response = handleUpdateSemester(request);
        } else if (action == "deleteSemester") {
            response = handleDeleteSemester(request);
        } else if (action == "getCourseTypes") {
            response = handleGetCourseTypes(request);
        } else if (action == "createCourseType") {
            response = handleCreateCourseType(request);
        } else if (action == "updateCourseType") {
            response = handleUpdateCourseType(request);
        } else if (action == "deleteCourseType") {
            response = handleDeleteCourseType(request);
        } else {
            response = createErrorResponse("Unknown action: " + action);
        }

        return response.dump();
    } catch (const json::parse_error& e) {
        json errorResponse = createErrorResponse(string("Parse error: ") + e.what());
        return errorResponse.dump();
    } catch (const exception& e) {
        json errorResponse = createErrorResponse(string("Request error: ") + e.what());
        return errorResponse.dump();
    }
}

json MessageHandler::handleLogin(const json& request) {
    string username = request.value("username", "");
    string password = request.value("password", "");

    if (username.empty() || password.empty()) {
        return createErrorResponse("Username and password are required");
    }

    if (authService->login(username, password, currentUser)) {
        json userData;
        userData["userId"] = currentUser.userId;
        userData["username"] = currentUser.username;
        userData["fullName"] = currentUser.fullName;
        userData["roleId"] = currentUser.roleId;
        userData["studentId"] = currentUser.studentId;
        userData["professorId"] = currentUser.professorId;
        return createSuccessResponse(userData);
    }

    return createErrorResponse("Invalid username or password");
}

json MessageHandler::handleLogout(const json& request) {
    currentUser = UserInfo();
    return createSuccessResponse();
}

json MessageHandler::handleGetCurrentUser(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }

    json userData;
    userData["userId"] = currentUser.userId;
    userData["username"] = currentUser.username;
    userData["fullName"] = currentUser.fullName;
    userData["roleId"] = currentUser.roleId;
    userData["studentId"] = currentUser.studentId;
    userData["professorId"] = currentUser.professorId;
    return createSuccessResponse(userData);
}

json MessageHandler::handleGetStudentDashboard(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isStudent() && !isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string studentId = resolveStudentId(request);
    if (studentId.empty()) {
        return createErrorResponse("Student ID is required");
    }

    Student student;
    if (!studentRepo->getStudentById(studentId, student)) {
        return createErrorResponse("Student not found");
    }

    string semesterId = request.value("semesterId", "");
    if (semesterId.empty()) {
        semesterId = getCurrentSemesterId();
    }

    auto grades = gradeRepo->getStudentGrades(studentId, semesterId);
    auto registrations = registrationRepo->getStudentRegistrations(studentId, semesterId);
    double gpa = semesterId.empty() ? 0.0 : gradeService->calculateGPA(studentId, semesterId);
    double cgpa = gradeService->calculateCGPA(studentId);
    double attendanceRate = semesterId.empty() ? 0.0 : calculateAttendanceRate(studentId, semesterId);

    json data;
    data["student"] = {
        {"id", student.id},
        {"firstName", student.firstName},
        {"lastName", student.lastName},
        {"fullName", student.fullName},
        {"email", student.email},
        {"departmentId", student.departmentId},
        {"levelId", student.academicLevelId}
    };
    
    data["gpa"] = gpa;
    data["cgpa"] = cgpa;
    data["totalCourses"] = registrations.size();
    data["registeredCourses"] = registrations.size();
    data["attendanceRate"] = attendanceRate;
    data["currentSemesterId"] = semesterId;
    
    json gradesArray = json::array();
    for (auto& g : grades) {
        gradesArray.push_back({
            {"courseId", g.courseId},
            {"courseCode", g.courseCode},
            {"courseName", g.courseName},
            {"totalMarks", g.totalMarks},
            {"percentage", g.percentage},
            {"letterGrade", g.letterGrade},
            {"evaluation", g.evaluation},
            {"gpa", g.gradePoints}
        });
    }
    data["grades"] = gradesArray;

    vector<Grade> recentGrades = grades;
    std::sort(recentGrades.begin(), recentGrades.end(),
              [](const Grade& a, const Grade& b) { return a.updatedAt > b.updatedAt; });
    json recentGradesArray = json::array();
    size_t maxRecent = 5;
    for (size_t i = 0; i < recentGrades.size() && i < maxRecent; ++i) {
        auto& g = recentGrades[i];
        recentGradesArray.push_back({
            {"courseId", g.courseId},
            {"courseCode", g.courseCode},
            {"courseName", g.courseName},
            {"totalMarks", g.totalMarks},
            {"percentage", g.percentage},
            {"letterGrade", g.letterGrade},
            {"evaluation", g.evaluation},
            {"gpa", g.gradePoints}
        });
    }
    data["recentGrades"] = recentGradesArray;

    return createSuccessResponse(data);
}

json MessageHandler::handleGetStudentGrades(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isStudent() && !isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string studentId = resolveStudentId(request);
    if (studentId.empty()) {
        return createErrorResponse("Student ID is required");
    }
    string semesterId = request.value("semesterId", "");
    
    auto grades = gradeRepo->getStudentGrades(studentId, semesterId);
    
    json gradesArray = json::array();
    for (auto& g : grades) {
        gradesArray.push_back({
            {"id", g.id},
            {"studentId", g.studentId},
            {"courseId", g.courseId},
            {"courseCode", g.courseCode},
            {"courseName", g.courseName},
            {"semesterId", g.semesterId},
            {"assignment1", g.assignment1},
            {"assignment2", g.assignment2},
            {"yearWork", g.yearWork},
            {"midterm", g.midterm},
            {"finalExam", g.finalExam},
            {"totalMarks", g.totalMarks},
            {"percentage", g.percentage},
            {"letterGrade", g.letterGrade},
            {"evaluation", g.evaluation},
            {"gradePoints", g.gradePoints},
            {"gpa", g.gradePoints}
        });
    }

    json responseData;
    responseData["grades"] = gradesArray;
    return createSuccessResponse(responseData);
}

json MessageHandler::handleGetStudentAttendance(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isStudent() && !isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string studentId = resolveStudentId(request);
    if (studentId.empty()) {
        return createErrorResponse("Student ID is required");
    }
    string courseId = request.value("courseId", "");
    string semesterId = request.value("semesterId", "");
    
    auto attendance = attendanceRepo->getStudentAttendance(studentId, courseId, semesterId);
    
    json attendanceArray = json::array();
    for (auto& a : attendance) {
        attendanceArray.push_back({
            {"id", a.id},
            {"studentId", a.studentId},
            {"courseId", a.courseId},
            {"courseCode", a.courseCode},
            {"courseName", a.courseName},
            {"date", a.date},
            {"sessionDate", a.date},
            {"status", a.status},
            {"notes", a.notes},
            {"remarks", a.notes}
        });
    }

    json responseData;
    responseData["attendance"] = attendanceArray;
    return createSuccessResponse(responseData);
}

json MessageHandler::handleGetStudentCourses(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isStudent() && !isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string studentId = resolveStudentId(request);
    if (studentId.empty()) {
        return createErrorResponse("Student ID is required");
    }
    string semesterId = request.value("semesterId", "");
    
    auto registrations = registrationRepo->getStudentRegistrations(studentId, semesterId);
    
    json coursesArray = json::array();
    for (auto& r : registrations) {
        coursesArray.push_back({
            {"registrationId", r.id},
            {"courseId", r.courseId},
            {"courseCode", r.courseCode},
            {"courseName", r.courseName},
            {"semesterId", r.semesterId},
            {"status", r.status}
        });
    }

    json responseData;
    responseData["courses"] = coursesArray;
    return createSuccessResponse(responseData);
}

json MessageHandler::handleGetProfessorCourses(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isProfessor() && !isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }
    if (!db || !db->isConnected()) {
        if (!db->connect()) {
            return createErrorResponse("Database connection failed");
        }
    }

    string professorId = request.value("professorId", currentUser.professorId);

    string assignmentTable = db->resolveTable({"course_assignments", "professor_courses"});
    string courseTable = db->resolveTable({"courses"});
    string semesterTable = db->resolveTable({"semesters"});

    if (!db->tableExists(assignmentTable) || !db->tableExists(courseTable) || !db->tableExists(semesterTable)) {
        json responseData;
        responseData["courses"] = json::array();
        return createSuccessResponse(responseData);
    }

    string assignmentCourseCol = db->resolveColumn(assignmentTable, {"course_id", "courseId"});
    string assignmentProfessorCol = db->resolveColumn(assignmentTable, {"professor_id", "professorId"});
    string assignmentSemesterCol = db->resolveColumn(assignmentTable, {"semester_id", "semesterId"});
    string courseIdCol = db->resolveColumn(courseTable, {"id", "course_id", "courseId"});
    string courseCodeCol = db->resolveColumn(courseTable, {"code", "course_code", "courseCode"});
    string courseNameCol = db->resolveColumn(courseTable, {"name_en", "nameEn", "name"});
    string courseNameArCol = db->resolveColumn(courseTable, {"name_ar", "nameAr"});
    string creditHoursCol = db->resolveColumn(courseTable, {"credit_hours", "creditHours"});
    string semesterIdCol = db->resolveColumn(semesterTable, {"id", "semester_id", "semesterId"});
    string semesterNameCol = db->resolveColumn(semesterTable, {"name_en", "nameEn", "name"});
    string academicYearCol = db->resolveColumn(semesterTable, {"academic_year", "academicYear"});
    string semesterNumberCol = db->resolveColumn(semesterTable, {"semester_number", "semesterNumber"});

    string query = "SELECT c." + courseIdCol + " as course_id, "
                  "c." + courseCodeCol + " as course_code, "
                  "c." + courseNameCol + " as course_name, "
                  "c." + courseNameArCol + " as course_name_ar, "
                  "c." + creditHoursCol + " as credit_hours, "
                  "s." + semesterIdCol + " as semester_id, "
                  "s." + semesterNameCol + " as semester_name "
                  "FROM " + assignmentTable + " pc "
                  "JOIN " + courseTable + " c ON pc." + assignmentCourseCol + " = c." + courseIdCol + " "
                  "JOIN " + semesterTable + " s ON pc." + assignmentSemesterCol + " = s." + semesterIdCol + " "
                  "WHERE pc." + assignmentProfessorCol + " = '" + db->escapeString(professorId) + "'";

    if (db->columnExists(semesterTable, academicYearCol) && db->columnExists(semesterTable, semesterNumberCol)) {
        query += " ORDER BY s." + academicYearCol + " DESC, s." + semesterNumberCol + " DESC";
        if (db->columnExists(courseTable, courseCodeCol)) {
            query += ", c." + courseCodeCol;
        }
    } else if (db->columnExists(courseTable, courseCodeCol)) {
        query += " ORDER BY c." + courseCodeCol;
    }

    auto results = db->executeQuery(query);

    json coursesArray = json::array();
    if (results) {
        while (results->next()) {
            coursesArray.push_back({
                {"courseId", results->getString("course_id")},
                {"courseCode", results->getString("course_code")},
                {"courseName", results->getString("course_name")},
                {"code", results->getString("course_code")},
                {"nameEn", results->getString("course_name")},
                {"nameAr", results->getString("course_name_ar")},
                {"creditHours", results->getInt("credit_hours")},
                {"semesterId", results->getString("semester_id")},
                {"semesterName", results->getString("semester_name")}
            });
        }
    }

    json responseData;
    responseData["courses"] = coursesArray;
    return createSuccessResponse(responseData);
}

json MessageHandler::handleGetCourseStudents(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isProfessor() && !isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string courseId = request.value("courseId", "");
    string semesterId = request.value("semesterId", "");

    if (courseId.empty()) {
        return createErrorResponse("Course ID is required");
    }
    if (semesterId.empty()) {
        semesterId = getCurrentSemesterId();
    }
    if (semesterId.empty()) {
        return createErrorResponse("Semester ID is required");
    }
    if (isProfessor() && !isProfessorAssignedToCourse(courseId, semesterId)) {
        return createErrorResponse("Not assigned to this course");
    }

    auto registrations = registrationRepo->getCourseRegistrations(courseId, semesterId);
    
    json studentsArray = json::array();
    for (auto& r : registrations) {
        // Get grade for this student-course
        Grade grade;
        bool hasGrade = gradeRepo->getGrade(r.studentId, courseId, semesterId, grade);
        
        json studentJson = {
            {"registrationId", r.id},
            {"studentId", r.studentId},
            {"studentName", r.studentName},
            {"registrationStatus", r.status},
            {"courseId", courseId},
            {"semesterId", semesterId}
        };
        
        studentJson["assignment1"] = hasGrade ? grade.assignment1 : 0.0;
        studentJson["assignment2"] = hasGrade ? grade.assignment2 : 0.0;
        studentJson["yearWork"] = hasGrade ? grade.yearWork : 0.0;
        studentJson["midterm"] = hasGrade ? grade.midterm : 0.0;
        studentJson["finalExam"] = hasGrade ? grade.finalExam : 0.0;
        studentJson["totalMarks"] = hasGrade ? grade.totalMarks : 0.0;
        studentJson["percentage"] = hasGrade ? grade.percentage : 0.0;
        studentJson["letterGrade"] = hasGrade ? grade.letterGrade : "N/A";
        
        studentsArray.push_back(studentJson);
    }

    json responseData;
    responseData["students"] = studentsArray;
    return createSuccessResponse(responseData);
}

json MessageHandler::handleUpdateGrade(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isProfessor() && !isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string studentId = request.value("studentId", "");
    string courseId = request.value("courseId", "");
    string semesterId = request.value("semesterId", "");
    int registrationId = request.value("registrationId", 0);

    if (registrationId > 0 && (studentId.empty() || courseId.empty() || semesterId.empty())) {
        Registration reg;
        if (!registrationRepo->getRegistrationById(registrationId, reg)) {
            return createErrorResponse("Registration not found");
        }
        studentId = reg.studentId;
        courseId = reg.courseId;
        semesterId = reg.semesterId;
    }

    if (studentId.empty() || courseId.empty() || semesterId.empty()) {
        return createErrorResponse("Student ID, Course ID, and Semester ID are required");
    }
    if (isProfessor() && !isProfessorAssignedToCourse(courseId, semesterId)) {
        return createErrorResponse("Not assigned to this course");
    }

    Grade grade;
    grade.studentId = studentId;
    grade.courseId = courseId;
    grade.semesterId = semesterId;
    grade.assignment1 = request.value("assignment1", 0.0);
    grade.assignment2 = request.value("assignment2", 0.0);
    grade.yearWork = request.value("yearWork", 0.0);
    grade.midterm = request.value("midterm", 0.0);
    grade.finalExam = request.value("finalExam", 0.0);

    Course course;
    int maxMarks = 100;
    if (courseRepo->getCourseById(courseId, course) && course.maxMarks > 0) {
        maxMarks = course.maxMarks;
    }

    GradeCalculation calc = gradeService->calculateGrade(
        grade.assignment1,
        grade.assignment2,
        grade.yearWork,
        grade.midterm,
        grade.finalExam,
        maxMarks);
    grade.totalMarks = calc.totalMarks;
    grade.percentage = calc.percentage;
    grade.gradePoints = calc.gradePoints;
    grade.letterGrade = calc.letterGrade;
    grade.evaluation = calc.evaluation;

    if (gradeRepo->createOrUpdateGrade(grade, currentUser.userId)) {
        return createSuccessResponse();
    }

    return createErrorResponse("Failed to update grade");
}

json MessageHandler::handleRecordAttendance(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isProfessor() && !isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string courseId = request.value("courseId", "");
    string semesterId = request.value("semesterId", "");
    string date = request.value("date", "");
    if (date.empty()) {
        date = request.value("sessionDate", "");
    }
    string sessionType = request.value("sessionType", "");

    if (courseId.empty() || date.empty()) {
        return createErrorResponse("Course ID and date are required");
    }

    if (semesterId.empty()) {
        semesterId = getCurrentSemesterId();
    }
    if (semesterId.empty()) {
        return createErrorResponse("Semester ID is required");
    }
    if (isProfessor() && !isProfessorAssignedToCourse(courseId, semesterId)) {
        return createErrorResponse("Not assigned to this course");
    }

    if (request.contains("students") && request["students"].is_array()) {
        ensureSessionRecord(courseId, semesterId, date, sessionType);
        int failures = 0;
        int recorded = 0;
        for (const auto& entry : request["students"]) {
            if (!entry.is_object()) continue;
            string studentId = entry.value("studentId", "");
            if (studentId.empty()) continue;
            string status = entry.value("status", "present");
            string notes = entry.value("notes", entry.value("remarks", ""));
            if (attendanceRepo->recordAttendance(studentId, courseId, semesterId, date, status, notes, currentUser.userId)) {
                recorded++;
            } else {
                failures++;
            }
        }
        if (failures == 0) {
            json data;
            data["recorded"] = recorded;
            return createSuccessResponse(data);
        }
        return createErrorResponse("Failed to record attendance for " + to_string(failures) + " students");
    }

    string studentId = request.value("studentId", "");
    string status = request.value("status", "present");
    string notes = request.value("notes", "");
    if (studentId.empty()) {
        return createErrorResponse("Student ID is required");
    }

    ensureSessionRecord(courseId, semesterId, date, sessionType);
    if (attendanceRepo->recordAttendance(studentId, courseId, semesterId, date, status, notes, currentUser.userId)) {
        return createSuccessResponse();
    }

    return createErrorResponse("Failed to record attendance");
}

json MessageHandler::handleGetCourseAttendance(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isProfessor() && !isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string courseId = request.value("courseId", "");
    string semesterId = request.value("semesterId", "");
    string date = request.value("date", "");

    if (courseId.empty()) {
        return createErrorResponse("Course ID is required");
    }
    if (semesterId.empty()) {
        semesterId = getCurrentSemesterId();
    }
    if (semesterId.empty()) {
        return createErrorResponse("Semester ID is required");
    }
    if (isProfessor() && !isProfessorAssignedToCourse(courseId, semesterId)) {
        return createErrorResponse("Not assigned to this course");
    }

    auto attendance = attendanceRepo->getCourseAttendance(courseId, semesterId, date);
    
    json attendanceArray = json::array();
    for (auto& a : attendance) {
        attendanceArray.push_back({
            {"id", a.id},
            {"studentId", a.studentId},
            {"studentName", a.studentName},
            {"date", a.date},
            {"status", a.status},
            {"notes", a.notes}
        });
    }

    json responseData;
    responseData["attendance"] = attendanceArray;
    return createSuccessResponse(responseData);
}

json MessageHandler::handleGetCourseGrades(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isProfessor() && !isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string courseId = request.value("courseId", "");
    string semesterId = request.value("semesterId", "");

    if (courseId.empty()) {
        return createErrorResponse("Course ID is required");
    }
    if (semesterId.empty()) {
        semesterId = getCurrentSemesterId();
    }
    if (semesterId.empty()) {
        return createErrorResponse("Semester ID is required");
    }
    if (isProfessor() && !isProfessorAssignedToCourse(courseId, semesterId)) {
        return createErrorResponse("Not assigned to this course");
    }

    auto grades = gradeRepo->getCourseGrades(courseId, semesterId);
    
    json gradesArray = json::array();
    for (auto& g : grades) {
        gradesArray.push_back({
            {"id", g.id},
            {"studentId", g.studentId},
            {"studentName", g.studentName},
            {"assignment1", g.assignment1},
            {"assignment2", g.assignment2},
            {"yearWork", g.yearWork},
            {"midterm", g.midterm},
            {"finalExam", g.finalExam},
            {"totalMarks", g.totalMarks},
            {"percentage", g.percentage},
            {"letterGrade", g.letterGrade},
            {"evaluation", g.evaluation},
            {"gpa", g.gradePoints}
        });
    }

    json responseData;
    responseData["grades"] = gradesArray;
    return createSuccessResponse(responseData);
}

json MessageHandler::handleGetStudents(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin() && !isStudentAffairs()) {
        return createErrorResponse("Access denied");
    }

    auto students = studentRepo->listAll();
    
    json studentsArray = json::array();
    for (auto& s : students) {
        studentsArray.push_back({
            {"id", s.id},
            {"firstName", s.firstName},
            {"lastName", s.lastName},
            {"fullName", s.fullName},
            {"email", s.email},
            {"phone", s.phone},
            {"gender", s.gender},
            {"dateOfBirth", s.dateOfBirth},
            {"enrollmentDate", s.enrollmentDate},
            {"departmentId", s.departmentId},
            {"levelId", s.academicLevelId},
            {"academicLevelId", s.academicLevelId},
            {"status", s.status}
        });
    }

    json responseData;
    responseData["students"] = studentsArray;
    return createSuccessResponse(responseData);
}

json MessageHandler::handleCreateStudent(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin() && !isStudentAffairs()) {
        return createErrorResponse("Access denied");
    }

    Student student;
    student.id = request.value("id", "");
    student.firstName = request.value("firstName", "");
    student.lastName = request.value("lastName", "");
    student.email = request.value("email", "");
    student.phone = request.value("phone", "");
    student.gender = request.value("gender", "");
    student.dateOfBirth = request.value("dateOfBirth", "");
    student.departmentId = request.value("departmentId", "");
    student.academicLevelId = request.value("levelId", "");

    if (student.id.empty() || student.firstName.empty() || student.lastName.empty()) {
        return createErrorResponse("ID, first name, and last name are required");
    }

    if (studentRepo->createStudent(student, currentUser.userId)) {
        return createSuccessResponse();
    }

    return createErrorResponse("Failed to create student");
}

json MessageHandler::handleUpdateStudent(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin() && !isStudentAffairs()) {
        return createErrorResponse("Access denied");
    }

    string studentId = request.value("id", "");
    
    Student student;
    if (!studentRepo->getStudentById(studentId, student)) {
        return createErrorResponse("Student not found");
    }

    // Update fields if provided
    if (request.contains("firstName")) student.firstName = request["firstName"];
    if (request.contains("lastName")) student.lastName = request["lastName"];
    if (request.contains("email")) student.email = request["email"];
    if (request.contains("phone")) student.phone = request["phone"];
    if (request.contains("gender")) student.gender = request["gender"];
    if (request.contains("dateOfBirth")) student.dateOfBirth = request["dateOfBirth"];
    if (request.contains("departmentId")) student.departmentId = request["departmentId"];
    if (request.contains("levelId")) student.academicLevelId = request["levelId"];
    if (request.contains("status")) student.status = request["status"];

    if (studentRepo->updateStudent(studentId, student, currentUser.userId)) {
        return createSuccessResponse();
    }

    return createErrorResponse("Failed to update student");
}

json MessageHandler::handleGetCourses(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string departmentId = request.value("departmentId", "");
    
    vector<Course> courses;
    if (departmentId.empty()) {
        courses = courseRepo->listAll();
    } else {
        courses = courseRepo->listByDepartment(departmentId);
    }
    
    json coursesArray = json::array();
    for (auto& c : courses) {
        coursesArray.push_back({
            {"id", c.id},
            {"code", c.code},
            {"nameEn", c.nameEn},
            {"nameAr", c.nameAr},
            {"description", c.description},
            {"courseTypeId", c.courseTypeId},
            {"creditHours", c.creditHours},
            {"lectureHours", c.lectureHours},
            {"tutorialHours", c.tutorialHours},
            {"labHours", c.labHours},
            {"practicalHours", c.practicalHours},
            {"departmentId", c.departmentId},
            {"levelId", c.academicLevelId},
            {"academicLevelId", c.academicLevelId},
            {"semesterNumber", c.semesterNumber},
            {"maxMarks", c.maxMarks},
            {"assignment1Marks", c.assignment1Marks},
            {"assignment2Marks", c.assignment2Marks},
            {"yearWorkMarks", c.yearWorkMarks},
            {"finalExamMarks", c.finalExamMarks}
        });
    }

    json responseData;
    responseData["courses"] = coursesArray;
    return createSuccessResponse(responseData);
}

json MessageHandler::handleRegisterStudentToCourse(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string studentId = request.value("studentId", "");
    string courseId = request.value("courseId", "");
    string semesterId = request.value("semesterId", "");

    if (studentId.empty() || courseId.empty() || semesterId.empty()) {
        return createErrorResponse("Student ID, Course ID, and Semester ID are required");
    }

    if (registrationRepo->registerStudent(studentId, courseId, semesterId, currentUser.userId)) {
        return createSuccessResponse();
    }

    return createErrorResponse("Failed to register student");
}

json MessageHandler::handleGetProfessors(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string departmentId = request.value("departmentId", "");
    vector<Professor> professors = departmentId.empty()
        ? professorRepo->listAll()
        : professorRepo->listByDepartment(departmentId);

    json professorsArray = json::array();
    for (auto& p : professors) {
        professorsArray.push_back({
            {"id", p.id},
            {"fullName", p.fullName},
            {"title", p.title},
            {"firstName", p.firstName},
            {"lastName", p.lastName},
            {"email", p.email},
            {"phone", p.phone},
            {"departmentId", p.departmentId},
            {"hireDate", p.hireDate},
            {"specialization", p.specialization}
        });
    }

    json responseData;
    responseData["professors"] = professorsArray;
    return createSuccessResponse(responseData);
}

json MessageHandler::handleGetFacilities(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }
    if (!db || !db->isConnected()) {
        if (!db->connect()) {
            return createErrorResponse("Database connection failed");
        }
    }

    json lectureHalls = json::array();
    string hallTable = db->resolveTable({"lecture_halls", "lecture_hall"});
    if (db->tableExists(hallTable)) {
        string idCol = db->resolveColumn(hallTable, {"id"});
        string nameCol = db->resolveColumn(hallTable, {"name"});
        string buildingCol = db->resolveColumn(hallTable, {"building"});
        string floorCol = db->resolveColumn(hallTable, {"floor"});
        string capacityCol = db->resolveColumn(hallTable, {"max_capacity", "maxCapacity"});
        string seatingCol = db->resolveColumn(hallTable, {"seating_desc", "seatingDesc"});
        string acCol = db->resolveColumn(hallTable, {"ac_units", "acUnits"});
        string fanCol = db->resolveColumn(hallTable, {"fan_units", "fanUnits"});
        string lightingCol = db->resolveColumn(hallTable, {"lighting_units", "lightingUnits"});
        string descCol = db->resolveColumn(hallTable, {"description"});

        bool hasBuilding = db->columnExists(hallTable, buildingCol);
        bool hasFloor = db->columnExists(hallTable, floorCol);
        bool hasSeating = db->columnExists(hallTable, seatingCol);
        bool hasAc = db->columnExists(hallTable, acCol);
        bool hasFan = db->columnExists(hallTable, fanCol);
        bool hasLighting = db->columnExists(hallTable, lightingCol);
        bool hasDesc = db->columnExists(hallTable, descCol);

        string hallQuery = "SELECT * FROM " + hallTable;
        if (hasBuilding && hasFloor && db->columnExists(hallTable, nameCol)) {
            hallQuery += " ORDER BY " + buildingCol + ", " + floorCol + ", " + nameCol;
        }
        auto hallResults = db->executeQuery(hallQuery);
        if (hallResults) {
            while (hallResults->next()) {
                lectureHalls.push_back({
                    {"id", hallResults->getString(idCol)},
                    {"name", hallResults->getString(nameCol)},
                    {"building", hasBuilding ? hallResults->getString(buildingCol) : ""},
                    {"floor", hasFloor ? json(hallResults->getInt(floorCol)) : json(nullptr)},
                    {"maxCapacity", hallResults->getInt(capacityCol)},
                    {"seatingDesc", hasSeating ? hallResults->getString(seatingCol) : ""},
                    {"acUnits", hasAc ? json(hallResults->getInt(acCol)) : json(nullptr)},
                    {"fanUnits", hasFan ? json(hallResults->getInt(fanCol)) : json(nullptr)},
                    {"lightingUnits", hasLighting ? json(hallResults->getInt(lightingCol)) : json(nullptr)},
                    {"description", hasDesc ? hallResults->getString(descCol) : ""}
                });
            }
        }
    }

    json laboratories = json::array();
    string labTable = db->resolveTable({"laboratories", "labs"});
    if (db->tableExists(labTable)) {
        string idCol = db->resolveColumn(labTable, {"id"});
        string nameCol = db->resolveColumn(labTable, {"name"});
        string buildingCol = db->resolveColumn(labTable, {"building"});
        string floorCol = db->resolveColumn(labTable, {"floor"});
        string capacityCol = db->resolveColumn(labTable, {"max_capacity", "maxCapacity"});
        string labTypeCol = db->resolveColumn(labTable, {"lab_type", "labType"});
        string computersCol = db->resolveColumn(labTable, {"computers_count", "computersCount"});
        string seatsCol = db->resolveColumn(labTable, {"seats_count", "seatsCount"});
        string acCol = db->resolveColumn(labTable, {"ac_units", "acUnits"});
        string fanCol = db->resolveColumn(labTable, {"fan_units", "fanUnits"});
        string lightingCol = db->resolveColumn(labTable, {"lighting_units", "lightingUnits"});
        string descCol = db->resolveColumn(labTable, {"description"});

        bool hasBuilding = db->columnExists(labTable, buildingCol);
        bool hasFloor = db->columnExists(labTable, floorCol);
        bool hasLabType = db->columnExists(labTable, labTypeCol);
        bool hasComputers = db->columnExists(labTable, computersCol);
        bool hasSeats = db->columnExists(labTable, seatsCol);
        bool hasAc = db->columnExists(labTable, acCol);
        bool hasFan = db->columnExists(labTable, fanCol);
        bool hasLighting = db->columnExists(labTable, lightingCol);
        bool hasDesc = db->columnExists(labTable, descCol);

        string labQuery = "SELECT * FROM " + labTable;
        if (hasBuilding && hasFloor && db->columnExists(labTable, nameCol)) {
            labQuery += " ORDER BY " + buildingCol + ", " + floorCol + ", " + nameCol;
        }
        auto labResults = db->executeQuery(labQuery);
        if (labResults) {
            while (labResults->next()) {
                laboratories.push_back({
                    {"id", labResults->getString(idCol)},
                    {"name", labResults->getString(nameCol)},
                    {"building", hasBuilding ? labResults->getString(buildingCol) : ""},
                    {"floor", hasFloor ? json(labResults->getInt(floorCol)) : json(nullptr)},
                    {"maxCapacity", labResults->getInt(capacityCol)},
                    {"labType", hasLabType ? labResults->getString(labTypeCol) : ""},
                    {"computersCount", hasComputers ? json(labResults->getInt(computersCol)) : json(nullptr)},
                    {"seatsCount", hasSeats ? json(labResults->getInt(seatsCol)) : json(nullptr)},
                    {"acUnits", hasAc ? json(labResults->getInt(acCol)) : json(nullptr)},
                    {"fanUnits", hasFan ? json(labResults->getInt(fanCol)) : json(nullptr)},
                    {"lightingUnits", hasLighting ? json(labResults->getInt(lightingCol)) : json(nullptr)},
                    {"description", hasDesc ? labResults->getString(descCol) : ""}
                });
            }
        }
    }

    json responseData;
    responseData["lectureHalls"] = lectureHalls;
    responseData["laboratories"] = laboratories;
    return createSuccessResponse(responseData);
}

string MessageHandler::getCurrentSemesterId() {
    if (!db || !db->isConnected()) {
        if (!db->connect()) {
            return "";
        }
    }

    string semesterTable = db->resolveTable({"semesters"});
    if (!db->tableExists(semesterTable)) {
        return "";
    }
    string semesterIdCol = db->resolveColumn(semesterTable, {"id", "semester_id", "semesterId"});
    string currentCol = db->resolveColumn(semesterTable, {"is_current", "isCurrent", "current"});
    string yearCol = db->resolveColumn(semesterTable, {"academic_year", "academicYear"});
    string numberCol = db->resolveColumn(semesterTable, {"semester_number", "semesterNumber"});

    string query = "SELECT " + semesterIdCol + " as semester_id FROM " + semesterTable + " ";
    if (db->columnExists(semesterTable, currentCol)) {
        query += "WHERE " + currentCol + " = TRUE ";
    }
    if (db->columnExists(semesterTable, yearCol) && db->columnExists(semesterTable, numberCol)) {
        query += "ORDER BY " + yearCol + " DESC, " + numberCol + " DESC ";
    }
    query += "LIMIT 1";
    auto results = db->executeQuery(query);
    if (results && results->next()) {
        return results->getString("semester_id");
    }

    string fallbackQuery = "SELECT " + semesterIdCol + " as semester_id FROM " + semesterTable + " ";
    if (db->columnExists(semesterTable, yearCol) && db->columnExists(semesterTable, numberCol)) {
        fallbackQuery += "ORDER BY " + yearCol + " DESC, " + numberCol + " DESC ";
    }
    fallbackQuery += "LIMIT 1";
    auto fallback = db->executeQuery(fallbackQuery);
    if (fallback && fallback->next()) {
        return fallback->getString("semester_id");
    }

    return "";
}

double MessageHandler::calculateAttendanceRate(const string& studentId, const string& semesterId) {
    if (studentId.empty() || semesterId.empty()) {
        return 0.0;
    }
    if (!db || !db->isConnected()) {
        if (!db->connect()) {
            return 0.0;
        }
    }

    string attendanceTable = db->resolveTable({"attendance"});
    if (!db->tableExists(attendanceTable)) {
        return 0.0;
    }
    string studentCol = db->resolveColumn(attendanceTable, {"student_id", "studentId"});
    string semesterCol = db->resolveColumn(attendanceTable, {"semester_id", "semesterId"});
    string statusCol = db->resolveColumn(attendanceTable, {"status"});

    string totalQuery = "SELECT COUNT(*) as cnt FROM " + attendanceTable + " "
                       "WHERE " + studentCol + " = '" + db->escapeString(studentId) + "' "
                       "AND " + semesterCol + " = '" + db->escapeString(semesterId) + "'";
    auto totalRes = db->executeQuery(totalQuery);
    int total = 0;
    if (totalRes && totalRes->next()) {
        total = totalRes->getInt("cnt");
    }
    if (total == 0) {
        return 0.0;
    }

    string attendedQuery = "SELECT COUNT(*) as cnt FROM " + attendanceTable + " "
                          "WHERE " + studentCol + " = '" + db->escapeString(studentId) + "' "
                          "AND " + semesterCol + " = '" + db->escapeString(semesterId) + "' "
                          "AND " + statusCol + " IN ('present','late','excused')";
    auto attendedRes = db->executeQuery(attendedQuery);
    int attended = 0;
    if (attendedRes && attendedRes->next()) {
        attended = attendedRes->getInt("cnt");
    }

    return (static_cast<double>(attended) / static_cast<double>(total)) * 100.0;
}

void MessageHandler::ensureSessionRecord(const string& courseId, const string& semesterId,
                                         const string& sessionDate, const string& sessionType) {
    if (courseId.empty() || semesterId.empty() || sessionDate.empty() || sessionType.empty()) {
        return;
    }
    if (!db || !db->isConnected()) {
        if (!db->connect()) {
            return;
        }
    }

    string sessionTable = db->resolveTable({"sessions"});
    if (!db->tableExists(sessionTable)) {
        return;
    }
    string sessionIdCol = db->resolveColumn(sessionTable, {"id"});
    string courseCol = db->resolveColumn(sessionTable, {"course_id", "courseId"});
    string professorCol = db->resolveColumn(sessionTable, {"professor_id", "professorId"});
    string semesterCol = db->resolveColumn(sessionTable, {"semester_id", "semesterId"});
    string dateCol = db->resolveColumn(sessionTable, {"session_date", "sessionDate", "date"});
    string typeCol = db->resolveColumn(sessionTable, {"session_type", "sessionType", "type"});

    if (courseCol.empty() || semesterCol.empty() || dateCol.empty() || typeCol.empty()) {
        return;
    }

    string checkQuery = "SELECT " + sessionIdCol + " FROM " + sessionTable + " WHERE " + courseCol + " = '" + db->escapeString(courseId) + "' "
                       "AND " + semesterCol + " = '" + db->escapeString(semesterId) + "' "
                       "AND " + dateCol + " = '" + db->escapeString(sessionDate) + "' "
                       "AND " + typeCol + " = '" + db->escapeString(sessionType) + "' ";
    if (!currentUser.professorId.empty()) {
        checkQuery += "AND " + professorCol + " = '" + db->escapeString(currentUser.professorId) + "'";
    } else {
        checkQuery += "AND " + professorCol + " IS NULL";
    }

    auto checkRes = db->executeQuery(checkQuery);
    if (checkRes && checkRes->next()) {
        return;
    }

    string professorValue = currentUser.professorId.empty()
        ? "NULL"
        : "'" + db->escapeString(currentUser.professorId) + "'";
    string insertQuery = "INSERT INTO " + sessionTable + " (" + courseCol + ", " + professorCol + ", " + semesterCol + ", " + dateCol + ", " + typeCol + ") "
                        "VALUES ('" + db->escapeString(courseId) + "', " + professorValue + ", '" +
                        db->escapeString(semesterId) + "', '" + db->escapeString(sessionDate) + "', '" +
                        db->escapeString(sessionType) + "')";
    db->executeUpdate(insertQuery);
}

json MessageHandler::handleDeleteStudent(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin() && !isStudentAffairs()) {
        return createErrorResponse("Access denied");
    }

    string studentId = request.value("id", request.value("studentId", ""));
    if (studentId.empty()) {
        return createErrorResponse("Student ID is required");
    }

    if (studentRepo->deactivateStudent(studentId)) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to delete student");
}

json MessageHandler::handleCreateProfessor(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    Professor professor;
    professor.id = request.value("id", "");
    professor.firstName = request.value("firstName", "");
    professor.lastName = request.value("lastName", "");
    professor.title = request.value("title", "");
    professor.email = request.value("email", "");
    professor.phone = request.value("phone", "");
    professor.departmentId = request.value("departmentId", "");
    professor.hireDate = request.value("hireDate", "");
    professor.specialization = request.value("specialization", "");

    if (professor.id.empty() || professor.firstName.empty() || professor.lastName.empty()) {
        return createErrorResponse("ID, first name, and last name are required");
    }

    if (professorRepo->createProfessor(professor, currentUser.userId)) {
        return createSuccessResponse();
    }

    return createErrorResponse("Failed to create professor");
}

json MessageHandler::handleUpdateProfessor(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string professorId = request.value("id", request.value("professorId", ""));
    if (professorId.empty()) {
        return createErrorResponse("Professor ID is required");
    }

    Professor professor;
    if (!professorRepo->getProfessorById(professorId, professor)) {
        return createErrorResponse("Professor not found");
    }

    if (request.contains("title")) professor.title = request["title"];
    if (request.contains("firstName")) professor.firstName = request["firstName"];
    if (request.contains("lastName")) professor.lastName = request["lastName"];
    if (request.contains("email")) professor.email = request["email"];
    if (request.contains("phone")) professor.phone = request["phone"];
    if (request.contains("departmentId")) professor.departmentId = request["departmentId"];
    if (request.contains("hireDate")) professor.hireDate = request["hireDate"];
    if (request.contains("specialization")) professor.specialization = request["specialization"];

    if (professorRepo->updateProfessor(professorId, professor, currentUser.userId)) {
        return createSuccessResponse();
    }

    return createErrorResponse("Failed to update professor");
}

json MessageHandler::handleDeleteProfessor(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string professorId = request.value("id", request.value("professorId", ""));
    if (professorId.empty()) {
        return createErrorResponse("Professor ID is required");
    }

    if (professorRepo->deleteProfessor(professorId)) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to delete professor");
}

json MessageHandler::handleCreateCourse(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    Course course;
    course.id = request.value("id", "");
    course.code = request.value("code", "");
    course.nameEn = request.value("nameEn", "");
    course.nameAr = request.value("nameAr", "");
    course.description = request.value("description", "");
    course.departmentId = request.value("departmentId", "");
    course.academicLevelId = request.value("levelId", request.value("academicLevelId", ""));
    course.semesterNumber = request.value("semesterNumber", 0);
    course.courseTypeId = request.value("courseTypeId", "");
    course.creditHours = request.value("creditHours", 0);
    course.lectureHours = request.value("lectureHours", 0);
    course.tutorialHours = request.value("tutorialHours", 0);
    course.labHours = request.value("labHours", 0);
    course.practicalHours = request.value("practicalHours", 0);
    course.maxMarks = request.value("maxMarks", 0);
    course.assignment1Marks = request.value("assignment1Marks", 0);
    course.assignment2Marks = request.value("assignment2Marks", 0);
    course.yearWorkMarks = request.value("yearWorkMarks", 0);
    course.finalExamMarks = request.value("finalExamMarks", 0);

    if (course.id.empty() || course.code.empty() || course.nameEn.empty()) {
        return createErrorResponse("Course ID, code, and name are required");
    }
    if (course.departmentId.empty() || course.academicLevelId.empty() || course.courseTypeId.empty()) {
        return createErrorResponse("Department, level, and course type are required");
    }
    if (course.semesterNumber <= 0 || course.creditHours <= 0 || course.maxMarks <= 0) {
        return createErrorResponse("Semester number, credit hours, and max marks are required");
    }

    if (courseRepo->createCourse(course)) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to create course");
}

json MessageHandler::handleUpdateCourse(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string courseId = request.value("id", request.value("courseId", ""));
    if (courseId.empty()) {
        return createErrorResponse("Course ID is required");
    }

    Course course;
    if (!courseRepo->getCourseById(courseId, course)) {
        return createErrorResponse("Course not found");
    }

    if (request.contains("code")) course.code = request["code"];
    if (request.contains("nameEn")) course.nameEn = request["nameEn"];
    if (request.contains("nameAr")) course.nameAr = request["nameAr"];
    if (request.contains("description")) course.description = request["description"];
    if (request.contains("departmentId")) course.departmentId = request["departmentId"];
    if (request.contains("academicLevelId")) course.academicLevelId = request["academicLevelId"];
    if (request.contains("levelId")) course.academicLevelId = request["levelId"];
    if (request.contains("semesterNumber")) course.semesterNumber = request["semesterNumber"];
    if (request.contains("courseTypeId")) course.courseTypeId = request["courseTypeId"];
    if (request.contains("creditHours")) course.creditHours = request["creditHours"];
    if (request.contains("lectureHours")) course.lectureHours = request["lectureHours"];
    if (request.contains("tutorialHours")) course.tutorialHours = request["tutorialHours"];
    if (request.contains("labHours")) course.labHours = request["labHours"];
    if (request.contains("practicalHours")) course.practicalHours = request["practicalHours"];
    if (request.contains("maxMarks")) course.maxMarks = request["maxMarks"];
    if (request.contains("assignment1Marks")) course.assignment1Marks = request["assignment1Marks"];
    if (request.contains("assignment2Marks")) course.assignment2Marks = request["assignment2Marks"];
    if (request.contains("yearWorkMarks")) course.yearWorkMarks = request["yearWorkMarks"];
    if (request.contains("finalExamMarks")) course.finalExamMarks = request["finalExamMarks"];

    if (courseRepo->updateCourse(courseId, course)) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to update course");
}

json MessageHandler::handleDeleteCourse(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string courseId = request.value("id", request.value("courseId", ""));
    if (courseId.empty()) {
        return createErrorResponse("Course ID is required");
    }

    if (courseRepo->deleteCourse(courseId)) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to delete course");
}

json MessageHandler::handleDeleteGrade(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isProfessor() && !isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    int gradeId = request.value("id", request.value("gradeId", 0));
    string studentId = request.value("studentId", "");
    string courseId = request.value("courseId", "");
    string semesterId = request.value("semesterId", "");

    if (courseId.empty() && gradeId == 0) {
        return createErrorResponse("Grade ID or course details are required");
    }

    if (isProfessor() && !courseId.empty() && !isProfessorAssignedToCourse(courseId, semesterId)) {
        return createErrorResponse("Not assigned to this course");
    }

    bool deleted = false;
    if (gradeId > 0) {
        deleted = gradeRepo->deleteGrade(gradeId);
    } else if (!studentId.empty() && !courseId.empty() && !semesterId.empty()) {
        deleted = gradeRepo->deleteGradeByKey(studentId, courseId, semesterId);
    }

    if (deleted) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to delete grade");
}

json MessageHandler::handleUpdateAttendanceRecord(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isProfessor() && !isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    int attendanceId = request.value("id", request.value("attendanceId", 0));
    if (attendanceId <= 0) {
        return createErrorResponse("Attendance ID is required");
    }

    Attendance attendance;
    if (!attendanceRepo->getAttendanceById(attendanceId, attendance)) {
        return createErrorResponse("Attendance record not found");
    }
    if (isProfessor() && !isProfessorAssignedToCourse(attendance.courseId, attendance.semesterId)) {
        return createErrorResponse("Not assigned to this course");
    }

    string status = request.value("status", attendance.status);
    string notes = request.value("notes", request.value("remarks", attendance.notes));
    if (attendanceRepo->updateAttendance(attendanceId, status, notes, currentUser.userId)) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to update attendance");
}

json MessageHandler::handleDeleteAttendance(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isProfessor() && !isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    int attendanceId = request.value("id", request.value("attendanceId", 0));
    if (attendanceId <= 0) {
        return createErrorResponse("Attendance ID is required");
    }

    Attendance attendance;
    if (!attendanceRepo->getAttendanceById(attendanceId, attendance)) {
        return createErrorResponse("Attendance record not found");
    }
    if (isProfessor() && !isProfessorAssignedToCourse(attendance.courseId, attendance.semesterId)) {
        return createErrorResponse("Not assigned to this course");
    }

    if (attendanceRepo->deleteAttendance(attendanceId)) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to delete attendance");
}

json MessageHandler::handleGetRegistrations(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string studentId = request.value("studentId", "");
    string courseId = request.value("courseId", "");
    string semesterId = request.value("semesterId", "");

    json registrationsArray = json::array();

    if (!studentId.empty()) {
        auto registrations = registrationRepo->getStudentRegistrations(studentId, semesterId);
        for (auto& r : registrations) {
            registrationsArray.push_back({
                {"id", r.id},
                {"studentId", r.studentId},
                {"studentName", r.studentName},
                {"courseId", r.courseId},
                {"courseCode", r.courseCode},
                {"courseName", r.courseName},
                {"semesterId", r.semesterId},
                {"status", r.status},
                {"createdAt", r.createdAt}
            });
        }
    } else if (!courseId.empty()) {
        string effectiveSemester = semesterId.empty() ? getCurrentSemesterId() : semesterId;
        auto registrations = registrationRepo->getCourseRegistrations(courseId, effectiveSemester);
        for (auto& r : registrations) {
            registrationsArray.push_back({
                {"id", r.id},
                {"studentId", r.studentId},
                {"studentName", r.studentName},
                {"courseId", r.courseId},
                {"courseCode", r.courseCode},
                {"courseName", r.courseName},
                {"semesterId", r.semesterId},
                {"status", r.status},
                {"createdAt", r.createdAt}
            });
        }
    } else {
        string registrationTable = db->resolveTable({"registrations"});
        string studentTable = db->resolveTable({"students"});
        string courseTable = db->resolveTable({"courses"});
        if (db->tableExists(registrationTable)) {
            string studentCol = db->resolveColumn(registrationTable, {"student_id", "studentId"});
            string courseCol = db->resolveColumn(registrationTable, {"course_id", "courseId"});
            string semesterCol = db->resolveColumn(registrationTable, {"semester_id", "semesterId"});
            string statusCol = db->resolveColumn(registrationTable, {"status"});
            string createdAtCol = db->resolveColumn(registrationTable, {"created_at", "createdAt"});

            string studentIdCol = db->resolveColumn(studentTable, {"id", "student_id", "studentId"});
            string courseIdCol = db->resolveColumn(courseTable, {"id", "course_id", "courseId"});
            string studentFirstCol = db->resolveColumn(studentTable, {"first_name", "firstName"});
            string studentLastCol = db->resolveColumn(studentTable, {"last_name", "lastName"});
            string courseNameCol = db->resolveColumn(courseTable, {"name_en", "nameEn", "name"});
            string courseCodeCol = db->resolveColumn(courseTable, {"code", "course_code", "courseCode"});

            string query = "SELECT r.*, CONCAT(s." + studentFirstCol + ", ' ', s." + studentLastCol + ") as student_name, "
                          "c." + courseNameCol + " as course_name, c." + courseCodeCol + " as course_code "
                          "FROM " + registrationTable + " r "
                          "JOIN " + studentTable + " s ON r." + studentCol + " = s." + studentIdCol + " "
                          "JOIN " + courseTable + " c ON r." + courseCol + " = c." + courseIdCol + " "
                          "ORDER BY r.id DESC";
            auto results = db->executeQuery(query);
            if (results) {
                while (results->next()) {
                    registrationsArray.push_back({
                        {"id", results->getInt("id")},
                        {"studentId", results->getString(studentCol)},
                        {"studentName", results->getString("student_name")},
                        {"courseId", results->getString(courseCol)},
                        {"courseCode", results->getString("course_code")},
                        {"courseName", results->getString("course_name")},
                        {"semesterId", results->getString(semesterCol)},
                        {"status", results->getString(statusCol)},
                        {"createdAt", results->getString(createdAtCol)}
                    });
                }
            }
        }
    }

    json responseData;
    responseData["registrations"] = registrationsArray;
    return createSuccessResponse(responseData);
}

json MessageHandler::handleUpdateRegistration(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    int registrationId = request.value("id", request.value("registrationId", 0));
    string status = request.value("status", "");
    if (registrationId <= 0 || status.empty()) {
        return createErrorResponse("Registration ID and status are required");
    }

    if (registrationRepo->updateRegistrationStatus(registrationId, status)) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to update registration");
}

json MessageHandler::handleDeleteRegistration(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    int registrationId = request.value("id", request.value("registrationId", 0));
    if (registrationId <= 0) {
        return createErrorResponse("Registration ID is required");
    }

    if (registrationRepo->deleteRegistration(registrationId)) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to delete registration");
}

json MessageHandler::handleGetCourseAssignments(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    json assignmentsArray = json::array();

    string assignmentTable = db->resolveTable({"course_assignments", "professor_courses"});
    if (!db->tableExists(assignmentTable)) {
        json responseData;
        responseData["assignments"] = assignmentsArray;
        return createSuccessResponse(responseData);
    }

    string courseTable = db->resolveTable({"courses"});
    string professorTable = db->resolveTable({"professors"});
    string semesterTable = db->resolveTable({"semesters"});

    string assignmentIdCol = db->resolveColumn(assignmentTable, {"id"});
    string assignmentCourseCol = db->resolveColumn(assignmentTable, {"course_id", "courseId"});
    string assignmentProfessorCol = db->resolveColumn(assignmentTable, {"professor_id", "professorId"});
    string assignmentSemesterCol = db->resolveColumn(assignmentTable, {"semester_id", "semesterId"});
    string assignmentPrimaryCol = db->resolveColumn(assignmentTable, {"is_primary", "isPrimary"});

    string courseIdCol = db->resolveColumn(courseTable, {"id", "course_id", "courseId"});
    string courseCodeCol = db->resolveColumn(courseTable, {"code", "course_code", "courseCode"});
    string courseNameCol = db->resolveColumn(courseTable, {"name_en", "nameEn", "name"});
    string professorIdCol = db->resolveColumn(professorTable, {"id", "professor_id", "professorId"});
    string professorTitleCol = db->resolveColumn(professorTable, {"title"});
    string professorFirstCol = db->resolveColumn(professorTable, {"first_name", "firstName"});
    string professorLastCol = db->resolveColumn(professorTable, {"last_name", "lastName"});
    string semesterIdCol = db->resolveColumn(semesterTable, {"id", "semester_id", "semesterId"});
    string semesterNameCol = db->resolveColumn(semesterTable, {"name_en", "nameEn", "name"});

    string query = "SELECT a.*, c." + courseCodeCol + " as course_code, c." + courseNameCol + " as course_name, "
                  "s." + semesterNameCol + " as semester_name, "
                  "CONCAT(COALESCE(p." + professorTitleCol + ", ''), ' ', p." + professorFirstCol + ", ' ', p." + professorLastCol + ") as professor_name "
                  "FROM " + assignmentTable + " a "
                  "JOIN " + courseTable + " c ON a." + assignmentCourseCol + " = c." + courseIdCol + " "
                  "JOIN " + professorTable + " p ON a." + assignmentProfessorCol + " = p." + professorIdCol + " "
                  "JOIN " + semesterTable + " s ON a." + assignmentSemesterCol + " = s." + semesterIdCol + " "
                  "ORDER BY s." + semesterIdCol + ", c." + courseCodeCol;
    auto results = db->executeQuery(query);
    if (results) {
        while (results->next()) {
            assignmentsArray.push_back({
                {"id", results->getInt(assignmentIdCol)},
                {"courseId", results->getString(assignmentCourseCol)},
                {"courseCode", results->getString("course_code")},
                {"courseName", results->getString("course_name")},
                {"professorId", results->getString(assignmentProfessorCol)},
                {"professorName", results->getString("professor_name")},
                {"semesterId", results->getString(assignmentSemesterCol)},
                {"semesterName", results->getString("semester_name")},
                {"isPrimary", results->getBoolean(assignmentPrimaryCol)}
            });
        }
    }

    json responseData;
    responseData["assignments"] = assignmentsArray;
    return createSuccessResponse(responseData);
}

json MessageHandler::handleCreateCourseAssignment(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string courseId = request.value("courseId", "");
    string professorId = request.value("professorId", "");
    string semesterId = request.value("semesterId", "");
    bool isPrimary = request.value("isPrimary", true);

    if (courseId.empty() || professorId.empty() || semesterId.empty()) {
        return createErrorResponse("Course, professor, and semester are required");
    }

    string assignmentTable = db->resolveTable({"course_assignments", "professor_courses"});
    if (!db->tableExists(assignmentTable)) {
        return createErrorResponse("Assignments table not found");
    }

    string assignmentCourseCol = db->resolveColumn(assignmentTable, {"course_id", "courseId"});
    string assignmentProfessorCol = db->resolveColumn(assignmentTable, {"professor_id", "professorId"});
    string assignmentSemesterCol = db->resolveColumn(assignmentTable, {"semester_id", "semesterId"});
    string assignmentPrimaryCol = db->resolveColumn(assignmentTable, {"is_primary", "isPrimary"});
    string assignmentByCol = db->resolveColumn(assignmentTable, {"assigned_by", "assignedBy"});

    string query = "INSERT INTO " + assignmentTable + " (" + assignmentCourseCol + ", " + assignmentProfessorCol + ", " + assignmentSemesterCol;
    if (db->columnExists(assignmentTable, assignmentPrimaryCol)) {
        query += ", " + assignmentPrimaryCol;
    }
    if (db->columnExists(assignmentTable, assignmentByCol)) {
        query += ", " + assignmentByCol;
    }
    query += ") VALUES ('" + db->escapeString(courseId) + "', '" + db->escapeString(professorId) + "', '" + db->escapeString(semesterId) + "'";
    if (db->columnExists(assignmentTable, assignmentPrimaryCol)) {
        query += ", " + string(isPrimary ? "TRUE" : "FALSE");
    }
    if (db->columnExists(assignmentTable, assignmentByCol)) {
        query += ", " + to_string(currentUser.userId);
    }
    query += ") ON DUPLICATE KEY UPDATE ";
    if (db->columnExists(assignmentTable, assignmentPrimaryCol)) {
        query += assignmentPrimaryCol + " = " + string(isPrimary ? "TRUE" : "FALSE");
    } else {
        query += assignmentCourseCol + " = " + assignmentCourseCol;
    }

    if (db->executeUpdate(query) >= 0) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to assign professor");
}

json MessageHandler::handleDeleteCourseAssignment(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    int assignmentId = request.value("id", request.value("assignmentId", 0));
    string courseId = request.value("courseId", "");
    string professorId = request.value("professorId", "");
    string semesterId = request.value("semesterId", "");

    string assignmentTable = db->resolveTable({"course_assignments", "professor_courses"});
    if (!db->tableExists(assignmentTable)) {
        return createErrorResponse("Assignments table not found");
    }

    string assignmentIdCol = db->resolveColumn(assignmentTable, {"id"});
    string assignmentCourseCol = db->resolveColumn(assignmentTable, {"course_id", "courseId"});
    string assignmentProfessorCol = db->resolveColumn(assignmentTable, {"professor_id", "professorId"});
    string assignmentSemesterCol = db->resolveColumn(assignmentTable, {"semester_id", "semesterId"});

    string query = "DELETE FROM " + assignmentTable + " WHERE ";
    if (assignmentId > 0) {
        query += assignmentIdCol + " = " + to_string(assignmentId);
    } else if (!courseId.empty() && !professorId.empty() && !semesterId.empty()) {
        query += assignmentCourseCol + " = '" + db->escapeString(courseId) + "' "
                 "AND " + assignmentProfessorCol + " = '" + db->escapeString(professorId) + "' "
                 "AND " + assignmentSemesterCol + " = '" + db->escapeString(semesterId) + "'";
    } else {
        return createErrorResponse("Assignment ID or full assignment keys are required");
    }

    if (db->executeUpdate(query) > 0) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to delete assignment");
}

json MessageHandler::handleGetDepartments(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    json departmentsArray = json::array();
    string departmentTable = db->resolveTable({"departments"});
    if (!db->tableExists(departmentTable)) {
        json responseData;
        responseData["departments"] = departmentsArray;
        return createSuccessResponse(responseData);
    }

    string collegeTable = db->resolveTable({"colleges"});
    string deptIdCol = db->resolveColumn(departmentTable, {"id", "department_id", "departmentId"});
    string deptCollegeCol = db->resolveColumn(departmentTable, {"college_id", "collegeId"});
    string deptNameEnCol = db->resolveColumn(departmentTable, {"name_en", "nameEn", "name"});
    string deptNameArCol = db->resolveColumn(departmentTable, {"name_ar", "nameAr"});
    string deptCodeCol = db->resolveColumn(departmentTable, {"code"});

    string collegeIdCol = db->resolveColumn(collegeTable, {"id", "college_id", "collegeId"});
    string collegeNameEnCol = db->resolveColumn(collegeTable, {"name_en", "nameEn", "name"});

    string query = "SELECT d." + deptIdCol + " as dept_id, d." + deptCodeCol + " as dept_code, "
                  "d." + deptNameEnCol + " as dept_name_en, d." + deptNameArCol + " as dept_name_ar, "
                  "d." + deptCollegeCol + " as college_id";
    if (db->tableExists(collegeTable)) {
        query += ", c." + collegeNameEnCol + " as college_name";
    }
    query += " FROM " + departmentTable + " d ";
    if (db->tableExists(collegeTable)) {
        query += "LEFT JOIN " + collegeTable + " c ON d." + deptCollegeCol + " = c." + collegeIdCol + " ";
    }
    query += "ORDER BY d." + deptCodeCol;

    auto results = db->executeQuery(query);
    if (results) {
        while (results->next()) {
            departmentsArray.push_back({
                {"id", results->getString("dept_id")},
                {"code", results->getString("dept_code")},
                {"nameEn", results->getString("dept_name_en")},
                {"nameAr", results->getString("dept_name_ar")},
                {"collegeId", results->getString("college_id")},
                {"collegeName", results->getString("college_name")}
            });
        }
    }

    json responseData;
    responseData["departments"] = departmentsArray;
    return createSuccessResponse(responseData);
}

json MessageHandler::handleCreateDepartment(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string departmentTable = db->resolveTable({"departments"});
    if (!db->tableExists(departmentTable)) {
        return createErrorResponse("Departments table not found");
    }

    string id = request.value("id", "");
    string collegeId = request.value("collegeId", "");
    string nameEn = request.value("nameEn", "");
    string nameAr = request.value("nameAr", "");
    string code = request.value("code", "");

    if (id.empty() || collegeId.empty() || nameEn.empty() || code.empty()) {
        return createErrorResponse("ID, college, name, and code are required");
    }

    string deptIdCol = db->resolveColumn(departmentTable, {"id", "department_id", "departmentId"});
    string deptCollegeCol = db->resolveColumn(departmentTable, {"college_id", "collegeId"});
    string deptNameEnCol = db->resolveColumn(departmentTable, {"name_en", "nameEn", "name"});
    string deptNameArCol = db->resolveColumn(departmentTable, {"name_ar", "nameAr"});
    string deptCodeCol = db->resolveColumn(departmentTable, {"code"});

    vector<string> cols;
    vector<string> vals;
    cols.push_back(deptIdCol);
    vals.push_back("'" + db->escapeString(id) + "'");
    cols.push_back(deptCollegeCol);
    vals.push_back("'" + db->escapeString(collegeId) + "'");
    cols.push_back(deptNameEnCol);
    vals.push_back("'" + db->escapeString(nameEn) + "'");
    if (db->columnExists(departmentTable, deptNameArCol)) {
        cols.push_back(deptNameArCol);
        vals.push_back("'" + db->escapeString(nameAr) + "'");
    }
    cols.push_back(deptCodeCol);
    vals.push_back("'" + db->escapeString(code) + "'");

    string query = "INSERT INTO " + departmentTable + " (";
    for (size_t i = 0; i < cols.size(); ++i) {
        if (i > 0) query += ", ";
        query += cols[i];
    }
    query += ") VALUES (";
    for (size_t i = 0; i < vals.size(); ++i) {
        if (i > 0) query += ", ";
        query += vals[i];
    }
    query += ")";

    if (db->executeUpdate(query) > 0) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to create department");
}

json MessageHandler::handleUpdateDepartment(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string departmentTable = db->resolveTable({"departments"});
    if (!db->tableExists(departmentTable)) {
        return createErrorResponse("Departments table not found");
    }

    string id = request.value("id", request.value("departmentId", ""));
    if (id.empty()) {
        return createErrorResponse("Department ID is required");
    }

    string deptIdCol = db->resolveColumn(departmentTable, {"id", "department_id", "departmentId"});
    string deptCollegeCol = db->resolveColumn(departmentTable, {"college_id", "collegeId"});
    string deptNameEnCol = db->resolveColumn(departmentTable, {"name_en", "nameEn", "name"});
    string deptNameArCol = db->resolveColumn(departmentTable, {"name_ar", "nameAr"});
    string deptCodeCol = db->resolveColumn(departmentTable, {"code"});

    vector<string> updates;
    if (request.contains("collegeId") && db->columnExists(departmentTable, deptCollegeCol)) {
        updates.push_back(deptCollegeCol + " = '" + db->escapeString(request["collegeId"]) + "'");
    }
    if (request.contains("nameEn") && db->columnExists(departmentTable, deptNameEnCol)) {
        updates.push_back(deptNameEnCol + " = '" + db->escapeString(request["nameEn"]) + "'");
    }
    if (request.contains("nameAr") && db->columnExists(departmentTable, deptNameArCol)) {
        updates.push_back(deptNameArCol + " = '" + db->escapeString(request["nameAr"]) + "'");
    }
    if (request.contains("code") && db->columnExists(departmentTable, deptCodeCol)) {
        updates.push_back(deptCodeCol + " = '" + db->escapeString(request["code"]) + "'");
    }

    if (updates.empty()) {
        return createErrorResponse("No changes provided");
    }

    string query = "UPDATE " + departmentTable + " SET ";
    for (size_t i = 0; i < updates.size(); ++i) {
        if (i > 0) query += ", ";
        query += updates[i];
    }
    query += " WHERE " + deptIdCol + " = '" + db->escapeString(id) + "'";

    if (db->executeUpdate(query) > 0) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to update department");
}

json MessageHandler::handleDeleteDepartment(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string departmentTable = db->resolveTable({"departments"});
    if (!db->tableExists(departmentTable)) {
        return createErrorResponse("Departments table not found");
    }

    string id = request.value("id", request.value("departmentId", ""));
    if (id.empty()) {
        return createErrorResponse("Department ID is required");
    }

    string deptIdCol = db->resolveColumn(departmentTable, {"id", "department_id", "departmentId"});
    string query = "DELETE FROM " + departmentTable + " WHERE " + deptIdCol + " = '" + db->escapeString(id) + "'";
    if (db->executeUpdate(query) > 0) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to delete department");
}

json MessageHandler::handleGetColleges(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    json collegesArray = json::array();
    string collegeTable = db->resolveTable({"colleges"});
    if (!db->tableExists(collegeTable)) {
        json responseData;
        responseData["colleges"] = collegesArray;
        return createSuccessResponse(responseData);
    }

    string collegeIdCol = db->resolveColumn(collegeTable, {"id", "college_id", "collegeId"});
    string nameEnCol = db->resolveColumn(collegeTable, {"name_en", "nameEn", "name"});
    string nameArCol = db->resolveColumn(collegeTable, {"name_ar", "nameAr"});
    string activeCol = db->resolveColumn(collegeTable, {"is_active", "isActive", "active"});

    string query = "SELECT * FROM " + collegeTable + " ORDER BY " + collegeIdCol;
    auto results = db->executeQuery(query);
    if (results) {
        while (results->next()) {
            collegesArray.push_back({
                {"id", results->getString(collegeIdCol)},
                {"nameEn", results->getString(nameEnCol)},
                {"nameAr", results->getString(nameArCol)},
                {"isActive", results->getBoolean(activeCol)}
            });
        }
    }

    json responseData;
    responseData["colleges"] = collegesArray;
    return createSuccessResponse(responseData);
}

json MessageHandler::handleCreateCollege(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string collegeTable = db->resolveTable({"colleges"});
    if (!db->tableExists(collegeTable)) {
        return createErrorResponse("Colleges table not found");
    }

    string id = request.value("id", "");
    string nameEn = request.value("nameEn", "");
    string nameAr = request.value("nameAr", "");
    bool isActive = request.value("isActive", true);

    if (id.empty() || nameEn.empty()) {
        return createErrorResponse("ID and name are required");
    }

    string collegeIdCol = db->resolveColumn(collegeTable, {"id", "college_id", "collegeId"});
    string nameEnCol = db->resolveColumn(collegeTable, {"name_en", "nameEn", "name"});
    string nameArCol = db->resolveColumn(collegeTable, {"name_ar", "nameAr"});
    string activeCol = db->resolveColumn(collegeTable, {"is_active", "isActive", "active"});

    vector<string> cols;
    vector<string> vals;
    cols.push_back(collegeIdCol);
    vals.push_back("'" + db->escapeString(id) + "'");
    cols.push_back(nameEnCol);
    vals.push_back("'" + db->escapeString(nameEn) + "'");
    if (db->columnExists(collegeTable, nameArCol)) {
        cols.push_back(nameArCol);
        vals.push_back("'" + db->escapeString(nameAr) + "'");
    }
    if (db->columnExists(collegeTable, activeCol)) {
        cols.push_back(activeCol);
        vals.push_back(isActive ? "TRUE" : "FALSE");
    }

    string query = "INSERT INTO " + collegeTable + " (";
    for (size_t i = 0; i < cols.size(); ++i) {
        if (i > 0) query += ", ";
        query += cols[i];
    }
    query += ") VALUES (";
    for (size_t i = 0; i < vals.size(); ++i) {
        if (i > 0) query += ", ";
        query += vals[i];
    }
    query += ")";

    if (db->executeUpdate(query) > 0) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to create college");
}

json MessageHandler::handleUpdateCollege(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string collegeTable = db->resolveTable({"colleges"});
    if (!db->tableExists(collegeTable)) {
        return createErrorResponse("Colleges table not found");
    }

    string id = request.value("id", request.value("collegeId", ""));
    if (id.empty()) {
        return createErrorResponse("College ID is required");
    }

    string collegeIdCol = db->resolveColumn(collegeTable, {"id", "college_id", "collegeId"});
    string nameEnCol = db->resolveColumn(collegeTable, {"name_en", "nameEn", "name"});
    string nameArCol = db->resolveColumn(collegeTable, {"name_ar", "nameAr"});
    string activeCol = db->resolveColumn(collegeTable, {"is_active", "isActive", "active"});

    vector<string> updates;
    if (request.contains("nameEn") && db->columnExists(collegeTable, nameEnCol)) {
        updates.push_back(nameEnCol + " = '" + db->escapeString(request["nameEn"]) + "'");
    }
    if (request.contains("nameAr") && db->columnExists(collegeTable, nameArCol)) {
        updates.push_back(nameArCol + " = '" + db->escapeString(request["nameAr"]) + "'");
    }
    if (request.contains("isActive") && db->columnExists(collegeTable, activeCol)) {
        updates.push_back(activeCol + " = " + string(request["isActive"].get<bool>() ? "TRUE" : "FALSE"));
    }

    if (updates.empty()) {
        return createErrorResponse("No changes provided");
    }

    string query = "UPDATE " + collegeTable + " SET ";
    for (size_t i = 0; i < updates.size(); ++i) {
        if (i > 0) query += ", ";
        query += updates[i];
    }
    query += " WHERE " + collegeIdCol + " = '" + db->escapeString(id) + "'";

    if (db->executeUpdate(query) > 0) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to update college");
}

json MessageHandler::handleDeleteCollege(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string collegeTable = db->resolveTable({"colleges"});
    if (!db->tableExists(collegeTable)) {
        return createErrorResponse("Colleges table not found");
    }

    string id = request.value("id", request.value("collegeId", ""));
    if (id.empty()) {
        return createErrorResponse("College ID is required");
    }

    string collegeIdCol = db->resolveColumn(collegeTable, {"id", "college_id", "collegeId"});
    string query = "DELETE FROM " + collegeTable + " WHERE " + collegeIdCol + " = '" + db->escapeString(id) + "'";
    if (db->executeUpdate(query) > 0) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to delete college");
}

json MessageHandler::handleGetAcademicLevels(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    json levelsArray = json::array();
    string levelTable = db->resolveTable({"academic_levels", "levels"});
    if (!db->tableExists(levelTable)) {
        json responseData;
        responseData["levels"] = levelsArray;
        return createSuccessResponse(responseData);
    }

    string idCol = db->resolveColumn(levelTable, {"id", "level_id", "levelId"});
    string nameEnCol = db->resolveColumn(levelTable, {"name_en", "nameEn", "name"});
    string nameArCol = db->resolveColumn(levelTable, {"name_ar", "nameAr"});
    string yearCol = db->resolveColumn(levelTable, {"year_number", "yearNumber", "year"});

    string query = "SELECT * FROM " + levelTable + " ORDER BY " + yearCol;
    auto results = db->executeQuery(query);
    if (results) {
        while (results->next()) {
            levelsArray.push_back({
                {"id", results->getString(idCol)},
                {"nameEn", results->getString(nameEnCol)},
                {"nameAr", results->getString(nameArCol)},
                {"yearNumber", results->getInt(yearCol)}
            });
        }
    }

    json responseData;
    responseData["levels"] = levelsArray;
    return createSuccessResponse(responseData);
}

json MessageHandler::handleCreateAcademicLevel(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string levelTable = db->resolveTable({"academic_levels", "levels"});
    if (!db->tableExists(levelTable)) {
        return createErrorResponse("Academic levels table not found");
    }

    string id = request.value("id", "");
    string nameEn = request.value("nameEn", "");
    string nameAr = request.value("nameAr", "");
    int yearNumber = request.value("yearNumber", 0);

    if (id.empty() || nameEn.empty() || yearNumber <= 0) {
        return createErrorResponse("ID, name, and year number are required");
    }

    string idCol = db->resolveColumn(levelTable, {"id", "level_id", "levelId"});
    string nameEnCol = db->resolveColumn(levelTable, {"name_en", "nameEn", "name"});
    string nameArCol = db->resolveColumn(levelTable, {"name_ar", "nameAr"});
    string yearCol = db->resolveColumn(levelTable, {"year_number", "yearNumber", "year"});

    vector<string> cols;
    vector<string> vals;
    cols.push_back(idCol);
    vals.push_back("'" + db->escapeString(id) + "'");
    cols.push_back(nameEnCol);
    vals.push_back("'" + db->escapeString(nameEn) + "'");
    if (db->columnExists(levelTable, nameArCol)) {
        cols.push_back(nameArCol);
        vals.push_back("'" + db->escapeString(nameAr) + "'");
    }
    cols.push_back(yearCol);
    vals.push_back(to_string(yearNumber));

    string query = "INSERT INTO " + levelTable + " (";
    for (size_t i = 0; i < cols.size(); ++i) {
        if (i > 0) query += ", ";
        query += cols[i];
    }
    query += ") VALUES (";
    for (size_t i = 0; i < vals.size(); ++i) {
        if (i > 0) query += ", ";
        query += vals[i];
    }
    query += ")";

    if (db->executeUpdate(query) > 0) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to create academic level");
}

json MessageHandler::handleUpdateAcademicLevel(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string levelTable = db->resolveTable({"academic_levels", "levels"});
    if (!db->tableExists(levelTable)) {
        return createErrorResponse("Academic levels table not found");
    }

    string id = request.value("id", request.value("levelId", ""));
    if (id.empty()) {
        return createErrorResponse("Level ID is required");
    }

    string idCol = db->resolveColumn(levelTable, {"id", "level_id", "levelId"});
    string nameEnCol = db->resolveColumn(levelTable, {"name_en", "nameEn", "name"});
    string nameArCol = db->resolveColumn(levelTable, {"name_ar", "nameAr"});
    string yearCol = db->resolveColumn(levelTable, {"year_number", "yearNumber", "year"});

    vector<string> updates;
    if (request.contains("nameEn") && db->columnExists(levelTable, nameEnCol)) {
        updates.push_back(nameEnCol + " = '" + db->escapeString(request["nameEn"]) + "'");
    }
    if (request.contains("nameAr") && db->columnExists(levelTable, nameArCol)) {
        updates.push_back(nameArCol + " = '" + db->escapeString(request["nameAr"]) + "'");
    }
    if (request.contains("yearNumber") && db->columnExists(levelTable, yearCol)) {
        updates.push_back(yearCol + " = " + to_string((int)request["yearNumber"]));
    }

    if (updates.empty()) {
        return createErrorResponse("No changes provided");
    }

    string query = "UPDATE " + levelTable + " SET ";
    for (size_t i = 0; i < updates.size(); ++i) {
        if (i > 0) query += ", ";
        query += updates[i];
    }
    query += " WHERE " + idCol + " = '" + db->escapeString(id) + "'";

    if (db->executeUpdate(query) > 0) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to update academic level");
}

json MessageHandler::handleDeleteAcademicLevel(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string levelTable = db->resolveTable({"academic_levels", "levels"});
    if (!db->tableExists(levelTable)) {
        return createErrorResponse("Academic levels table not found");
    }

    string id = request.value("id", request.value("levelId", ""));
    if (id.empty()) {
        return createErrorResponse("Level ID is required");
    }

    string idCol = db->resolveColumn(levelTable, {"id", "level_id", "levelId"});
    string query = "DELETE FROM " + levelTable + " WHERE " + idCol + " = '" + db->escapeString(id) + "'";
    if (db->executeUpdate(query) > 0) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to delete academic level");
}

json MessageHandler::handleGetSemesters(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    json semestersArray = json::array();
    string semesterTable = db->resolveTable({"semesters"});
    if (!db->tableExists(semesterTable)) {
        json responseData;
        responseData["semesters"] = semestersArray;
        return createSuccessResponse(responseData);
    }

    string idCol = db->resolveColumn(semesterTable, {"id", "semester_id", "semesterId"});
    string nameEnCol = db->resolveColumn(semesterTable, {"name_en", "nameEn", "name"});
    string nameArCol = db->resolveColumn(semesterTable, {"name_ar", "nameAr"});
    string yearCol = db->resolveColumn(semesterTable, {"academic_year", "academicYear"});
    string numberCol = db->resolveColumn(semesterTable, {"semester_number", "semesterNumber"});
    string currentCol = db->resolveColumn(semesterTable, {"is_current", "isCurrent", "current"});

    string query = "SELECT * FROM " + semesterTable + " ORDER BY " + yearCol + " DESC, " + numberCol + " DESC";
    auto results = db->executeQuery(query);
    if (results) {
        while (results->next()) {
            semestersArray.push_back({
                {"id", results->getString(idCol)},
                {"nameEn", results->getString(nameEnCol)},
                {"nameAr", results->getString(nameArCol)},
                {"academicYear", results->getString(yearCol)},
                {"semesterNumber", results->getInt(numberCol)},
                {"isCurrent", results->getBoolean(currentCol)}
            });
        }
    }

    json responseData;
    responseData["semesters"] = semestersArray;
    return createSuccessResponse(responseData);
}

json MessageHandler::handleCreateSemester(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string semesterTable = db->resolveTable({"semesters"});
    if (!db->tableExists(semesterTable)) {
        return createErrorResponse("Semesters table not found");
    }

    string id = request.value("id", "");
    string nameEn = request.value("nameEn", "");
    string nameAr = request.value("nameAr", "");
    string academicYear = request.value("academicYear", "");
    int semesterNumber = request.value("semesterNumber", 0);
    bool isCurrent = request.value("isCurrent", false);

    if (id.empty() || academicYear.empty() || semesterNumber <= 0) {
        return createErrorResponse("ID, academic year, and semester number are required");
    }

    string idCol = db->resolveColumn(semesterTable, {"id", "semester_id", "semesterId"});
    string nameEnCol = db->resolveColumn(semesterTable, {"name_en", "nameEn", "name"});
    string nameArCol = db->resolveColumn(semesterTable, {"name_ar", "nameAr"});
    string yearCol = db->resolveColumn(semesterTable, {"academic_year", "academicYear"});
    string numberCol = db->resolveColumn(semesterTable, {"semester_number", "semesterNumber"});
    string currentCol = db->resolveColumn(semesterTable, {"is_current", "isCurrent", "current"});

    if (isCurrent && db->columnExists(semesterTable, currentCol)) {
        string resetQuery = "UPDATE " + semesterTable + " SET " + currentCol + " = FALSE";
        db->executeUpdate(resetQuery);
    }

    vector<string> cols;
    vector<string> vals;
    cols.push_back(idCol);
    vals.push_back("'" + db->escapeString(id) + "'");
    cols.push_back(yearCol);
    vals.push_back("'" + db->escapeString(academicYear) + "'");
    cols.push_back(numberCol);
    vals.push_back(to_string(semesterNumber));
    if (db->columnExists(semesterTable, nameEnCol)) {
        cols.push_back(nameEnCol);
        vals.push_back("'" + db->escapeString(nameEn) + "'");
    }
    if (db->columnExists(semesterTable, nameArCol)) {
        cols.push_back(nameArCol);
        vals.push_back("'" + db->escapeString(nameAr) + "'");
    }
    if (db->columnExists(semesterTable, currentCol)) {
        cols.push_back(currentCol);
        vals.push_back(isCurrent ? "TRUE" : "FALSE");
    }

    string query = "INSERT INTO " + semesterTable + " (";
    for (size_t i = 0; i < cols.size(); ++i) {
        if (i > 0) query += ", ";
        query += cols[i];
    }
    query += ") VALUES (";
    for (size_t i = 0; i < vals.size(); ++i) {
        if (i > 0) query += ", ";
        query += vals[i];
    }
    query += ")";

    if (db->executeUpdate(query) > 0) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to create semester");
}

json MessageHandler::handleUpdateSemester(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string semesterTable = db->resolveTable({"semesters"});
    if (!db->tableExists(semesterTable)) {
        return createErrorResponse("Semesters table not found");
    }

    string id = request.value("id", request.value("semesterId", ""));
    if (id.empty()) {
        return createErrorResponse("Semester ID is required");
    }

    string idCol = db->resolveColumn(semesterTable, {"id", "semester_id", "semesterId"});
    string nameEnCol = db->resolveColumn(semesterTable, {"name_en", "nameEn", "name"});
    string nameArCol = db->resolveColumn(semesterTable, {"name_ar", "nameAr"});
    string yearCol = db->resolveColumn(semesterTable, {"academic_year", "academicYear"});
    string numberCol = db->resolveColumn(semesterTable, {"semester_number", "semesterNumber"});
    string currentCol = db->resolveColumn(semesterTable, {"is_current", "isCurrent", "current"});

    bool setCurrent = request.contains("isCurrent") && request["isCurrent"].get<bool>();
    if (setCurrent && db->columnExists(semesterTable, currentCol)) {
        string resetQuery = "UPDATE " + semesterTable + " SET " + currentCol + " = FALSE";
        db->executeUpdate(resetQuery);
    }

    vector<string> updates;
    if (request.contains("nameEn") && db->columnExists(semesterTable, nameEnCol)) {
        updates.push_back(nameEnCol + " = '" + db->escapeString(request["nameEn"]) + "'");
    }
    if (request.contains("nameAr") && db->columnExists(semesterTable, nameArCol)) {
        updates.push_back(nameArCol + " = '" + db->escapeString(request["nameAr"]) + "'");
    }
    if (request.contains("academicYear") && db->columnExists(semesterTable, yearCol)) {
        updates.push_back(yearCol + " = '" + db->escapeString(request["academicYear"]) + "'");
    }
    if (request.contains("semesterNumber") && db->columnExists(semesterTable, numberCol)) {
        updates.push_back(numberCol + " = " + to_string((int)request["semesterNumber"]));
    }
    if (request.contains("isCurrent") && db->columnExists(semesterTable, currentCol)) {
        updates.push_back(currentCol + " = " + string(request["isCurrent"].get<bool>() ? "TRUE" : "FALSE"));
    }

    if (updates.empty()) {
        return createErrorResponse("No changes provided");
    }

    string query = "UPDATE " + semesterTable + " SET ";
    for (size_t i = 0; i < updates.size(); ++i) {
        if (i > 0) query += ", ";
        query += updates[i];
    }
    query += " WHERE " + idCol + " = '" + db->escapeString(id) + "'";

    if (db->executeUpdate(query) > 0) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to update semester");
}

json MessageHandler::handleDeleteSemester(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string semesterTable = db->resolveTable({"semesters"});
    if (!db->tableExists(semesterTable)) {
        return createErrorResponse("Semesters table not found");
    }

    string id = request.value("id", request.value("semesterId", ""));
    if (id.empty()) {
        return createErrorResponse("Semester ID is required");
    }

    string idCol = db->resolveColumn(semesterTable, {"id", "semester_id", "semesterId"});
    string query = "DELETE FROM " + semesterTable + " WHERE " + idCol + " = '" + db->escapeString(id) + "'";
    if (db->executeUpdate(query) > 0) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to delete semester");
}

json MessageHandler::handleGetCourseTypes(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    json typesArray = json::array();
    string typeTable = db->resolveTable({"course_types", "course_types"});
    if (!db->tableExists(typeTable)) {
        json responseData;
        responseData["courseTypes"] = typesArray;
        return createSuccessResponse(responseData);
    }

    string idCol = db->resolveColumn(typeTable, {"id", "type_id", "courseTypeId"});
    string nameCol = db->resolveColumn(typeTable, {"name", "type_name", "typeName"});
    string maxCol = db->resolveColumn(typeTable, {"max_marks", "maxMarks"});
    string assignment1Col = db->resolveColumn(typeTable, {"assignment1_max", "assignment1Max"});
    string assignment2Col = db->resolveColumn(typeTable, {"assignment2_max", "assignment2Max"});
    string yearWorkCol = db->resolveColumn(typeTable, {"year_work_max", "yearWorkMax"});
    string finalExamCol = db->resolveColumn(typeTable, {"final_exam_max", "finalExamMax"});

    string query = "SELECT * FROM " + typeTable + " ORDER BY " + idCol;
    auto results = db->executeQuery(query);
    if (results) {
        while (results->next()) {
            typesArray.push_back({
                {"id", results->getString(idCol)},
                {"name", results->getString(nameCol)},
                {"maxMarks", results->getInt(maxCol)},
                {"assignment1Max", results->getInt(assignment1Col)},
                {"assignment2Max", results->getInt(assignment2Col)},
                {"yearWorkMax", results->getInt(yearWorkCol)},
                {"finalExamMax", results->getInt(finalExamCol)}
            });
        }
    }

    json responseData;
    responseData["courseTypes"] = typesArray;
    return createSuccessResponse(responseData);
}

json MessageHandler::handleCreateCourseType(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string typeTable = db->resolveTable({"course_types", "course_types"});
    if (!db->tableExists(typeTable)) {
        return createErrorResponse("Course types table not found");
    }

    string id = request.value("id", "");
    string name = request.value("name", "");
    int maxMarks = request.value("maxMarks", 0);
    int assignment1Max = request.value("assignment1Max", 0);
    int assignment2Max = request.value("assignment2Max", 0);
    int yearWorkMax = request.value("yearWorkMax", 0);
    int finalExamMax = request.value("finalExamMax", 0);

    if (id.empty() || name.empty() || maxMarks <= 0) {
        return createErrorResponse("ID, name, and max marks are required");
    }

    string idCol = db->resolveColumn(typeTable, {"id", "type_id", "courseTypeId"});
    string nameCol = db->resolveColumn(typeTable, {"name", "type_name", "typeName"});
    string maxCol = db->resolveColumn(typeTable, {"max_marks", "maxMarks"});
    string assignment1Col = db->resolveColumn(typeTable, {"assignment1_max", "assignment1Max"});
    string assignment2Col = db->resolveColumn(typeTable, {"assignment2_max", "assignment2Max"});
    string yearWorkCol = db->resolveColumn(typeTable, {"year_work_max", "yearWorkMax"});
    string finalExamCol = db->resolveColumn(typeTable, {"final_exam_max", "finalExamMax"});

    vector<string> cols;
    vector<string> vals;
    cols.push_back(idCol);
    vals.push_back("'" + db->escapeString(id) + "'");
    cols.push_back(nameCol);
    vals.push_back("'" + db->escapeString(name) + "'");
    cols.push_back(maxCol);
    vals.push_back(to_string(maxMarks));
    if (db->columnExists(typeTable, assignment1Col)) {
        cols.push_back(assignment1Col);
        vals.push_back(to_string(assignment1Max));
    }
    if (db->columnExists(typeTable, assignment2Col)) {
        cols.push_back(assignment2Col);
        vals.push_back(to_string(assignment2Max));
    }
    if (db->columnExists(typeTable, yearWorkCol)) {
        cols.push_back(yearWorkCol);
        vals.push_back(to_string(yearWorkMax));
    }
    if (db->columnExists(typeTable, finalExamCol)) {
        cols.push_back(finalExamCol);
        vals.push_back(to_string(finalExamMax));
    }

    string query = "INSERT INTO " + typeTable + " (";
    for (size_t i = 0; i < cols.size(); ++i) {
        if (i > 0) query += ", ";
        query += cols[i];
    }
    query += ") VALUES (";
    for (size_t i = 0; i < vals.size(); ++i) {
        if (i > 0) query += ", ";
        query += vals[i];
    }
    query += ")";

    if (db->executeUpdate(query) > 0) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to create course type");
}

json MessageHandler::handleUpdateCourseType(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string typeTable = db->resolveTable({"course_types", "course_types"});
    if (!db->tableExists(typeTable)) {
        return createErrorResponse("Course types table not found");
    }

    string id = request.value("id", request.value("courseTypeId", ""));
    if (id.empty()) {
        return createErrorResponse("Course type ID is required");
    }

    string idCol = db->resolveColumn(typeTable, {"id", "type_id", "courseTypeId"});
    string nameCol = db->resolveColumn(typeTable, {"name", "type_name", "typeName"});
    string maxCol = db->resolveColumn(typeTable, {"max_marks", "maxMarks"});
    string assignment1Col = db->resolveColumn(typeTable, {"assignment1_max", "assignment1Max"});
    string assignment2Col = db->resolveColumn(typeTable, {"assignment2_max", "assignment2Max"});
    string yearWorkCol = db->resolveColumn(typeTable, {"year_work_max", "yearWorkMax"});
    string finalExamCol = db->resolveColumn(typeTable, {"final_exam_max", "finalExamMax"});

    vector<string> updates;
    if (request.contains("name") && db->columnExists(typeTable, nameCol)) {
        updates.push_back(nameCol + " = '" + db->escapeString(request["name"]) + "'");
    }
    if (request.contains("maxMarks") && db->columnExists(typeTable, maxCol)) {
        updates.push_back(maxCol + " = " + to_string((int)request["maxMarks"]));
    }
    if (request.contains("assignment1Max") && db->columnExists(typeTable, assignment1Col)) {
        updates.push_back(assignment1Col + " = " + to_string((int)request["assignment1Max"]));
    }
    if (request.contains("assignment2Max") && db->columnExists(typeTable, assignment2Col)) {
        updates.push_back(assignment2Col + " = " + to_string((int)request["assignment2Max"]));
    }
    if (request.contains("yearWorkMax") && db->columnExists(typeTable, yearWorkCol)) {
        updates.push_back(yearWorkCol + " = " + to_string((int)request["yearWorkMax"]));
    }
    if (request.contains("finalExamMax") && db->columnExists(typeTable, finalExamCol)) {
        updates.push_back(finalExamCol + " = " + to_string((int)request["finalExamMax"]));
    }

    if (updates.empty()) {
        return createErrorResponse("No changes provided");
    }

    string query = "UPDATE " + typeTable + " SET ";
    for (size_t i = 0; i < updates.size(); ++i) {
        if (i > 0) query += ", ";
        query += updates[i];
    }
    query += " WHERE " + idCol + " = '" + db->escapeString(id) + "'";

    if (db->executeUpdate(query) > 0) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to update course type");
}

json MessageHandler::handleDeleteCourseType(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string typeTable = db->resolveTable({"course_types", "course_types"});
    if (!db->tableExists(typeTable)) {
        return createErrorResponse("Course types table not found");
    }

    string id = request.value("id", request.value("courseTypeId", ""));
    if (id.empty()) {
        return createErrorResponse("Course type ID is required");
    }

    string idCol = db->resolveColumn(typeTable, {"id", "type_id", "courseTypeId"});
    string query = "DELETE FROM " + typeTable + " WHERE " + idCol + " = '" + db->escapeString(id) + "'";
    if (db->executeUpdate(query) > 0) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to delete course type");
}

json MessageHandler::handleCreateLectureHall(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string hallTable = db->resolveTable({"lecture_halls"});
    if (!db->tableExists(hallTable)) {
        return createErrorResponse("Lecture halls table not found");
    }

    string id = request.value("id", "");
    string name = request.value("name", "");
    string building = request.value("building", "");
    int floor = request.value("floor", 0);
    int maxCapacity = request.value("maxCapacity", 0);
    string seatingDesc = request.value("seatingDesc", "");
    int acUnits = request.value("acUnits", 0);
    int fanUnits = request.value("fanUnits", 0);
    int lightingUnits = request.value("lightingUnits", 0);
    string description = request.value("description", "");

    if (id.empty() || name.empty() || maxCapacity <= 0) {
        return createErrorResponse("ID, name, and capacity are required");
    }

    string idCol = db->resolveColumn(hallTable, {"id"});
    string nameCol = db->resolveColumn(hallTable, {"name"});
    string buildingCol = db->resolveColumn(hallTable, {"building"});
    string floorCol = db->resolveColumn(hallTable, {"floor"});
    string capacityCol = db->resolveColumn(hallTable, {"max_capacity", "maxCapacity"});
    string seatingCol = db->resolveColumn(hallTable, {"seating_desc", "seatingDesc"});
    string acCol = db->resolveColumn(hallTable, {"ac_units", "acUnits"});
    string fanCol = db->resolveColumn(hallTable, {"fan_units", "fanUnits"});
    string lightingCol = db->resolveColumn(hallTable, {"lighting_units", "lightingUnits"});
    string descCol = db->resolveColumn(hallTable, {"description"});

    vector<string> cols;
    vector<string> vals;
    cols.push_back(idCol);
    vals.push_back("'" + db->escapeString(id) + "'");
    cols.push_back(nameCol);
    vals.push_back("'" + db->escapeString(name) + "'");
    if (db->columnExists(hallTable, buildingCol)) {
        cols.push_back(buildingCol);
        vals.push_back("'" + db->escapeString(building) + "'");
    }
    if (db->columnExists(hallTable, floorCol)) {
        cols.push_back(floorCol);
        vals.push_back(to_string(floor));
    }
    cols.push_back(capacityCol);
    vals.push_back(to_string(maxCapacity));
    if (db->columnExists(hallTable, seatingCol)) {
        cols.push_back(seatingCol);
        vals.push_back("'" + db->escapeString(seatingDesc) + "'");
    }
    if (db->columnExists(hallTable, acCol)) {
        cols.push_back(acCol);
        vals.push_back(to_string(acUnits));
    }
    if (db->columnExists(hallTable, fanCol)) {
        cols.push_back(fanCol);
        vals.push_back(to_string(fanUnits));
    }
    if (db->columnExists(hallTable, lightingCol)) {
        cols.push_back(lightingCol);
        vals.push_back(to_string(lightingUnits));
    }
    if (db->columnExists(hallTable, descCol)) {
        cols.push_back(descCol);
        vals.push_back("'" + db->escapeString(description) + "'");
    }

    string query = "INSERT INTO " + hallTable + " (";
    for (size_t i = 0; i < cols.size(); ++i) {
        if (i > 0) query += ", ";
        query += cols[i];
    }
    query += ") VALUES (";
    for (size_t i = 0; i < vals.size(); ++i) {
        if (i > 0) query += ", ";
        query += vals[i];
    }
    query += ")";

    if (db->executeUpdate(query) > 0) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to create lecture hall");
}

json MessageHandler::handleUpdateLectureHall(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string hallTable = db->resolveTable({"lecture_halls"});
    if (!db->tableExists(hallTable)) {
        return createErrorResponse("Lecture halls table not found");
    }

    string id = request.value("id", "");
    if (id.empty()) {
        return createErrorResponse("Hall ID is required");
    }

    string idCol = db->resolveColumn(hallTable, {"id"});
    string nameCol = db->resolveColumn(hallTable, {"name"});
    string buildingCol = db->resolveColumn(hallTable, {"building"});
    string floorCol = db->resolveColumn(hallTable, {"floor"});
    string capacityCol = db->resolveColumn(hallTable, {"max_capacity", "maxCapacity"});
    string seatingCol = db->resolveColumn(hallTable, {"seating_desc", "seatingDesc"});
    string acCol = db->resolveColumn(hallTable, {"ac_units", "acUnits"});
    string fanCol = db->resolveColumn(hallTable, {"fan_units", "fanUnits"});
    string lightingCol = db->resolveColumn(hallTable, {"lighting_units", "lightingUnits"});
    string descCol = db->resolveColumn(hallTable, {"description"});

    vector<string> updates;
    if (request.contains("name") && db->columnExists(hallTable, nameCol)) {
        updates.push_back(nameCol + " = '" + db->escapeString(request["name"]) + "'");
    }
    if (request.contains("building") && db->columnExists(hallTable, buildingCol)) {
        updates.push_back(buildingCol + " = '" + db->escapeString(request["building"]) + "'");
    }
    if (request.contains("floor") && db->columnExists(hallTable, floorCol)) {
        updates.push_back(floorCol + " = " + to_string((int)request["floor"]));
    }
    if (request.contains("maxCapacity") && db->columnExists(hallTable, capacityCol)) {
        updates.push_back(capacityCol + " = " + to_string((int)request["maxCapacity"]));
    }
    if (request.contains("seatingDesc") && db->columnExists(hallTable, seatingCol)) {
        updates.push_back(seatingCol + " = '" + db->escapeString(request["seatingDesc"]) + "'");
    }
    if (request.contains("acUnits") && db->columnExists(hallTable, acCol)) {
        updates.push_back(acCol + " = " + to_string((int)request["acUnits"]));
    }
    if (request.contains("fanUnits") && db->columnExists(hallTable, fanCol)) {
        updates.push_back(fanCol + " = " + to_string((int)request["fanUnits"]));
    }
    if (request.contains("lightingUnits") && db->columnExists(hallTable, lightingCol)) {
        updates.push_back(lightingCol + " = " + to_string((int)request["lightingUnits"]));
    }
    if (request.contains("description") && db->columnExists(hallTable, descCol)) {
        updates.push_back(descCol + " = '" + db->escapeString(request["description"]) + "'");
    }

    if (updates.empty()) {
        return createErrorResponse("No changes provided");
    }

    string query = "UPDATE " + hallTable + " SET ";
    for (size_t i = 0; i < updates.size(); ++i) {
        if (i > 0) query += ", ";
        query += updates[i];
    }
    query += " WHERE " + idCol + " = '" + db->escapeString(id) + "'";

    if (db->executeUpdate(query) > 0) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to update lecture hall");
}

json MessageHandler::handleDeleteLectureHall(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string hallTable = db->resolveTable({"lecture_halls"});
    if (!db->tableExists(hallTable)) {
        return createErrorResponse("Lecture halls table not found");
    }

    string id = request.value("id", "");
    if (id.empty()) {
        return createErrorResponse("Hall ID is required");
    }

    string idCol = db->resolveColumn(hallTable, {"id"});
    string query = "DELETE FROM " + hallTable + " WHERE " + idCol + " = '" + db->escapeString(id) + "'";
    if (db->executeUpdate(query) > 0) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to delete lecture hall");
}

json MessageHandler::handleCreateLaboratory(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string labTable = db->resolveTable({"laboratories"});
    if (!db->tableExists(labTable)) {
        return createErrorResponse("Laboratories table not found");
    }

    string id = request.value("id", "");
    string name = request.value("name", "");
    string building = request.value("building", "");
    int floor = request.value("floor", 0);
    int maxCapacity = request.value("maxCapacity", 0);
    string labType = request.value("labType", "");
    int computersCount = request.value("computersCount", 0);
    int seatsCount = request.value("seatsCount", 0);
    int acUnits = request.value("acUnits", 0);
    int fanUnits = request.value("fanUnits", 0);
    int lightingUnits = request.value("lightingUnits", 0);
    string description = request.value("description", "");

    if (id.empty() || name.empty() || maxCapacity <= 0) {
        return createErrorResponse("ID, name, and capacity are required");
    }

    string idCol = db->resolveColumn(labTable, {"id"});
    string nameCol = db->resolveColumn(labTable, {"name"});
    string buildingCol = db->resolveColumn(labTable, {"building"});
    string floorCol = db->resolveColumn(labTable, {"floor"});
    string capacityCol = db->resolveColumn(labTable, {"max_capacity", "maxCapacity"});
    string labTypeCol = db->resolveColumn(labTable, {"lab_type", "labType"});
    string computersCol = db->resolveColumn(labTable, {"computers_count", "computersCount"});
    string seatsCol = db->resolveColumn(labTable, {"seats_count", "seatsCount"});
    string acCol = db->resolveColumn(labTable, {"ac_units", "acUnits"});
    string fanCol = db->resolveColumn(labTable, {"fan_units", "fanUnits"});
    string lightingCol = db->resolveColumn(labTable, {"lighting_units", "lightingUnits"});
    string descCol = db->resolveColumn(labTable, {"description"});

    vector<string> cols;
    vector<string> vals;
    cols.push_back(idCol);
    vals.push_back("'" + db->escapeString(id) + "'");
    cols.push_back(nameCol);
    vals.push_back("'" + db->escapeString(name) + "'");
    if (db->columnExists(labTable, buildingCol)) {
        cols.push_back(buildingCol);
        vals.push_back("'" + db->escapeString(building) + "'");
    }
    if (db->columnExists(labTable, floorCol)) {
        cols.push_back(floorCol);
        vals.push_back(to_string(floor));
    }
    cols.push_back(capacityCol);
    vals.push_back(to_string(maxCapacity));
    if (db->columnExists(labTable, labTypeCol)) {
        cols.push_back(labTypeCol);
        vals.push_back("'" + db->escapeString(labType) + "'");
    }
    if (db->columnExists(labTable, computersCol)) {
        cols.push_back(computersCol);
        vals.push_back(to_string(computersCount));
    }
    if (db->columnExists(labTable, seatsCol)) {
        cols.push_back(seatsCol);
        vals.push_back(to_string(seatsCount));
    }
    if (db->columnExists(labTable, acCol)) {
        cols.push_back(acCol);
        vals.push_back(to_string(acUnits));
    }
    if (db->columnExists(labTable, fanCol)) {
        cols.push_back(fanCol);
        vals.push_back(to_string(fanUnits));
    }
    if (db->columnExists(labTable, lightingCol)) {
        cols.push_back(lightingCol);
        vals.push_back(to_string(lightingUnits));
    }
    if (db->columnExists(labTable, descCol)) {
        cols.push_back(descCol);
        vals.push_back("'" + db->escapeString(description) + "'");
    }

    string query = "INSERT INTO " + labTable + " (";
    for (size_t i = 0; i < cols.size(); ++i) {
        if (i > 0) query += ", ";
        query += cols[i];
    }
    query += ") VALUES (";
    for (size_t i = 0; i < vals.size(); ++i) {
        if (i > 0) query += ", ";
        query += vals[i];
    }
    query += ")";

    if (db->executeUpdate(query) > 0) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to create laboratory");
}

json MessageHandler::handleUpdateLaboratory(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string labTable = db->resolveTable({"laboratories"});
    if (!db->tableExists(labTable)) {
        return createErrorResponse("Laboratories table not found");
    }

    string id = request.value("id", "");
    if (id.empty()) {
        return createErrorResponse("Lab ID is required");
    }

    string idCol = db->resolveColumn(labTable, {"id"});
    string nameCol = db->resolveColumn(labTable, {"name"});
    string buildingCol = db->resolveColumn(labTable, {"building"});
    string floorCol = db->resolveColumn(labTable, {"floor"});
    string capacityCol = db->resolveColumn(labTable, {"max_capacity", "maxCapacity"});
    string labTypeCol = db->resolveColumn(labTable, {"lab_type", "labType"});
    string computersCol = db->resolveColumn(labTable, {"computers_count", "computersCount"});
    string seatsCol = db->resolveColumn(labTable, {"seats_count", "seatsCount"});
    string acCol = db->resolveColumn(labTable, {"ac_units", "acUnits"});
    string fanCol = db->resolveColumn(labTable, {"fan_units", "fanUnits"});
    string lightingCol = db->resolveColumn(labTable, {"lighting_units", "lightingUnits"});
    string descCol = db->resolveColumn(labTable, {"description"});

    vector<string> updates;
    if (request.contains("name") && db->columnExists(labTable, nameCol)) {
        updates.push_back(nameCol + " = '" + db->escapeString(request["name"]) + "'");
    }
    if (request.contains("building") && db->columnExists(labTable, buildingCol)) {
        updates.push_back(buildingCol + " = '" + db->escapeString(request["building"]) + "'");
    }
    if (request.contains("floor") && db->columnExists(labTable, floorCol)) {
        updates.push_back(floorCol + " = " + to_string((int)request["floor"]));
    }
    if (request.contains("maxCapacity") && db->columnExists(labTable, capacityCol)) {
        updates.push_back(capacityCol + " = " + to_string((int)request["maxCapacity"]));
    }
    if (request.contains("labType") && db->columnExists(labTable, labTypeCol)) {
        updates.push_back(labTypeCol + " = '" + db->escapeString(request["labType"]) + "'");
    }
    if (request.contains("computersCount") && db->columnExists(labTable, computersCol)) {
        updates.push_back(computersCol + " = " + to_string((int)request["computersCount"]));
    }
    if (request.contains("seatsCount") && db->columnExists(labTable, seatsCol)) {
        updates.push_back(seatsCol + " = " + to_string((int)request["seatsCount"]));
    }
    if (request.contains("acUnits") && db->columnExists(labTable, acCol)) {
        updates.push_back(acCol + " = " + to_string((int)request["acUnits"]));
    }
    if (request.contains("fanUnits") && db->columnExists(labTable, fanCol)) {
        updates.push_back(fanCol + " = " + to_string((int)request["fanUnits"]));
    }
    if (request.contains("lightingUnits") && db->columnExists(labTable, lightingCol)) {
        updates.push_back(lightingCol + " = " + to_string((int)request["lightingUnits"]));
    }
    if (request.contains("description") && db->columnExists(labTable, descCol)) {
        updates.push_back(descCol + " = '" + db->escapeString(request["description"]) + "'");
    }

    if (updates.empty()) {
        return createErrorResponse("No changes provided");
    }

    string query = "UPDATE " + labTable + " SET ";
    for (size_t i = 0; i < updates.size(); ++i) {
        if (i > 0) query += ", ";
        query += updates[i];
    }
    query += " WHERE " + idCol + " = '" + db->escapeString(id) + "'";

    if (db->executeUpdate(query) > 0) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to update laboratory");
}

json MessageHandler::handleDeleteLaboratory(const json& request) {
    if (!currentUser.isValid) {
        return createErrorResponse("Not logged in");
    }
    if (!isSuperAdmin()) {
        return createErrorResponse("Access denied");
    }

    string labTable = db->resolveTable({"laboratories"});
    if (!db->tableExists(labTable)) {
        return createErrorResponse("Laboratories table not found");
    }

    string id = request.value("id", "");
    if (id.empty()) {
        return createErrorResponse("Lab ID is required");
    }

    string idCol = db->resolveColumn(labTable, {"id"});
    string query = "DELETE FROM " + labTable + " WHERE " + idCol + " = '" + db->escapeString(id) + "'";
    if (db->executeUpdate(query) > 0) {
        return createSuccessResponse();
    }
    return createErrorResponse("Failed to delete laboratory");
}
bool MessageHandler::isSuperAdmin() const {
    return currentUser.isValid && currentUser.roleId == "ROLE_SUPERADMIN";
}

bool MessageHandler::isStudentAffairs() const {
    return currentUser.isValid && currentUser.roleId == "ROLE_STUDENT_AFFAIRS";
}

bool MessageHandler::isProfessor() const {
    return currentUser.isValid && currentUser.roleId == "ROLE_PROFESSOR";
}

bool MessageHandler::isStudent() const {
    return currentUser.isValid && currentUser.roleId == "ROLE_STUDENT";
}

string MessageHandler::resolveStudentId(const json& request) const {
    string requested = request.value("studentId", "");
    if (isStudent()) {
        if (!currentUser.studentId.empty()) {
            return currentUser.studentId;
        }
        if (!currentUser.username.empty()) {
            return currentUser.username;
        }
        return requested;
    }
    if (!requested.empty()) {
        return requested;
    }
    if (!currentUser.studentId.empty()) {
        return currentUser.studentId;
    }
    if (!currentUser.username.empty()) {
        return currentUser.username;
    }
    return "";
}

bool MessageHandler::isProfessorAssignedToCourse(const string& courseId, const string& semesterId) const {
    if (!db || !db->isConnected()) {
        if (!db->connect()) {
            return false;
        }
    }
    if (currentUser.professorId.empty() || courseId.empty()) {
        return false;
    }

    string assignmentTable = db->resolveTable({"course_assignments", "professor_courses"});
    if (!db->tableExists(assignmentTable)) {
        return false;
    }

    string professorCol = db->resolveColumn(assignmentTable, {"professor_id", "professorId"});
    string courseCol = db->resolveColumn(assignmentTable, {"course_id", "courseId"});
    string semesterCol = db->resolveColumn(assignmentTable, {"semester_id", "semesterId"});

    string query = "SELECT COUNT(*) as cnt FROM " + assignmentTable + " "
                  "WHERE " + professorCol + " = '" + db->escapeString(currentUser.professorId) + "' "
                  "AND " + courseCol + " = '" + db->escapeString(courseId) + "'";
    if (!semesterId.empty()) {
        query += " AND " + semesterCol + " = '" + db->escapeString(semesterId) + "'";
    }

    auto results = db->executeQuery(query);
    if (results && results->next()) {
        return results->getInt("cnt") > 0;
    }
    return false;
}

json MessageHandler::createErrorResponse(const string& error) {
    json response;
    response["success"] = false;
    response["error"] = error;
    return response;
}

json MessageHandler::createSuccessResponse(const json& data) {
    json response;
    response["success"] = true;
    response["data"] = data;
    return response;
}
