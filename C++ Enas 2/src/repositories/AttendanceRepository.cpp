#include "AttendanceRepository.h"

AttendanceRepository::AttendanceRepository(Database* database) : db(database) {
}

bool AttendanceRepository::recordAttendance(const string& studentId, const string& courseId,
                                            const string& semesterId, const string& date,
                                            const string& status, const string& notes, int recordedBy) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return false;
    }

    string attendanceTable = db->resolveTable({"attendance"});
    string studentCol = db->resolveColumn(attendanceTable, {"student_id", "studentId"});
    string courseCol = db->resolveColumn(attendanceTable, {"course_id", "courseId"});
    string semesterCol = db->resolveColumn(attendanceTable, {"semester_id", "semesterId"});
    string dateCol = db->resolveColumn(attendanceTable, {"date", "session_date", "attendance_date"});
    string statusCol = db->resolveColumn(attendanceTable, {"status"});
    string notesCol = db->resolveColumn(attendanceTable, {"notes", "remarks"});
    string recordedByCol = db->resolveColumn(attendanceTable, {"recorded_by", "recordedBy"});

    string query = "INSERT INTO " + attendanceTable + " (" + studentCol + ", " + courseCol + ", " + semesterCol + ", "
                  + dateCol + ", " + statusCol + ", " + notesCol + ", " + recordedByCol + ") "
                  "VALUES ('" + db->escapeString(studentId) + "', '" + db->escapeString(courseId) + "', '" +
                  db->escapeString(semesterId) + "', '" + db->escapeString(date) + "', '" +
                  db->escapeString(status) + "', '" + db->escapeString(notes) + "', " +
                  to_string(recordedBy) + ") "
                  "ON DUPLICATE KEY UPDATE " + statusCol + " = '" + db->escapeString(status) + "', "
                  + notesCol + " = '" + db->escapeString(notes) + "', " + recordedByCol + " = " + to_string(recordedBy);

    return db->executeUpdate(query) >= 0;
}

vector<Attendance> AttendanceRepository::getStudentAttendance(const string& studentId,
                                                              const string& courseId,
                                                              const string& semesterId) {
    vector<Attendance> attendances;

    if (!db || !db->isConnected()) {
        if (!db->connect()) return attendances;
    }

    string attendanceTable = db->resolveTable({"attendance"});
    string studentCol = db->resolveColumn(attendanceTable, {"student_id", "studentId"});
    string courseCol = db->resolveColumn(attendanceTable, {"course_id", "courseId"});
    string semesterCol = db->resolveColumn(attendanceTable, {"semester_id", "semesterId"});
    string dateCol = db->resolveColumn(attendanceTable, {"date", "session_date", "attendance_date"});
    string statusCol = db->resolveColumn(attendanceTable, {"status"});
    string notesCol = db->resolveColumn(attendanceTable, {"notes", "remarks"});
    string recordedByCol = db->resolveColumn(attendanceTable, {"recorded_by", "recordedBy"});
    string createdAtCol = db->resolveColumn(attendanceTable, {"created_at", "createdAt"});

    string studentTable = db->resolveTable({"students"});
    string courseTable = db->resolveTable({"courses"});
    string studentIdCol = db->resolveColumn(studentTable, {"id", "student_id", "studentId"});
    string courseIdCol = db->resolveColumn(courseTable, {"id", "course_id", "courseId"});
    string studentFirstCol = db->resolveColumn(studentTable, {"first_name", "firstName"});
    string studentLastCol = db->resolveColumn(studentTable, {"last_name", "lastName"});
    string courseNameCol = db->resolveColumn(courseTable, {"name_en", "nameEn", "name"});
    string courseCodeCol = db->resolveColumn(courseTable, {"code", "course_code", "courseCode"});

    string query = "SELECT a.*, CONCAT(s." + studentFirstCol + ", ' ', s." + studentLastCol + ") as student_name, "
                  "c." + courseNameCol + " as course_name, c." + courseCodeCol + " as course_code "
                  "FROM " + attendanceTable + " a "
                  "JOIN " + studentTable + " s ON a." + studentCol + " = s." + studentIdCol + " "
                  "JOIN " + courseTable + " c ON a." + courseCol + " = c." + courseIdCol + " "
                  "WHERE a." + studentCol + " = '" + db->escapeString(studentId) + "'";

    if (!courseId.empty()) {
        query += " AND a." + courseCol + " = '" + db->escapeString(courseId) + "'";
    }
    if (!semesterId.empty()) {
        query += " AND a." + semesterCol + " = '" + db->escapeString(semesterId) + "'";
    }

    query += " ORDER BY a." + dateCol + " DESC, c." + courseCodeCol;

    auto results = db->executeQuery(query);

    if (results) {
        while (results->next()) {
            Attendance att;
            att.id = results->getInt("id");
            att.studentId = results->getString(studentCol);
            att.courseId = results->getString(courseCol);
            att.semesterId = results->getString(semesterCol);
            att.date = results->getString(dateCol);
            att.status = results->getString(statusCol);
            att.notes = results->getString(notesCol);
            att.recordedBy = results->getInt(recordedByCol);
            att.createdAt = results->getString(createdAtCol);
            att.studentName = results->getString("student_name");
            att.courseName = results->getString("course_name");
            att.courseCode = results->getString("course_code");
            attendances.push_back(att);
        }
    }

    return attendances;
}

