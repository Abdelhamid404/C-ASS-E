#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <iostream>
#include <string>
#include "Database.h"
#include "AuthService.h"
#include "GradeService.h"
#include "repositories/StudentRepository.h"
#include "repositories/ProfessorRepository.h"
#include "repositories/CourseRepository.h"
#include "repositories/RegistrationRepository.h"
#include "repositories/GradeRepository.h"
#include "repositories/AttendanceRepository.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

class MessageHandler {
private:
    Database* db;
    AuthService* authService;
    GradeService* gradeService;
    StudentRepository* studentRepo;
    ProfessorRepository* professorRepo;
    CourseRepository* courseRepo;
    RegistrationRepository* registrationRepo;
    GradeRepository* gradeRepo;
    AttendanceRepository* attendanceRepo;
    
    // Current user session
    UserInfo currentUser;
    
    // Handler methods
    json handleLogin(const json& request);
    json handleLogout(const json& request);
    json handleGetCurrentUser(const json& request);
    
    // Student handlers
    json handleGetStudentDashboard(const json& request);
    json handleGetStudentGrades(const json& request);
    json handleGetStudentAttendance(const json& request);
    json handleGetStudentCourses(const json& request);
    
    // Professor handlers
    json handleGetProfessorCourses(const json& request);
    json handleGetCourseStudents(const json& request);
    json handleUpdateGrade(const json& request);
    json handleRecordAttendance(const json& request);
    json handleGetCourseAttendance(const json& request);
    json handleGetCourseGrades(const json& request);

    // Professor/Facilities handlers
    json handleGetProfessors(const json& request);
    json handleGetFacilities(const json& request);
    json handleCreateProfessor(const json& request);
    json handleUpdateProfessor(const json& request);
    json handleDeleteProfessor(const json& request);
    json handleCreateCourse(const json& request);
    json handleUpdateCourse(const json& request);
    json handleDeleteCourse(const json& request);
    json handleDeleteStudent(const json& request);
    json handleDeleteGrade(const json& request);
    json handleUpdateAttendanceRecord(const json& request);
    json handleDeleteAttendance(const json& request);
    json handleGetRegistrations(const json& request);
    json handleUpdateRegistration(const json& request);
    json handleDeleteRegistration(const json& request);
    json handleGetCourseAssignments(const json& request);
    json handleCreateCourseAssignment(const json& request);
    json handleDeleteCourseAssignment(const json& request);
    json handleGetDepartments(const json& request);
    json handleCreateDepartment(const json& request);
    json handleUpdateDepartment(const json& request);
    json handleDeleteDepartment(const json& request);
    json handleGetColleges(const json& request);
    json handleCreateCollege(const json& request);
    json handleUpdateCollege(const json& request);
    json handleDeleteCollege(const json& request);
    json handleGetAcademicLevels(const json& request);
    json handleCreateAcademicLevel(const json& request);
    json handleUpdateAcademicLevel(const json& request);
    json handleDeleteAcademicLevel(const json& request);
    json handleGetSemesters(const json& request);
    json handleCreateSemester(const json& request);
    json handleUpdateSemester(const json& request);
    json handleDeleteSemester(const json& request);
    json handleGetCourseTypes(const json& request);
    json handleCreateCourseType(const json& request);
    json handleUpdateCourseType(const json& request);
    json handleDeleteCourseType(const json& request);
    json handleCreateLectureHall(const json& request);
    json handleUpdateLectureHall(const json& request);
    json handleDeleteLectureHall(const json& request);
    json handleCreateLaboratory(const json& request);
    json handleUpdateLaboratory(const json& request);
    json handleDeleteLaboratory(const json& request);
    
    // Admin handlers
    json handleGetStudents(const json& request);
    json handleCreateStudent(const json& request);
    json handleUpdateStudent(const json& request);
    json handleGetCourses(const json& request);
    json handleRegisterStudentToCourse(const json& request);

    // Helpers
    string getCurrentSemesterId();
    double calculateAttendanceRate(const string& studentId, const string& semesterId);
    void ensureSessionRecord(const string& courseId, const string& semesterId,
                             const string& sessionDate, const string& sessionType);
    bool isSuperAdmin() const;
    bool isStudentAffairs() const;
    bool isProfessor() const;
    bool isStudent() const;
    bool isProfessorAssignedToCourse(const string& courseId, const string& semesterId) const;
    string resolveStudentId(const json& request) const;
    
    // Utility methods
    json createErrorResponse(const string& error);
    json createSuccessResponse(const json& data = json::object());

public:
    MessageHandler();
    ~MessageHandler();
    
    string handleMessage(const string& messageJson);
};

#endif // MESSAGEHANDLER_H
