#include "AuthService.h"

AuthService::AuthService(Database* database) : db(database) {
}

bool AuthService::login(const string& username, const string& password, UserInfo& userInfo) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) {
            return false;
        }
    }

    string userTable = db->resolveTable({"users"});
    string studentTable = db->resolveTable({"students"});
    string professorTable = db->resolveTable({"professors"});
    string userIdCol = db->resolveColumn(userTable, {"id", "user_id", "userId"});
    string usernameCol = db->resolveColumn(userTable, {"username", "user_name", "userName"});
    string passwordCol = db->resolveColumn(userTable, {"password_hash", "password", "passwordHash"});
    string fullNameCol = db->resolveColumn(userTable, {"full_name", "fullName", "name"});
    string roleCol = db->resolveColumn(userTable, {"role_id", "roleId"});
    string activeCol = db->resolveColumn(userTable, {"is_active", "active", "isActive"});
    string lastLoginCol = db->resolveColumn(userTable, {"last_login", "lastLogin"});

    string studentIdCol = db->resolveColumn(studentTable, {"id", "student_id", "studentId"});
    string studentUserIdCol = db->resolveColumn(studentTable, {"user_id", "userId"});
    string professorIdCol = db->resolveColumn(professorTable, {"id", "professor_id", "professorId"});
    string professorUserIdCol = db->resolveColumn(professorTable, {"user_id", "userId"});

    string query = "SELECT u." + userIdCol + " as user_id, "
                  "u." + usernameCol + " as username, "
                  "u." + fullNameCol + " as full_name, "
                  "u." + roleCol + " as role_id, "
                  "s." + studentIdCol + " as student_id, "
                  "p." + professorIdCol + " as professor_id "
                  "FROM " + userTable + " u "
                  "LEFT JOIN " + studentTable + " s ON u." + userIdCol + " = s." + studentUserIdCol + " "
                  "LEFT JOIN " + professorTable + " p ON u." + userIdCol + " = p." + professorUserIdCol + " "
                  "WHERE u." + usernameCol + " = '" + db->escapeString(username) + "' "
                  "AND u." + passwordCol + " = '" + db->escapeString(password) + "'";
    if (db->columnExists(userTable, activeCol)) {
        query += " AND u." + activeCol + " = TRUE";
    }

    auto results = db->executeQuery(query);
    
    if (results && results->next()) {
        userInfo.userId = results->getInt("user_id");
        userInfo.username = results->getString("username");
        userInfo.fullName = results->getString("full_name");
        userInfo.roleId = results->getString("role_id");
        userInfo.studentId = results->getString("student_id");
        userInfo.professorId = results->getString("professor_id");
        userInfo.isValid = true;
        
        // Update last login
        if (db->columnExists(userTable, lastLoginCol)) {
            string updateQuery = "UPDATE " + userTable + " SET " + lastLoginCol + " = NOW() "
                                 "WHERE " + userIdCol + " = " + to_string(userInfo.userId);
            db->executeUpdate(updateQuery);
        }
        
        return true;
    }

    userInfo.isValid = false;
    return false;
}

bool AuthService::getUserInfo(int userId, UserInfo& userInfo) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) {
            return false;
        }
    }

    string userTable = db->resolveTable({"users"});
    string studentTable = db->resolveTable({"students"});
    string professorTable = db->resolveTable({"professors"});
    string userIdCol = db->resolveColumn(userTable, {"id", "user_id", "userId"});
    string usernameCol = db->resolveColumn(userTable, {"username", "user_name", "userName"});
    string fullNameCol = db->resolveColumn(userTable, {"full_name", "fullName", "name"});
    string roleCol = db->resolveColumn(userTable, {"role_id", "roleId"});
    string activeCol = db->resolveColumn(userTable, {"is_active", "active", "isActive"});
    string studentIdCol = db->resolveColumn(studentTable, {"id", "student_id", "studentId"});
    string studentUserIdCol = db->resolveColumn(studentTable, {"user_id", "userId"});
    string professorIdCol = db->resolveColumn(professorTable, {"id", "professor_id", "professorId"});
    string professorUserIdCol = db->resolveColumn(professorTable, {"user_id", "userId"});

    string query = "SELECT u." + userIdCol + " as user_id, "
                  "u." + usernameCol + " as username, "
                  "u." + fullNameCol + " as full_name, "
                  "u." + roleCol + " as role_id, "
                  "s." + studentIdCol + " as student_id, "
                  "p." + professorIdCol + " as professor_id "
                  "FROM " + userTable + " u "
                  "LEFT JOIN " + studentTable + " s ON u." + userIdCol + " = s." + studentUserIdCol + " "
                  "LEFT JOIN " + professorTable + " p ON u." + userIdCol + " = p." + professorUserIdCol + " "
                  "WHERE u." + userIdCol + " = " + to_string(userId);
    if (db->columnExists(userTable, activeCol)) {
        query += " AND u." + activeCol + " = TRUE";
    }

    auto results = db->executeQuery(query);
    
    if (results && results->next()) {
        userInfo.userId = results->getInt("user_id");
        userInfo.username = results->getString("username");
        userInfo.fullName = results->getString("full_name");
        userInfo.roleId = results->getString("role_id");
        userInfo.studentId = results->getString("student_id");
        userInfo.professorId = results->getString("professor_id");
        userInfo.isValid = true;
        return true;
    }

    userInfo.isValid = false;
    return false;
}

bool AuthService::hasPermission(int userId, const string& permissionId) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) {
            return false;
        }
    }

    string query = "SELECT COUNT(*) as cnt FROM ("
                  "SELECT rp.permission_id FROM users u "
                  "JOIN role_permissions rp ON u.role_id = rp.role_id "
                  "WHERE u.id = " + to_string(userId) + " "
                  "AND rp.permission_id = '" + db->escapeString(permissionId) + "' "
                  "UNION "
                  "SELECT up.permission_id FROM user_permissions up "
                  "WHERE up.user_id = " + to_string(userId) + " "
                  "AND up.permission_id = '" + db->escapeString(permissionId) + "' "
                  "AND up.is_granted = TRUE"
                  ") AS perms";

    auto results = db->executeQuery(query);
    
    if (results && results->next()) {
        return results->getInt("cnt") > 0;
    }

    return false;
}
