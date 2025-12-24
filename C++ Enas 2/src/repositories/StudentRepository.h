#ifndef STUDENTREPOSITORY_H
#define STUDENTREPOSITORY_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "../Database.h"

using namespace std;

struct Student {
    string id;
    int userId;
    string firstName;
    string lastName;
    string dateOfBirth;
    string email;
    string phone;
    string gender;
    string departmentId;
    string academicLevelId;
    string enrollmentDate;
    string status;
    string fullName;
};

class StudentRepository {
private:
    Database* db;

public:
    StudentRepository(Database* database);
    
    bool getStudentById(const string& studentId, Student& student);
    vector<Student> listByDepartmentLevel(const string& departmentId, const string& levelId);
    vector<Student> listAll();
    bool createStudent(const Student& student, int createdBy);
    bool updateStudent(const string& studentId, const Student& student, int updatedBy);
    bool deactivateStudent(const string& studentId);
};

#endif // STUDENTREPOSITORY_H
