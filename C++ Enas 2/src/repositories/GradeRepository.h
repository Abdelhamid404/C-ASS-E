#ifndef GRADEREPOSITORY_H
#define GRADEREPOSITORY_H

#include <iostream>
#include <string>
#include <vector>
#include "../Database.h"

using namespace std;

struct Grade {
    int id;
    string studentId;
    string courseId;
    string semesterId;
    double assignment1;
    double assignment2;
    double yearWork;
    double midterm;
    double finalExam;
    double totalMarks;
    double percentage;
    double gradePoints;
    string letterGrade;
    string evaluation;
    int updatedBy;
    string updatedAt;
    string studentName;
    string courseName;
    string courseCode;
    int maxMarks;
};

class GradeRepository {
private:
    Database* db;

public:
    GradeRepository(Database* database);
    
    bool getGrade(const string& studentId, const string& courseId, const string& semesterId, Grade& grade);
    vector<Grade> getStudentGrades(const string& studentId, const string& semesterId = "");
    vector<Grade> getCourseGrades(const string& courseId, const string& semesterId);
    bool updateGrade(const Grade& grade, int updatedBy);
    bool createOrUpdateGrade(const Grade& grade, int userId);
    bool deleteGrade(int gradeId);
    bool deleteGradeByKey(const string& studentId, const string& courseId, const string& semesterId);
};

#endif // GRADEREPOSITORY_H
