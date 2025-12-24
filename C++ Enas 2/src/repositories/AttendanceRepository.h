#ifndef ATTENDANCEREPOSITORY_H
#define ATTENDANCEREPOSITORY_H

#include <iostream>
#include <string>
#include <vector>
#include "../Database.h"

using namespace std;

struct Attendance {
    int id;
    string studentId;
    string courseId;
    string semesterId;
    string date;
    string status;
    string notes;
    int recordedBy;
    string createdAt;
    string studentName;
    string courseName;
    string courseCode;
};

class AttendanceRepository {
private:
    Database* db;

public:
    AttendanceRepository(Database* database);
    
    bool recordAttendance(const string& studentId, const string& courseId, const string& semesterId,
                         const string& date, const string& status, const string& notes, int recordedBy);
    vector<Attendance> getStudentAttendance(const string& studentId, const string& courseId = "", 
                                           const string& semesterId = "");
    vector<Attendance> getCourseAttendance(const string& courseId, const string& semesterId, 
                                          const string& date = "");
    int getAttendanceCount(const string& studentId, const string& courseId, const string& semesterId,
                          const string& status);
    bool getAttendanceById(int attendanceId, Attendance& attendance);
    bool updateAttendance(int attendanceId, const string& status, const string& notes, int recordedBy);
    bool deleteAttendance(int attendanceId);
};

#endif // ATTENDANCEREPOSITORY_H
