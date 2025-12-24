#ifndef REGISTRATIONREPOSITORY_H
#define REGISTRATIONREPOSITORY_H

#include <iostream>
#include <string>
#include <vector>
#include "../Database.h"

using namespace std;

struct Registration {
    int id;
    string studentId;
    string courseId;
    string semesterId;
    string status;
    int registeredBy;
    string createdAt;
    string studentName;
    string courseName;
    string courseCode;
};

class RegistrationRepository {
private:
    Database* db;

public:
    RegistrationRepository(Database* database);
    
    bool registerStudent(const string& studentId, const string& courseId, const string& semesterId, int registeredBy);
    vector<Registration> getStudentRegistrations(const string& studentId, const string& semesterId = "");
    vector<Registration> getCourseRegistrations(const string& courseId, const string& semesterId);
    bool dropRegistration(int registrationId);
    bool getRegistrationById(int registrationId, Registration& registration);
    bool updateRegistrationStatus(int registrationId, const string& status);
    bool deleteRegistration(int registrationId);
};

#endif // REGISTRATIONREPOSITORY_H
