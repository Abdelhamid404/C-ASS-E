#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include <iostream>
#include <string>
#include "Database.h"

using namespace std;

struct UserInfo {
    int userId;
    string username;
    string fullName;
    string roleId;
    string studentId;      // Empty if not a student
    string professorId;    // Empty if not a professor
    bool isValid;

    UserInfo() : userId(0), isValid(false) {}
};

class AuthService {
private:
    Database* db;

public:
    AuthService(Database* database);
    
    // Login with username and password
    bool login(const string& username, const string& password, UserInfo& userInfo);
    
    // Get user info by user ID
    bool getUserInfo(int userId, UserInfo& userInfo);
    
    // Check if user has a specific permission
    bool hasPermission(int userId, const string& permissionId);
};

#endif // AUTHSERVICE_H
