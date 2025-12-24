#include "GradeRepository.h"

GradeRepository::GradeRepository(Database* database) : db(database) {
}

bool GradeRepository::getGrade(const string& studentId, const string& courseId, 
                               const string& semesterId, Grade& grade) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return false;
    }

    string gradeTable = db->resolveTable({"grades"});
    if (!db->tableExists(gradeTable)) {
        return false;
    }
    string gradeStudentCol = db->resolveColumn(gradeTable, {"student_id", "studentId"});
    string gradeCourseCol = db->resolveColumn(gradeTable, {"course_id", "courseId"});
    string gradeSemesterCol = db->resolveColumn(gradeTable, {"semester_id", "semesterId"});
    string assignment1Col = db->resolveColumn(gradeTable, {"assignment1_marks", "assignment1Marks", "assignment1"});
    string assignment2Col = db->resolveColumn(gradeTable, {"assignment2_marks", "assignment2Marks", "assignment2"});
    string yearWorkCol = db->resolveColumn(gradeTable, {"year_work_marks", "yearWorkMarks", "yearWork"});
    string midtermCol = db->resolveColumn(gradeTable, {"midterm_marks", "midtermMarks", "midterm"});
    string finalExamCol = db->resolveColumn(gradeTable, {"final_exam_marks", "finalExamMarks", "finalExam"});
    string totalMarksCol = db->resolveColumn(gradeTable, {"total_marks", "totalMarks"});
    string percentageCol = db->resolveColumn(gradeTable, {"percentage"});
    string gradePointsCol = db->resolveColumn(gradeTable, {"grade_points", "gradePoints"});
    string letterGradeCol = db->resolveColumn(gradeTable, {"letter_grade", "letterGrade"});
    string evaluationCol = db->resolveColumn(gradeTable, {"evaluation"});
    string updatedByCol = db->resolveColumn(gradeTable, {"updated_by", "updatedBy"});
    string updatedAtCol = db->resolveColumn(gradeTable, {"updated_at", "updatedAt"});

    string studentTable = db->resolveTable({"students"});
    string courseTable = db->resolveTable({"courses"});
    string studentIdCol = db->resolveColumn(studentTable, {"id", "student_id", "studentId"});
    string courseIdCol = db->resolveColumn(courseTable, {"id", "course_id", "courseId"});
    string studentFirstCol = db->resolveColumn(studentTable, {"first_name", "firstName"});
    string studentLastCol = db->resolveColumn(studentTable, {"last_name", "lastName"});
    string courseNameCol = db->resolveColumn(courseTable, {"name_en", "nameEn", "name"});
    string courseCodeCol = db->resolveColumn(courseTable, {"code", "course_code", "courseCode"});
    string courseMaxCol = db->resolveColumn(courseTable, {"max_marks", "maxMarks"});

    string query = "SELECT g.*, CONCAT(s." + studentFirstCol + ", ' ', s." + studentLastCol + ") as student_name, "
                  "c." + courseNameCol + " as course_name, c." + courseCodeCol + " as course_code, c." + courseMaxCol + " as max_marks "
                  "FROM " + gradeTable + " g "
                  "JOIN " + studentTable + " s ON g." + gradeStudentCol + " = s." + studentIdCol + " "
                  "JOIN " + courseTable + " c ON g." + gradeCourseCol + " = c." + courseIdCol + " "
                  "WHERE g." + gradeStudentCol + " = '" + db->escapeString(studentId) + "' "
                  "AND g." + gradeCourseCol + " = '" + db->escapeString(courseId) + "' "
                  "AND g." + gradeSemesterCol + " = '" + db->escapeString(semesterId) + "'";

    auto results = db->executeQuery(query);
    
    if (results && results->next()) {
        grade.id = results->getInt("id");
        grade.studentId = results->getString(gradeStudentCol);
        grade.courseId = results->getString(gradeCourseCol);
        grade.semesterId = results->getString(gradeSemesterCol);
        grade.assignment1 = results->getDouble(assignment1Col);
        grade.assignment2 = results->getDouble(assignment2Col);
        grade.yearWork = results->getDouble(yearWorkCol);
        grade.midterm = results->getDouble(midtermCol);
        grade.finalExam = results->getDouble(finalExamCol);
        grade.totalMarks = results->getDouble(totalMarksCol);
        grade.percentage = results->getDouble(percentageCol);
        grade.gradePoints = results->getDouble(gradePointsCol);
        grade.letterGrade = results->getString(letterGradeCol);
        grade.evaluation = results->getString(evaluationCol);
        grade.updatedBy = results->getInt(updatedByCol);
        grade.updatedAt = results->getString(updatedAtCol);
        grade.studentName = results->getString("student_name");
        grade.courseName = results->getString("course_name");
        grade.courseCode = results->getString("course_code");
        grade.maxMarks = results->getInt("max_marks");
        return true;
    }

    return false;
}