vector<Attendance> AttendanceRepository::getCourseAttendance(const string& courseId,
                                                             const string& semesterId,
                                                             const string& date) {
    vector<Attendance> attendances;

    if (!db || !db->isConnected()) {
        if (!db->connect()) return attendances;
    }

    string attendanceTable = db->resolveTable({"attendance"});
    string studentCol = db->resolveColumn(attendanceTable, {"student_id", "studentId"});
    string courseCol = db->resolveColumn(attendanceTable, {"course_id", "courseId"});
    string semesterCol = db->resolveColumn(attendanceTable, {"semester_id", "semesterId"});
    string dateCol = db->resolveColumn(attendanceTable, {"date", "session_date", "attendance_date"});
    string statusCol = db->resolveColumn(attendanceTable, {"status"});
    string notesCol = db->resolveColumn(attendanceTable, {"notes", "remarks"});
    string recordedByCol = db->resolveColumn(attendanceTable, {"recorded_by", "recordedBy"});
    string createdAtCol = db->resolveColumn(attendanceTable, {"created_at", "createdAt"});

    string studentTable = db->resolveTable({"students"});
    string courseTable = db->resolveTable({"courses"});
    string studentIdCol = db->resolveColumn(studentTable, {"id", "student_id", "studentId"});
    string courseIdCol = db->resolveColumn(courseTable, {"id", "course_id", "courseId"});
    string studentFirstCol = db->resolveColumn(studentTable, {"first_name", "firstName"});
    string studentLastCol = db->resolveColumn(studentTable, {"last_name", "lastName"});
    string courseNameCol = db->resolveColumn(courseTable, {"name_en", "nameEn", "name"});
    string courseCodeCol = db->resolveColumn(courseTable, {"code", "course_code", "courseCode"});

    string query = "SELECT a.*, CONCAT(s." + studentFirstCol + ", ' ', s." + studentLastCol + ") as student_name, "
                  "c." + courseNameCol + " as course_name, c." + courseCodeCol + " as course_code "
                  "FROM " + attendanceTable + " a "
                  "JOIN " + studentTable + " s ON a." + studentCol + " = s." + studentIdCol + " "
                  "JOIN " + courseTable + " c ON a." + courseCol + " = c." + courseIdCol + " "
                  "WHERE a." + courseCol + " = '" + db->escapeString(courseId) + "' "
                  "AND a." + semesterCol + " = '" + db->escapeString(semesterId) + "'";

    if (!date.empty()) {
        query += " AND a." + dateCol + " = '" + db->escapeString(date) + "'";
    }

    query += " ORDER BY a." + dateCol + " DESC, s." + studentIdCol;

    auto results = db->executeQuery(query);

    if (results) {
        while (results->next()) {
            Attendance att;
            att.id = results->getInt("id");
            att.studentId = results->getString(studentCol);
            att.courseId = results->getString(courseCol);
            att.semesterId = results->getString(semesterCol);
            att.date = results->getString(dateCol);
            att.status = results->getString(statusCol);
            att.notes = results->getString(notesCol);
            att.recordedBy = results->getInt(recordedByCol);
            att.createdAt = results->getString(createdAtCol);
            att.studentName = results->getString("student_name");
            att.courseName = results->getString("course_name");
            att.courseCode = results->getString("course_code");
            attendances.push_back(att);
        }
    }

    return attendances;
}

