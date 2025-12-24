#include "RegistrationRepository.h"

RegistrationRepository::RegistrationRepository(Database* database) : db(database) {
}

bool RegistrationRepository::registerStudent(const string& studentId, const string& courseId,
                                             const string& semesterId, int registeredBy) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return false;
    }

    string registrationTable = db->resolveTable({"registrations"});
    string studentCol = db->resolveColumn(registrationTable, {"student_id", "studentId"});
    string courseCol = db->resolveColumn(registrationTable, {"course_id", "courseId"});
    string semesterCol = db->resolveColumn(registrationTable, {"semester_id", "semesterId"});
    string statusCol = db->resolveColumn(registrationTable, {"status"});
    string registeredByCol = db->resolveColumn(registrationTable, {"registered_by", "registeredBy"});

    string query = "INSERT INTO " + registrationTable + " (" + studentCol + ", " + courseCol + ", " + semesterCol + ", " + statusCol + ", " + registeredByCol + ") "
                  "VALUES ('" + db->escapeString(studentId) + "', '" + db->escapeString(courseId) + "', '" +
                  db->escapeString(semesterId) + "', 'registered', " + to_string(registeredBy) + ") "
                  "ON DUPLICATE KEY UPDATE " + statusCol + " = 'registered'";

    return db->executeUpdate(query) > 0;
}

vector<Registration> RegistrationRepository::getStudentRegistrations(const string& studentId, const string& semesterId) {
    vector<Registration> registrations;

    if (!db || !db->isConnected()) {
        if (!db->connect()) return registrations;
    }

    string registrationTable = db->resolveTable({"registrations"});
    string studentCol = db->resolveColumn(registrationTable, {"student_id", "studentId"});
    string courseCol = db->resolveColumn(registrationTable, {"course_id", "courseId"});
    string semesterCol = db->resolveColumn(registrationTable, {"semester_id", "semesterId"});
    string statusCol = db->resolveColumn(registrationTable, {"status"});
    string registeredByCol = db->resolveColumn(registrationTable, {"registered_by", "registeredBy"});
    string createdAtCol = db->resolveColumn(registrationTable, {"created_at", "createdAt"});

    string studentTable = db->resolveTable({"students"});
    string courseTable = db->resolveTable({"courses"});
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
                  "WHERE r." + studentCol + " = '" + db->escapeString(studentId) + "'";

    if (!semesterId.empty()) {
        query += " AND r." + semesterCol + " = '" + db->escapeString(semesterId) + "'";
    }

    query += " ORDER BY c." + courseCodeCol;

    auto results = db->executeQuery(query);

    if (results) {
        while (results->next()) {
            Registration reg;
            reg.id = results->getInt("id");
            reg.studentId = results->getString(studentCol);
            reg.courseId = results->getString(courseCol);
            reg.semesterId = results->getString(semesterCol);
            reg.status = results->getString(statusCol);
            reg.registeredBy = results->getInt(registeredByCol);
            reg.createdAt = results->getString(createdAtCol);
            reg.studentName = results->getString("student_name");
            reg.courseName = results->getString("course_name");
            reg.courseCode = results->getString("course_code");
            registrations.push_back(reg);
        }
    }

    return registrations;
}

vector<Registration> RegistrationRepository::getCourseRegistrations(const string& courseId, const string& semesterId) {
    vector<Registration> registrations;

    if (!db || !db->isConnected()) {
        if (!db->connect()) return registrations;
    }

    string registrationTable = db->resolveTable({"registrations"});
    string studentCol = db->resolveColumn(registrationTable, {"student_id", "studentId"});
    string courseCol = db->resolveColumn(registrationTable, {"course_id", "courseId"});
    string semesterCol = db->resolveColumn(registrationTable, {"semester_id", "semesterId"});
    string statusCol = db->resolveColumn(registrationTable, {"status"});
    string registeredByCol = db->resolveColumn(registrationTable, {"registered_by", "registeredBy"});
    string createdAtCol = db->resolveColumn(registrationTable, {"created_at", "createdAt"});

    string studentTable = db->resolveTable({"students"});
    string courseTable = db->resolveTable({"courses"});
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
                  "WHERE r." + courseCol + " = '" + db->escapeString(courseId) + "' "
                  "AND r." + semesterCol + " = '" + db->escapeString(semesterId) + "' "
                  "AND r." + statusCol + " = 'registered' "
                  "ORDER BY s." + studentIdCol;

    auto results = db->executeQuery(query);

    if (results) {
        while (results->next()) {
            Registration reg;
            reg.id = results->getInt("id");
            reg.studentId = results->getString(studentCol);
            reg.courseId = results->getString(courseCol);
            reg.semesterId = results->getString(semesterCol);
            reg.status = results->getString(statusCol);
            reg.registeredBy = results->getInt(registeredByCol);
            reg.createdAt = results->getString(createdAtCol);
            reg.studentName = results->getString("student_name");
            reg.courseName = results->getString("course_name");
            reg.courseCode = results->getString("course_code");
            registrations.push_back(reg);
        }
    }

    return registrations;
}

bool RegistrationRepository::dropRegistration(int registrationId) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return false;
    }

    string registrationTable = db->resolveTable({"registrations"});
    string statusCol = db->resolveColumn(registrationTable, {"status"});
    string query = "UPDATE " + registrationTable + " SET " + statusCol + " = 'dropped' WHERE id = " + to_string(registrationId);
    return db->executeUpdate(query) > 0;
}

bool RegistrationRepository::getRegistrationById(int registrationId, Registration& registration) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return false;
    }

    string registrationTable = db->resolveTable({"registrations"});
    string studentCol = db->resolveColumn(registrationTable, {"student_id", "studentId"});
    string courseCol = db->resolveColumn(registrationTable, {"course_id", "courseId"});
    string semesterCol = db->resolveColumn(registrationTable, {"semester_id", "semesterId"});
    string statusCol = db->resolveColumn(registrationTable, {"status"});
    string registeredByCol = db->resolveColumn(registrationTable, {"registered_by", "registeredBy"});
    string createdAtCol = db->resolveColumn(registrationTable, {"created_at", "createdAt"});

    string studentTable = db->resolveTable({"students"});
    string courseTable = db->resolveTable({"courses"});
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
                  "WHERE r.id = " + to_string(registrationId);

    auto results = db->executeQuery(query);

    if (results && results->next()) {
        registration.id = results->getInt("id");
        registration.studentId = results->getString(studentCol);
        registration.courseId = results->getString(courseCol);
        registration.semesterId = results->getString(semesterCol);
        registration.status = results->getString(statusCol);
        registration.registeredBy = results->getInt(registeredByCol);
        registration.createdAt = results->getString(createdAtCol);
        registration.studentName = results->getString("student_name");
        registration.courseName = results->getString("course_name");
        registration.courseCode = results->getString("course_code");
        return true;
    }

    return false;
}

bool RegistrationRepository::updateRegistrationStatus(int registrationId, const string& status) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return false;
    }

    string registrationTable = db->resolveTable({"registrations"});
    string statusCol = db->resolveColumn(registrationTable, {"status"});
    string query = "UPDATE " + registrationTable + " SET " + statusCol + " = '" + db->escapeString(status) + "' "
                  "WHERE id = " + to_string(registrationId);
    return db->executeUpdate(query) > 0;
}

bool RegistrationRepository::deleteRegistration(int registrationId) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return false;
    }

    string registrationTable = db->resolveTable({"registrations"});
    string query = "DELETE FROM " + registrationTable + " WHERE id = " + to_string(registrationId);
    return db->executeUpdate(query) > 0;
}
