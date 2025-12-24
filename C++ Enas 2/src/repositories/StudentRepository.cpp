#include "StudentRepository.h"

namespace {
Student buildStudent(ResultSet* results,
                     const std::string& idCol,
                     const std::string& userIdCol,
                     const std::string& firstNameCol,
                     const std::string& lastNameCol,
                     const std::string& dobCol,
                     const std::string& emailCol,
                     const std::string& phoneCol,
                     const std::string& genderCol,
                     const std::string& departmentCol,
                     const std::string& levelCol,
                     const std::string& enrollmentCol,
                     const std::string& statusCol) {
    Student student;
    student.id = results->getString(idCol);
    student.userId = results->getString("user_id").empty() ? 0 : std::stoi(results->getString("user_id"));
    student.firstName = results->getString(firstNameCol);
    student.lastName = results->getString(lastNameCol);
    student.dateOfBirth = results->getString(dobCol);
    student.email = results->getString(emailCol);
    student.phone = results->getString(phoneCol);
    student.gender = results->getString(genderCol);
    student.departmentId = results->getString(departmentCol);
    student.academicLevelId = results->getString(levelCol);
    student.enrollmentDate = results->getString(enrollmentCol);
    student.status = results->getString(statusCol);
    student.fullName = results->getString("full_name");
    return student;
}
}

StudentRepository::StudentRepository(Database* database) : db(database) {
}

bool StudentRepository::getStudentById(const string& studentId, Student& student) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return false;
    }

    string studentTable = db->resolveTable({"students"});
    string userTable = db->resolveTable({"users"});
    string idCol = db->resolveColumn(studentTable, {"id", "student_id", "studentId"});
    string userIdCol = db->resolveColumn(studentTable, {"user_id", "userId"});
    string firstNameCol = db->resolveColumn(studentTable, {"first_name", "firstName"});
    string lastNameCol = db->resolveColumn(studentTable, {"last_name", "lastName"});
    string dobCol = db->resolveColumn(studentTable, {"date_of_birth", "dateOfBirth"});
    string emailCol = db->resolveColumn(studentTable, {"email"});
    string phoneCol = db->resolveColumn(studentTable, {"phone"});
    string genderCol = db->resolveColumn(studentTable, {"gender"});
    string departmentCol = db->resolveColumn(studentTable, {"department_id", "departmentId"});
    string levelCol = db->resolveColumn(studentTable, {"academic_level_id", "academicLevelId", "level_id", "levelId"});
    string enrollmentCol = db->resolveColumn(studentTable, {"enrollment_date", "enrollmentDate"});
    string statusCol = db->resolveColumn(studentTable, {"status"});
    string userPkCol = db->resolveColumn(userTable, {"id", "user_id", "userId"});

    string query = "SELECT s.*, u." + userPkCol + " as user_id, CONCAT(s." + firstNameCol + ", ' ', s." + lastNameCol + ") as full_name "
                  "FROM " + studentTable + " s "
                  "LEFT JOIN " + userTable + " u ON s." + userIdCol + " = u." + userPkCol + " "
                  "WHERE s." + idCol + " = '" + db->escapeString(studentId) + "'";

    auto results = db->executeQuery(query);

    if (results && results->next()) {
        student = buildStudent(results.get(), idCol, userIdCol, firstNameCol, lastNameCol, dobCol,
                               emailCol, phoneCol, genderCol, departmentCol, levelCol, enrollmentCol, statusCol);
        return true;
    }

    return false;
}