vector<Grade> GradeRepository::getStudentGrades(const string& studentId, const string& semesterId) {
    vector<Grade> grades;
    
    if (!db || !db->isConnected()) {
        if (!db->connect()) return grades;
    }

    string gradeTable = db->resolveTable({"grades"});
    if (!db->tableExists(gradeTable)) {
        return grades;
    }
    string gradeStudentCol = db->resolveColumn(gradeTable, {"student_id", "studentId"});
    string gradeCourseCol = db->resolveColumn(gradeTable, {"course_id", "courseId"});
    string gradeSemesterCol = db->resolveColumn(gradeTable, {"semester_id", "semesterId"});
    string assignment1Col = db->resolveColumn(gradeTable, {"assignment1_marks", "assignment1Marks", "assignment1"});
    string assignment2Col = db->resolveColumn(gradeTable, {"assignment2_marks", "assignment2Marks", "assignment2"});
    string yearWorkCol = db->resolveColumn(gradeTable, {"year_work_marks", "yearWorkMarks", "yearWork"});
    string midtermCol = db->resolveColumn(gradeTable, {"midterm_marks", "midtermMarks", "midterm"});
    string finalExamCol = db->resolveColumn(gradeTable, {"final_exam_marks", "finalExamMarks", "finalExam"});
    string totalMarksCol = db->resolveColumn(gradeTable, {"total_marks", "totalMarks"});
    string percentageCol = db->resolveColumn(gradeTable, {"percentage"});
    string gradePointsCol = db->resolveColumn(gradeTable, {"grade_points", "gradePoints"});
    string letterGradeCol = db->resolveColumn(gradeTable, {"letter_grade", "letterGrade"});
    string evaluationCol = db->resolveColumn(gradeTable, {"evaluation"});
    string updatedByCol = db->resolveColumn(gradeTable, {"updated_by", "updatedBy"});
    string updatedAtCol = db->resolveColumn(gradeTable, {"updated_at", "updatedAt"});

    string studentTable = db->resolveTable({"students"});
    string courseTable = db->resolveTable({"courses"});
    string studentIdCol = db->resolveColumn(studentTable, {"id", "student_id", "studentId"});
    string courseIdCol = db->resolveColumn(courseTable, {"id", "course_id", "courseId"});
    string studentFirstCol = db->resolveColumn(studentTable, {"first_name", "firstName"});
    string studentLastCol = db->resolveColumn(studentTable, {"last_name", "lastName"});
    string courseNameCol = db->resolveColumn(courseTable, {"name_en", "nameEn", "name"});
    string courseCodeCol = db->resolveColumn(courseTable, {"code", "course_code", "courseCode"});
    string courseMaxCol = db->resolveColumn(courseTable, {"max_marks", "maxMarks"});

    string query = "SELECT g.*, CONCAT(s." + studentFirstCol + ", ' ', s." + studentLastCol + ") as student_name, "
                  "c." + courseNameCol + " as course_name, c." + courseCodeCol + " as course_code, c." + courseMaxCol + " as max_marks "
                  "FROM " + gradeTable + " g "
                  "JOIN " + studentTable + " s ON g." + gradeStudentCol + " = s." + studentIdCol + " "
                  "JOIN " + courseTable + " c ON g." + gradeCourseCol + " = c." + courseIdCol + " "
                  "WHERE g." + gradeStudentCol + " = '" + db->escapeString(studentId) + "'";
    
    if (!semesterId.empty()) {
        query += " AND g." + gradeSemesterCol + " = '" + db->escapeString(semesterId) + "'";
    }
    
    query += " ORDER BY c.code";

    auto results = db->executeQuery(query);
    
    if (results) {
        while (results->next()) {
            Grade grade;
            grade.id = results->getInt("id");
            grade.studentId = results->getString(gradeStudentCol);
            grade.courseId = results->getString(gradeCourseCol);
            grade.semesterId = results->getString(gradeSemesterCol);
            grade.assignment1 = results->getDouble(assignment1Col);
            grade.assignment2 = results->getDouble(assignment2Col);
            grade.yearWork = results->getDouble(yearWorkCol);
            grade.midterm = results->getDouble(midtermCol);
            grade.finalExam = results->getDouble(finalExamCol);
            grade.totalMarks = results->getDouble(totalMarksCol);
            grade.percentage = results->getDouble(percentageCol);
            grade.gradePoints = results->getDouble(gradePointsCol);
            grade.letterGrade = results->getString(letterGradeCol);
            grade.evaluation = results->getString(evaluationCol);
            grade.updatedBy = results->getInt(updatedByCol);
            grade.updatedAt = results->getString(updatedAtCol);
            grade.studentName = results->getString("student_name");
            grade.courseName = results->getString("course_name");
            grade.courseCode = results->getString("course_code");
            grade.maxMarks = results->getInt("max_marks");
            grades.push_back(grade);
        }
    }

    return grades;
}

