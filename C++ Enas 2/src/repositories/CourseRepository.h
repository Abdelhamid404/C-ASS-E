#ifndef COURSEREPOSITORY_H
#define COURSEREPOSITORY_H

#include <iostream>
#include <string>
#include <vector>
#include "../Database.h"

using namespace std;

struct Course {
    string id;
    string code;
    string nameAr;
    string nameEn;
    string description;
    string departmentId;
    string academicLevelId;
    int semesterNumber;
    string courseTypeId;
    int creditHours;
    int lectureHours;
    int tutorialHours;
    int labHours;
    int practicalHours;
    int maxMarks;
    int assignment1Marks;
    int assignment2Marks;
    int yearWorkMarks;
    int finalExamMarks;
};

class CourseRepository {
private:
    Database* db;

public:
    CourseRepository(Database* database);
    
    bool getCourseById(const string& courseId, Course& course);
    vector<Course> listByDepartment(const string& departmentId);
    vector<Course> listByDepartmentLevel(const string& departmentId, const string& levelId);
    vector<Course> listBySemester(const string& semesterId);
    vector<Course> listAll();
    bool createCourse(const Course& course);
    bool updateCourse(const string& courseId, const Course& course);
    bool deleteCourse(const string& courseId);
};

#endif // COURSEREPOSITORY_H