vector<Student> StudentRepository::listByDepartmentLevel(const string& departmentId, const string& levelId) {
    vector<Student> students;

    if (!db || !db->isConnected()) {
        if (!db->connect()) return students;
    }

    string studentTable = db->resolveTable({"students"});
    string userTable = db->resolveTable({"users"});
    string idCol = db->resolveColumn(studentTable, {"id", "student_id", "studentId"});
    string userIdCol = db->resolveColumn(studentTable, {"user_id", "userId"});
    string firstNameCol = db->resolveColumn(studentTable, {"first_name", "firstName"});
    string lastNameCol = db->resolveColumn(studentTable, {"last_name", "lastName"});
    string dobCol = db->resolveColumn(studentTable, {"date_of_birth", "dateOfBirth"});
    string emailCol = db->resolveColumn(studentTable, {"email"});
    string phoneCol = db->resolveColumn(studentTable, {"phone"});
    string genderCol = db->resolveColumn(studentTable, {"gender"});
    string departmentCol = db->resolveColumn(studentTable, {"department_id", "departmentId"});
    string levelCol = db->resolveColumn(studentTable, {"academic_level_id", "academicLevelId", "level_id", "levelId"});
    string enrollmentCol = db->resolveColumn(studentTable, {"enrollment_date", "enrollmentDate"});
    string statusCol = db->resolveColumn(studentTable, {"status"});
    string userPkCol = db->resolveColumn(userTable, {"id", "user_id", "userId"});

    string query = "SELECT s.*, u." + userPkCol + " as user_id, CONCAT(s." + firstNameCol + ", ' ', s." + lastNameCol + ") as full_name "
                  "FROM " + studentTable + " s "
                  "LEFT JOIN " + userTable + " u ON s." + userIdCol + " = u." + userPkCol + " "
                  "WHERE s." + departmentCol + " = '" + db->escapeString(departmentId) + "' "
                  "AND s." + levelCol + " = '" + db->escapeString(levelId) + "' "
                  "AND s." + statusCol + " = 'active' "
                  "ORDER BY s." + idCol;

    auto results = db->executeQuery(query);

    if (results) {
        while (results->next()) {
            students.push_back(buildStudent(results.get(), idCol, userIdCol, firstNameCol, lastNameCol, dobCol,
                                            emailCol, phoneCol, genderCol, departmentCol, levelCol, enrollmentCol, statusCol));
        }
    }

    return students;
}

vector<Student> StudentRepository::listAll() {
    vector<Student> students;

    if (!db || !db->isConnected()) {
        if (!db->connect()) return students;
    }

    string studentTable = db->resolveTable({"students"});
    string userTable = db->resolveTable({"users"});
    string idCol = db->resolveColumn(studentTable, {"id", "student_id", "studentId"});
    string userIdCol = db->resolveColumn(studentTable, {"user_id", "userId"});
    string firstNameCol = db->resolveColumn(studentTable, {"first_name", "firstName"});
    string lastNameCol = db->resolveColumn(studentTable, {"last_name", "lastName"});
    string dobCol = db->resolveColumn(studentTable, {"date_of_birth", "dateOfBirth"});
    string emailCol = db->resolveColumn(studentTable, {"email"});
    string phoneCol = db->resolveColumn(studentTable, {"phone"});
    string genderCol = db->resolveColumn(studentTable, {"gender"});
    string departmentCol = db->resolveColumn(studentTable, {"department_id", "departmentId"});
    string levelCol = db->resolveColumn(studentTable, {"academic_level_id", "academicLevelId", "level_id", "levelId"});
    string enrollmentCol = db->resolveColumn(studentTable, {"enrollment_date", "enrollmentDate"});
    string statusCol = db->resolveColumn(studentTable, {"status"});
    string userPkCol = db->resolveColumn(userTable, {"id", "user_id", "userId"});

    string query = "SELECT s.*, u." + userPkCol + " as user_id, CONCAT(s." + firstNameCol + ", ' ', s." + lastNameCol + ") as full_name "
                  "FROM " + studentTable + " s "
                  "LEFT JOIN " + userTable + " u ON s." + userIdCol + " = u." + userPkCol + " "
                  "ORDER BY s." + idCol;

    auto results = db->executeQuery(query);

    if (results) {
        while (results->next()) {
            students.push_back(buildStudent(results.get(), idCol, userIdCol, firstNameCol, lastNameCol, dobCol,
                                            emailCol, phoneCol, genderCol, departmentCol, levelCol, enrollmentCol, statusCol));
        }
    }

    return students;
}