vector<Grade> GradeRepository::getCourseGrades(const string& courseId, const string& semesterId) {
    vector<Grade> grades;
    
    if (!db || !db->isConnected()) {
        if (!db->connect()) return grades;
    }

    string gradeTable = db->resolveTable({"grades"});
    if (!db->tableExists(gradeTable)) {
        return grades;
    }
    string gradeStudentCol = db->resolveColumn(gradeTable, {"student_id", "studentId"});
    string gradeCourseCol = db->resolveColumn(gradeTable, {"course_id", "courseId"});
    string gradeSemesterCol = db->resolveColumn(gradeTable, {"semester_id", "semesterId"});
    string assignment1Col = db->resolveColumn(gradeTable, {"assignment1_marks", "assignment1Marks", "assignment1"});
    string assignment2Col = db->resolveColumn(gradeTable, {"assignment2_marks", "assignment2Marks", "assignment2"});
    string yearWorkCol = db->resolveColumn(gradeTable, {"year_work_marks", "yearWorkMarks", "yearWork"});
    string midtermCol = db->resolveColumn(gradeTable, {"midterm_marks", "midtermMarks", "midterm"});
    string finalExamCol = db->resolveColumn(gradeTable, {"final_exam_marks", "finalExamMarks", "finalExam"});
    string totalMarksCol = db->resolveColumn(gradeTable, {"total_marks", "totalMarks"});
    string percentageCol = db->resolveColumn(gradeTable, {"percentage"});
    string gradePointsCol = db->resolveColumn(gradeTable, {"grade_points", "gradePoints"});
    string letterGradeCol = db->resolveColumn(gradeTable, {"letter_grade", "letterGrade"});
    string evaluationCol = db->resolveColumn(gradeTable, {"evaluation"});
    string updatedByCol = db->resolveColumn(gradeTable, {"updated_by", "updatedBy"});
    string updatedAtCol = db->resolveColumn(gradeTable, {"updated_at", "updatedAt"});

    string studentTable = db->resolveTable({"students"});
    string courseTable = db->resolveTable({"courses"});
    string studentIdCol = db->resolveColumn(studentTable, {"id", "student_id", "studentId"});
    string courseIdCol = db->resolveColumn(courseTable, {"id", "course_id", "courseId"});
    string studentFirstCol = db->resolveColumn(studentTable, {"first_name", "firstName"});
    string studentLastCol = db->resolveColumn(studentTable, {"last_name", "lastName"});
    string courseNameCol = db->resolveColumn(courseTable, {"name_en", "nameEn", "name"});
    string courseCodeCol = db->resolveColumn(courseTable, {"code", "course_code", "courseCode"});
    string courseMaxCol = db->resolveColumn(courseTable, {"max_marks", "maxMarks"});

    string query = "SELECT g.*, CONCAT(s." + studentFirstCol + ", ' ', s." + studentLastCol + ") as student_name, "
                  "c." + courseNameCol + " as course_name, c." + courseCodeCol + " as course_code, c." + courseMaxCol + " as max_marks "
                  "FROM " + gradeTable + " g "
                  "JOIN " + studentTable + " s ON g." + gradeStudentCol + " = s." + studentIdCol + " "
                  "JOIN " + courseTable + " c ON g." + gradeCourseCol + " = c." + courseIdCol + " "
                  "WHERE g." + gradeCourseCol + " = '" + db->escapeString(courseId) + "' "
                  "AND g." + gradeSemesterCol + " = '" + db->escapeString(semesterId) + "' "
                  "ORDER BY s." + studentIdCol;

    auto results = db->executeQuery(query);
    
    if (results) {
        while (results->next()) {
            Grade grade;
            grade.id = results->getInt("id");
            grade.studentId = results->getString(gradeStudentCol);
            grade.courseId = results->getString(gradeCourseCol);
            grade.semesterId = results->getString(gradeSemesterCol);
            grade.assignment1 = results->getDouble(assignment1Col);
            grade.assignment2 = results->getDouble(assignment2Col);
            grade.yearWork = results->getDouble(yearWorkCol);
            grade.midterm = results->getDouble(midtermCol);
            grade.finalExam = results->getDouble(finalExamCol);
            grade.totalMarks = results->getDouble(totalMarksCol);
            grade.percentage = results->getDouble(percentageCol);
            grade.gradePoints = results->getDouble(gradePointsCol);
            grade.letterGrade = results->getString(letterGradeCol);
            grade.evaluation = results->getString(evaluationCol);
            grade.updatedBy = results->getInt(updatedByCol);
            grade.updatedAt = results->getString(updatedAtCol);
            grade.studentName = results->getString("student_name");
            grade.courseName = results->getString("course_name");
            grade.courseCode = results->getString("course_code");
            grade.maxMarks = results->getInt("max_marks");
            grades.push_back(grade);
        }
    }

    return grades;
}