int AttendanceRepository::getAttendanceCount(const string& studentId, const string& courseId,
                                             const string& semesterId, const string& status) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return 0;
    }

    string attendanceTable = db->resolveTable({"attendance"});
    string studentCol = db->resolveColumn(attendanceTable, {"student_id", "studentId"});
    string courseCol = db->resolveColumn(attendanceTable, {"course_id", "courseId"});
    string semesterCol = db->resolveColumn(attendanceTable, {"semester_id", "semesterId"});
    string statusCol = db->resolveColumn(attendanceTable, {"status"});

    string query = "SELECT COUNT(*) as cnt FROM " + attendanceTable + " "
                  "WHERE " + studentCol + " = '" + db->escapeString(studentId) + "' "
                  "AND " + courseCol + " = '" + db->escapeString(courseId) + "' "
                  "AND " + semesterCol + " = '" + db->escapeString(semesterId) + "' "
                  "AND " + statusCol + " = '" + db->escapeString(status) + "'";

    auto results = db->executeQuery(query);

    if (results && results->next()) {
        return results->getInt("cnt");
    }

    return 0;
}

bool AttendanceRepository::getAttendanceById(int attendanceId, Attendance& attendance) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return false;
    }

    string attendanceTable = db->resolveTable({"attendance"});
    string idCol = db->resolveColumn(attendanceTable, {"id"});
    if (idCol.empty()) {
        return false;
    }

    string studentCol = db->resolveColumn(attendanceTable, {"student_id", "studentId"});
    string courseCol = db->resolveColumn(attendanceTable, {"course_id", "courseId"});
    string semesterCol = db->resolveColumn(attendanceTable, {"semester_id", "semesterId"});
    string dateCol = db->resolveColumn(attendanceTable, {"date", "session_date", "attendance_date"});
    string statusCol = db->resolveColumn(attendanceTable, {"status"});
    string notesCol = db->resolveColumn(attendanceTable, {"notes", "remarks"});
    string recordedByCol = db->resolveColumn(attendanceTable, {"recorded_by", "recordedBy"});
    string createdAtCol = db->resolveColumn(attendanceTable, {"created_at", "createdAt"});

    string query = "SELECT * FROM " + attendanceTable + " WHERE " + idCol + " = " + to_string(attendanceId);
    auto results = db->executeQuery(query);
    if (results && results->next()) {
        attendance.id = results->getInt(idCol);
        attendance.studentId = results->getString(studentCol);
        attendance.courseId = results->getString(courseCol);
        attendance.semesterId = results->getString(semesterCol);
        attendance.date = results->getString(dateCol);
        attendance.status = results->getString(statusCol);
        attendance.notes = results->getString(notesCol);
        attendance.recordedBy = results->getInt(recordedByCol);
        attendance.createdAt = results->getString(createdAtCol);
        return true;
    }

    return false;
}

bool AttendanceRepository::updateAttendance(int attendanceId, const string& status, const string& notes, int recordedBy) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return false;
    }

    string attendanceTable = db->resolveTable({"attendance"});
    string idCol = db->resolveColumn(attendanceTable, {"id"});
    string statusCol = db->resolveColumn(attendanceTable, {"status"});
    string notesCol = db->resolveColumn(attendanceTable, {"notes", "remarks"});
    string recordedByCol = db->resolveColumn(attendanceTable, {"recorded_by", "recordedBy"});

    if (idCol.empty()) {
        return false;
    }

    string query = "UPDATE " + attendanceTable + " SET "
                  + statusCol + " = '" + db->escapeString(status) + "', "
                  + notesCol + " = '" + db->escapeString(notes) + "', "
                  + recordedByCol + " = " + to_string(recordedBy) + " "
                  "WHERE " + idCol + " = " + to_string(attendanceId);

    return db->executeUpdate(query) > 0;
}

bool AttendanceRepository::deleteAttendance(int attendanceId) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return false;
    }

    string attendanceTable = db->resolveTable({"attendance"});
    string idCol = db->resolveColumn(attendanceTable, {"id"});
    if (idCol.empty()) {
        return false;
    }
    string query = "DELETE FROM " + attendanceTable + " WHERE " + idCol + " = " + to_string(attendanceId);
    return db->executeUpdate(query) > 0;
}