bool StudentRepository::createStudent(const Student& student, int createdBy) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return false;
    }

    string userTable = db->resolveTable({"users"});
    string studentTable = db->resolveTable({"students"});

    string usernameCol = db->resolveColumn(userTable, {"username", "user_name", "userName"});
    string passwordCol = db->resolveColumn(userTable, {"password_hash", "password", "passwordHash"});
    string fullNameCol = db->resolveColumn(userTable, {"full_name", "fullName", "name"});
    string emailCol = db->resolveColumn(userTable, {"email"});
    string phoneCol = db->resolveColumn(userTable, {"phone"});
    string roleCol = db->resolveColumn(userTable, {"role_id", "roleId"});
    string activeCol = db->resolveColumn(userTable, {"is_active", "active", "isActive"});

    vector<string> userCols;
    vector<string> userVals;
    userCols.push_back(usernameCol);
    userVals.push_back("'" + db->escapeString(student.id) + "'");
    userCols.push_back(passwordCol);
    userVals.push_back("'student123'");
    userCols.push_back(fullNameCol);
    userVals.push_back("'" + db->escapeString(student.firstName + " " + student.lastName) + "'");
    userCols.push_back(roleCol);
    userVals.push_back("'ROLE_STUDENT'");
    if (db->columnExists(userTable, emailCol)) {
        userCols.push_back(emailCol);
        userVals.push_back("'" + db->escapeString(student.email) + "'");
    }
    if (db->columnExists(userTable, phoneCol)) {
        userCols.push_back(phoneCol);
        userVals.push_back("'" + db->escapeString(student.phone) + "'");
    }
    if (db->columnExists(userTable, activeCol)) {
        userCols.push_back(activeCol);
        userVals.push_back("TRUE");
    }

    string userQuery = "INSERT INTO " + userTable + " (";
    for (size_t i = 0; i < userCols.size(); ++i) {
        if (i > 0) userQuery += ", ";
        userQuery += userCols[i];
    }
    userQuery += ") VALUES (";
    for (size_t i = 0; i < userVals.size(); ++i) {
        if (i > 0) userQuery += ", ";
        userQuery += userVals[i];
    }
    userQuery += ")";

    db->executeUpdate(userQuery);
    int userId = (int)db->getLastInsertId();

    string studentIdCol = db->resolveColumn(studentTable, {"id", "student_id", "studentId"});
    string studentUserIdCol = db->resolveColumn(studentTable, {"user_id", "userId"});
    string firstNameCol = db->resolveColumn(studentTable, {"first_name", "firstName"});
    string lastNameCol = db->resolveColumn(studentTable, {"last_name", "lastName"});
    string dobCol = db->resolveColumn(studentTable, {"date_of_birth", "dateOfBirth"});
    string studentEmailCol = db->resolveColumn(studentTable, {"email"});
    string studentPhoneCol = db->resolveColumn(studentTable, {"phone"});
    string genderCol = db->resolveColumn(studentTable, {"gender"});
    string departmentCol = db->resolveColumn(studentTable, {"department_id", "departmentId"});
    string levelCol = db->resolveColumn(studentTable, {"academic_level_id", "academicLevelId", "level_id", "levelId"});
    string enrollmentCol = db->resolveColumn(studentTable, {"enrollment_date", "enrollmentDate"});
    string statusCol = db->resolveColumn(studentTable, {"status"});

    vector<string> studentCols;
    vector<string> studentVals;
    studentCols.push_back(studentIdCol);
    studentVals.push_back("'" + db->escapeString(student.id) + "'");
    if (db->columnExists(studentTable, studentUserIdCol)) {
        studentCols.push_back(studentUserIdCol);
        studentVals.push_back(to_string(userId));
    }
    studentCols.push_back(firstNameCol);
    studentVals.push_back("'" + db->escapeString(student.firstName) + "'");
    studentCols.push_back(lastNameCol);
    studentVals.push_back("'" + db->escapeString(student.lastName) + "'");
    if (db->columnExists(studentTable, dobCol)) {
        studentCols.push_back(dobCol);
        if (student.dateOfBirth.empty()) {
            studentVals.push_back("NULL");
        } else {
            studentVals.push_back("'" + db->escapeString(student.dateOfBirth) + "'");
        }
    }
    if (db->columnExists(studentTable, studentEmailCol)) {
        studentCols.push_back(studentEmailCol);
        studentVals.push_back("'" + db->escapeString(student.email) + "'");
    }
    if (db->columnExists(studentTable, studentPhoneCol)) {
        studentCols.push_back(studentPhoneCol);
        studentVals.push_back("'" + db->escapeString(student.phone) + "'");
    }
    if (db->columnExists(studentTable, genderCol)) {
        studentCols.push_back(genderCol);
        studentVals.push_back("'" + db->escapeString(student.gender) + "'");
    }
    if (db->columnExists(studentTable, departmentCol)) {
        studentCols.push_back(departmentCol);
        studentVals.push_back("'" + db->escapeString(student.departmentId) + "'");
    }
    if (db->columnExists(studentTable, levelCol)) {
        studentCols.push_back(levelCol);
        studentVals.push_back("'" + db->escapeString(student.academicLevelId) + "'");
    }
    if (db->columnExists(studentTable, enrollmentCol)) {
        studentCols.push_back(enrollmentCol);
        studentVals.push_back(student.enrollmentDate.empty() ? "CURRENT_DATE" : "'" + db->escapeString(student.enrollmentDate) + "'");
    }
    if (db->columnExists(studentTable, statusCol)) {
        studentCols.push_back(statusCol);
        studentVals.push_back("'active'");
    }

    string studentQuery = "INSERT INTO " + studentTable + " (";
    for (size_t i = 0; i < studentCols.size(); ++i) {
        if (i > 0) studentQuery += ", ";
        studentQuery += studentCols[i];
    }
    studentQuery += ") VALUES (";
    for (size_t i = 0; i < studentVals.size(); ++i) {
        if (i > 0) studentQuery += ", ";
        studentQuery += studentVals[i];
    }
    studentQuery += ")";

    return db->executeUpdate(studentQuery) > 0;
}