bool GradeRepository::updateGrade(const Grade& grade, int updatedBy) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return false;
    }

    string gradeTable = db->resolveTable({"grades"});
    if (!db->tableExists(gradeTable)) {
        return false;
    }
    string assignment1Col = db->resolveColumn(gradeTable, {"assignment1_marks", "assignment1Marks", "assignment1"});
    string assignment2Col = db->resolveColumn(gradeTable, {"assignment2_marks", "assignment2Marks", "assignment2"});
    string yearWorkCol = db->resolveColumn(gradeTable, {"year_work_marks", "yearWorkMarks", "yearWork"});
    string midtermCol = db->resolveColumn(gradeTable, {"midterm_marks", "midtermMarks", "midterm"});
    string finalExamCol = db->resolveColumn(gradeTable, {"final_exam_marks", "finalExamMarks", "finalExam"});
    string totalMarksCol = db->resolveColumn(gradeTable, {"total_marks", "totalMarks"});
    string percentageCol = db->resolveColumn(gradeTable, {"percentage"});
    string gradePointsCol = db->resolveColumn(gradeTable, {"grade_points", "gradePoints"});
    string letterGradeCol = db->resolveColumn(gradeTable, {"letter_grade", "letterGrade"});
    string evaluationCol = db->resolveColumn(gradeTable, {"evaluation"});
    string updatedByCol = db->resolveColumn(gradeTable, {"updated_by", "updatedBy"});

    string query = "UPDATE " + gradeTable + " SET "
                  + assignment1Col + " = " + to_string(grade.assignment1) + ", "
                  + assignment2Col + " = " + to_string(grade.assignment2) + ", "
                  + yearWorkCol + " = " + to_string(grade.yearWork) + ", "
                  + midtermCol + " = " + to_string(grade.midterm) + ", "
                  + finalExamCol + " = " + to_string(grade.finalExam) + ", "
                  + totalMarksCol + " = " + to_string(grade.totalMarks) + ", "
                  + percentageCol + " = " + to_string(grade.percentage) + ", "
                  + gradePointsCol + " = " + to_string(grade.gradePoints) + ", "
                  + letterGradeCol + " = '" + db->escapeString(grade.letterGrade) + "', "
                  + evaluationCol + " = '" + db->escapeString(grade.evaluation) + "', "
                  + updatedByCol + " = " + to_string(updatedBy) + " "
                  "WHERE id = " + to_string(grade.id);

    return db->executeUpdate(query) > 0;
}