bool StudentRepository::updateStudent(const string& studentId, const Student& student, int updatedBy) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return false;
    }

    string studentTable = db->resolveTable({"students"});
    string idCol = db->resolveColumn(studentTable, {"id", "student_id", "studentId"});
    string firstNameCol = db->resolveColumn(studentTable, {"first_name", "firstName"});
    string lastNameCol = db->resolveColumn(studentTable, {"last_name", "lastName"});
    string dobCol = db->resolveColumn(studentTable, {"date_of_birth", "dateOfBirth"});
    string emailCol = db->resolveColumn(studentTable, {"email"});
    string phoneCol = db->resolveColumn(studentTable, {"phone"});
    string genderCol = db->resolveColumn(studentTable, {"gender"});
    string departmentCol = db->resolveColumn(studentTable, {"department_id", "departmentId"});
    string levelCol = db->resolveColumn(studentTable, {"academic_level_id", "academicLevelId", "level_id", "levelId"});
    string statusCol = db->resolveColumn(studentTable, {"status"});

    vector<string> updates;
    if (db->columnExists(studentTable, firstNameCol)) {
        updates.push_back(firstNameCol + " = '" + db->escapeString(student.firstName) + "'");
    }
    if (db->columnExists(studentTable, lastNameCol)) {
        updates.push_back(lastNameCol + " = '" + db->escapeString(student.lastName) + "'");
    }
    if (db->columnExists(studentTable, dobCol)) {
        updates.push_back(dobCol + " = " + (student.dateOfBirth.empty() ? "NULL" : "'" + db->escapeString(student.dateOfBirth) + "'"));
    }
    if (db->columnExists(studentTable, emailCol)) {
        updates.push_back(emailCol + " = '" + db->escapeString(student.email) + "'");
    }
    if (db->columnExists(studentTable, phoneCol)) {
        updates.push_back(phoneCol + " = '" + db->escapeString(student.phone) + "'");
    }
    if (db->columnExists(studentTable, genderCol)) {
        updates.push_back(genderCol + " = '" + db->escapeString(student.gender) + "'");
    }
    if (db->columnExists(studentTable, departmentCol)) {
        updates.push_back(departmentCol + " = '" + db->escapeString(student.departmentId) + "'");
    }
    if (db->columnExists(studentTable, levelCol)) {
        updates.push_back(levelCol + " = '" + db->escapeString(student.academicLevelId) + "'");
    }
    if (db->columnExists(studentTable, statusCol)) {
        updates.push_back(statusCol + " = '" + db->escapeString(student.status) + "'");
    }

    if (updates.empty()) {
        return false;
    }

    string query = "UPDATE " + studentTable + " SET ";
    for (size_t i = 0; i < updates.size(); ++i) {
        if (i > 0) query += ", ";
        query += updates[i];
    }
    query += " WHERE " + idCol + " = '" + db->escapeString(studentId) + "'";

    return db->executeUpdate(query) > 0;
}

bool StudentRepository::deactivateStudent(const string& studentId) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return false;
    }

    string studentTable = db->resolveTable({"students"});
    string idCol = db->resolveColumn(studentTable, {"id", "student_id", "studentId"});
    string statusCol = db->resolveColumn(studentTable, {"status"});
    if (!db->columnExists(studentTable, statusCol)) {
        return false;
    }

    string query = "UPDATE " + studentTable + " SET " + statusCol + " = 'inactive' WHERE " + idCol + " = '" + db->escapeString(studentId) + "'";
    return db->executeUpdate(query) > 0;
}