bool GradeRepository::createOrUpdateGrade(const Grade& grade, int userId) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return false;
    }

    string gradeTable = db->resolveTable({"grades"});
    if (!db->tableExists(gradeTable)) {
        return false;
    }
    string gradeStudentCol = db->resolveColumn(gradeTable, {"student_id", "studentId"});
    string gradeCourseCol = db->resolveColumn(gradeTable, {"course_id", "courseId"});
    string gradeSemesterCol = db->resolveColumn(gradeTable, {"semester_id", "semesterId"});
    string assignment1Col = db->resolveColumn(gradeTable, {"assignment1_marks", "assignment1Marks", "assignment1"});
    string assignment2Col = db->resolveColumn(gradeTable, {"assignment2_marks", "assignment2Marks", "assignment2"});
    string yearWorkCol = db->resolveColumn(gradeTable, {"year_work_marks", "yearWorkMarks", "yearWork"});
    string midtermCol = db->resolveColumn(gradeTable, {"midterm_marks", "midtermMarks", "midterm"});
    string finalExamCol = db->resolveColumn(gradeTable, {"final_exam_marks", "finalExamMarks", "finalExam"});
    string totalMarksCol = db->resolveColumn(gradeTable, {"total_marks", "totalMarks"});
    string percentageCol = db->resolveColumn(gradeTable, {"percentage"});
    string gradePointsCol = db->resolveColumn(gradeTable, {"grade_points", "gradePoints"});
    string letterGradeCol = db->resolveColumn(gradeTable, {"letter_grade", "letterGrade"});
    string evaluationCol = db->resolveColumn(gradeTable, {"evaluation"});
    string updatedByCol = db->resolveColumn(gradeTable, {"updated_by", "updatedBy"});

    string query = "INSERT INTO " + gradeTable + " (" + gradeStudentCol + ", " + gradeCourseCol + ", " + gradeSemesterCol + ", "
                  + assignment1Col + ", " + assignment2Col + ", " + yearWorkCol + ", " + midtermCol + ", " + finalExamCol + ", "
                  + totalMarksCol + ", " + percentageCol + ", " + gradePointsCol + ", " + letterGradeCol + ", " + evaluationCol + ", " + updatedByCol + ") "
                  "VALUES ('" + db->escapeString(grade.studentId) + "', '" +
                  db->escapeString(grade.courseId) + "', '" +
                  db->escapeString(grade.semesterId) + "', " +
                  to_string(grade.assignment1) + ", " + to_string(grade.assignment2) + ", " +
                  to_string(grade.yearWork) + ", " + to_string(grade.midterm) + ", " +
                  to_string(grade.finalExam) + ", " + to_string(grade.totalMarks) + ", " +
                  to_string(grade.percentage) + ", " + to_string(grade.gradePoints) + ", '" +
                  db->escapeString(grade.letterGrade) + "', '" + db->escapeString(grade.evaluation) + "', " +
                  to_string(userId) + ") "
                  "ON DUPLICATE KEY UPDATE "
                  + assignment1Col + " = " + to_string(grade.assignment1) + ", "
                  + assignment2Col + " = " + to_string(grade.assignment2) + ", "
                  + yearWorkCol + " = " + to_string(grade.yearWork) + ", "
                  + midtermCol + " = " + to_string(grade.midterm) + ", "
                  + finalExamCol + " = " + to_string(grade.finalExam) + ", "
                  + totalMarksCol + " = " + to_string(grade.totalMarks) + ", "
                  + percentageCol + " = " + to_string(grade.percentage) + ", "
                  + gradePointsCol + " = " + to_string(grade.gradePoints) + ", "
                  + letterGradeCol + " = '" + db->escapeString(grade.letterGrade) + "', "
                  + evaluationCol + " = '" + db->escapeString(grade.evaluation) + "', "
                  + updatedByCol + " = " + to_string(userId);

    return db->executeUpdate(query) >= 0;
}

bool GradeRepository::deleteGrade(int gradeId) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return false;
    }

    string gradeTable = db->resolveTable({"grades"});
    if (!db->tableExists(gradeTable)) {
        return false;
    }
    string idCol = db->resolveColumn(gradeTable, {"id"});
    string query = "DELETE FROM " + gradeTable + " WHERE " + idCol + " = " + to_string(gradeId);
    return db->executeUpdate(query) > 0;
}

bool GradeRepository::deleteGradeByKey(const string& studentId, const string& courseId, const string& semesterId) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return false;
    }

    string gradeTable = db->resolveTable({"grades"});
    if (!db->tableExists(gradeTable)) {
        return false;
    }
    string gradeStudentCol = db->resolveColumn(gradeTable, {"student_id", "studentId"});
    string gradeCourseCol = db->resolveColumn(gradeTable, {"course_id", "courseId"});
    string gradeSemesterCol = db->resolveColumn(gradeTable, {"semester_id", "semesterId"});

    string query = "DELETE FROM " + gradeTable + " WHERE " + gradeStudentCol + " = '" + db->escapeString(studentId) + "' "
                  "AND " + gradeCourseCol + " = '" + db->escapeString(courseId) + "' "
                  "AND " + gradeSemesterCol + " = '" + db->escapeString(semesterId) + "'";
    return db->executeUpdate(query) > 0;
}
