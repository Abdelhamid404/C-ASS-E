#include "CourseRepository.h"

namespace {
Course buildCourse(ResultSet* results,
                   const std::string& idCol,
                   const std::string& codeCol,
                   const std::string& nameArCol,
                   const std::string& nameEnCol,
                   const std::string& descriptionCol,
                   const std::string& departmentCol,
                   const std::string& levelCol,
                   const std::string& semesterCol,
                   const std::string& courseTypeCol,
                   const std::string& creditHoursCol,
                   const std::string& lectureHoursCol,
                   const std::string& tutorialHoursCol,
                   const std::string& labHoursCol,
                   const std::string& practicalHoursCol,
                   const std::string& maxMarksCol,
                   const std::string& assignment1Col,
                   const std::string& assignment2Col,
                   const std::string& yearWorkCol,
                   const std::string& finalExamCol) {
    Course course;
    course.id = results->getString(idCol);
    course.code = results->getString(codeCol);
    course.nameAr = results->getString(nameArCol);
    course.nameEn = results->getString(nameEnCol);
    course.description = results->getString(descriptionCol);
    course.departmentId = results->getString(departmentCol);
    course.academicLevelId = results->getString(levelCol);
    course.semesterNumber = results->getInt(semesterCol);
    course.courseTypeId = results->getString(courseTypeCol);
    course.creditHours = results->getInt(creditHoursCol);
    course.lectureHours = results->getInt(lectureHoursCol);
    course.tutorialHours = results->getInt(tutorialHoursCol);
    course.labHours = results->getInt(labHoursCol);
    course.practicalHours = results->getInt(practicalHoursCol);
    course.maxMarks = results->getInt(maxMarksCol);
    course.assignment1Marks = results->getInt(assignment1Col);
    course.assignment2Marks = results->getInt(assignment2Col);
    course.yearWorkMarks = results->getInt(yearWorkCol);
    course.finalExamMarks = results->getInt(finalExamCol);
    return course;
}
}

CourseRepository::CourseRepository(Database* database) : db(database) {
}

bool CourseRepository::getCourseById(const string& courseId, Course& course) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return false;
    }

    string courseTable = db->resolveTable({"courses"});
    string idCol = db->resolveColumn(courseTable, {"id", "course_id", "courseId"});
    string codeCol = db->resolveColumn(courseTable, {"code", "course_code", "courseCode"});
    string nameArCol = db->resolveColumn(courseTable, {"name_ar", "nameAr"});
    string nameEnCol = db->resolveColumn(courseTable, {"name_en", "nameEn", "name"});
    string descriptionCol = db->resolveColumn(courseTable, {"description"});
    string departmentCol = db->resolveColumn(courseTable, {"department_id", "departmentId"});
    string levelCol = db->resolveColumn(courseTable, {"academic_level_id", "academicLevelId", "level_id", "levelId"});
    string semesterCol = db->resolveColumn(courseTable, {"semester_number", "semesterNumber"});
    string courseTypeCol = db->resolveColumn(courseTable, {"course_type_id", "courseTypeId"});
    string creditHoursCol = db->resolveColumn(courseTable, {"credit_hours", "creditHours"});
    string lectureHoursCol = db->resolveColumn(courseTable, {"lecture_hours", "lectureHours"});
    string tutorialHoursCol = db->resolveColumn(courseTable, {"tutorial_hours", "tutorialHours"});
    string labHoursCol = db->resolveColumn(courseTable, {"lab_hours", "labHours"});
    string practicalHoursCol = db->resolveColumn(courseTable, {"practical_hours", "practicalHours"});
    string maxMarksCol = db->resolveColumn(courseTable, {"max_marks", "maxMarks"});
    string assignment1Col = db->resolveColumn(courseTable, {"assignment1_marks", "assignment1Marks", "assignment1"});
    string assignment2Col = db->resolveColumn(courseTable, {"assignment2_marks", "assignment2Marks", "assignment2"});
    string yearWorkCol = db->resolveColumn(courseTable, {"year_work_marks", "yearWorkMarks", "yearWork"});
    string finalExamCol = db->resolveColumn(courseTable, {"final_exam_marks", "finalExamMarks", "finalExam"});

    string query = "SELECT * FROM " + courseTable + " WHERE " + idCol + " = '" + db->escapeString(courseId) + "'";

    auto results = db->executeQuery(query);

    if (results && results->next()) {
        course = buildCourse(results.get(), idCol, codeCol, nameArCol, nameEnCol, descriptionCol,
                             departmentCol, levelCol, semesterCol, courseTypeCol, creditHoursCol,
                             lectureHoursCol, tutorialHoursCol, labHoursCol, practicalHoursCol,
                             maxMarksCol, assignment1Col, assignment2Col, yearWorkCol, finalExamCol);
        return true;
    }

    return false;
}

vector<Course> CourseRepository::listByDepartment(const string& departmentId) {
    vector<Course> courses;

    if (!db || !db->isConnected()) {
        if (!db->connect()) return courses;
    }

    string courseTable = db->resolveTable({"courses"});
    string idCol = db->resolveColumn(courseTable, {"id", "course_id", "courseId"});
    string codeCol = db->resolveColumn(courseTable, {"code", "course_code", "courseCode"});
    string nameArCol = db->resolveColumn(courseTable, {"name_ar", "nameAr"});
    string nameEnCol = db->resolveColumn(courseTable, {"name_en", "nameEn", "name"});
    string descriptionCol = db->resolveColumn(courseTable, {"description"});
    string departmentCol = db->resolveColumn(courseTable, {"department_id", "departmentId"});
    string levelCol = db->resolveColumn(courseTable, {"academic_level_id", "academicLevelId", "level_id", "levelId"});
    string semesterCol = db->resolveColumn(courseTable, {"semester_number", "semesterNumber"});
    string courseTypeCol = db->resolveColumn(courseTable, {"course_type_id", "courseTypeId"});
    string creditHoursCol = db->resolveColumn(courseTable, {"credit_hours", "creditHours"});
    string lectureHoursCol = db->resolveColumn(courseTable, {"lecture_hours", "lectureHours"});
    string tutorialHoursCol = db->resolveColumn(courseTable, {"tutorial_hours", "tutorialHours"});
    string labHoursCol = db->resolveColumn(courseTable, {"lab_hours", "labHours"});
    string practicalHoursCol = db->resolveColumn(courseTable, {"practical_hours", "practicalHours"});
    string maxMarksCol = db->resolveColumn(courseTable, {"max_marks", "maxMarks"});
    string assignment1Col = db->resolveColumn(courseTable, {"assignment1_marks", "assignment1Marks", "assignment1"});
    string assignment2Col = db->resolveColumn(courseTable, {"assignment2_marks", "assignment2Marks", "assignment2"});
    string yearWorkCol = db->resolveColumn(courseTable, {"year_work_marks", "yearWorkMarks", "yearWork"});
    string finalExamCol = db->resolveColumn(courseTable, {"final_exam_marks", "finalExamMarks", "finalExam"});

    string query = "SELECT * FROM " + courseTable + " WHERE " + departmentCol + " = '" + db->escapeString(departmentId) + "' ORDER BY " + codeCol;

    auto results = db->executeQuery(query);

    if (results) {
        while (results->next()) {
            courses.push_back(buildCourse(results.get(), idCol, codeCol, nameArCol, nameEnCol, descriptionCol,
                                          departmentCol, levelCol, semesterCol, courseTypeCol, creditHoursCol,
                                          lectureHoursCol, tutorialHoursCol, labHoursCol, practicalHoursCol,
                                          maxMarksCol, assignment1Col, assignment2Col, yearWorkCol, finalExamCol));
        }
    }

    return courses;
}

vector<Course> CourseRepository::listByDepartmentLevel(const string& departmentId, const string& levelId) {
    vector<Course> courses;

    if (!db || !db->isConnected()) {
        if (!db->connect()) return courses;
    }

    string courseTable = db->resolveTable({"courses"});
    string idCol = db->resolveColumn(courseTable, {"id", "course_id", "courseId"});
    string codeCol = db->resolveColumn(courseTable, {"code", "course_code", "courseCode"});
    string nameArCol = db->resolveColumn(courseTable, {"name_ar", "nameAr"});
    string nameEnCol = db->resolveColumn(courseTable, {"name_en", "nameEn", "name"});
    string descriptionCol = db->resolveColumn(courseTable, {"description"});
    string departmentCol = db->resolveColumn(courseTable, {"department_id", "departmentId"});
    string levelCol = db->resolveColumn(courseTable, {"academic_level_id", "academicLevelId", "level_id", "levelId"});
    string semesterCol = db->resolveColumn(courseTable, {"semester_number", "semesterNumber"});
    string courseTypeCol = db->resolveColumn(courseTable, {"course_type_id", "courseTypeId"});
    string creditHoursCol = db->resolveColumn(courseTable, {"credit_hours", "creditHours"});
    string lectureHoursCol = db->resolveColumn(courseTable, {"lecture_hours", "lectureHours"});
    string tutorialHoursCol = db->resolveColumn(courseTable, {"tutorial_hours", "tutorialHours"});
    string labHoursCol = db->resolveColumn(courseTable, {"lab_hours", "labHours"});
    string practicalHoursCol = db->resolveColumn(courseTable, {"practical_hours", "practicalHours"});
    string maxMarksCol = db->resolveColumn(courseTable, {"max_marks", "maxMarks"});
    string assignment1Col = db->resolveColumn(courseTable, {"assignment1_marks", "assignment1Marks", "assignment1"});
    string assignment2Col = db->resolveColumn(courseTable, {"assignment2_marks", "assignment2Marks", "assignment2"});
    string yearWorkCol = db->resolveColumn(courseTable, {"year_work_marks", "yearWorkMarks", "yearWork"});
    string finalExamCol = db->resolveColumn(courseTable, {"final_exam_marks", "finalExamMarks", "finalExam"});

    string query = "SELECT * FROM " + courseTable + " WHERE " + departmentCol + " = '" + db->escapeString(departmentId) + "' "
                  "AND " + levelCol + " = '" + db->escapeString(levelId) + "' ORDER BY " + codeCol;

    auto results = db->executeQuery(query);

    if (results) {
        while (results->next()) {
            courses.push_back(buildCourse(results.get(), idCol, codeCol, nameArCol, nameEnCol, descriptionCol,
                                          departmentCol, levelCol, semesterCol, courseTypeCol, creditHoursCol,
                                          lectureHoursCol, tutorialHoursCol, labHoursCol, practicalHoursCol,
                                          maxMarksCol, assignment1Col, assignment2Col, yearWorkCol, finalExamCol));
        }
    }

    return courses;
}

vector<Course> CourseRepository::listBySemester(const string& semesterId) {
    vector<Course> courses;

    if (!db || !db->isConnected()) {
        if (!db->connect()) return courses;
    }

    string semesterTable = db->resolveTable({"semesters"});
    string semesterIdCol = db->resolveColumn(semesterTable, {"id", "semester_id", "semesterId"});
    string semesterNumberCol = db->resolveColumn(semesterTable, {"semester_number", "semesterNumber"});

    string semQuery = "SELECT " + semesterNumberCol + " FROM " + semesterTable + " WHERE " + semesterIdCol + " = '" + db->escapeString(semesterId) + "'";
    auto semResult = db->executeQuery(semQuery);
    int semesterNum = 0;
    if (semResult && semResult->next()) {
        semesterNum = semResult->getInt(semesterNumberCol);
    }

    string courseTable = db->resolveTable({"courses"});
    string idCol = db->resolveColumn(courseTable, {"id", "course_id", "courseId"});
    string codeCol = db->resolveColumn(courseTable, {"code", "course_code", "courseCode"});
    string nameArCol = db->resolveColumn(courseTable, {"name_ar", "nameAr"});
    string nameEnCol = db->resolveColumn(courseTable, {"name_en", "nameEn", "name"});
    string descriptionCol = db->resolveColumn(courseTable, {"description"});
    string departmentCol = db->resolveColumn(courseTable, {"department_id", "departmentId"});
    string levelCol = db->resolveColumn(courseTable, {"academic_level_id", "academicLevelId", "level_id", "levelId"});
    string semesterCol = db->resolveColumn(courseTable, {"semester_number", "semesterNumber"});
    string courseTypeCol = db->resolveColumn(courseTable, {"course_type_id", "courseTypeId"});
    string creditHoursCol = db->resolveColumn(courseTable, {"credit_hours", "creditHours"});
    string lectureHoursCol = db->resolveColumn(courseTable, {"lecture_hours", "lectureHours"});
    string tutorialHoursCol = db->resolveColumn(courseTable, {"tutorial_hours", "tutorialHours"});
    string labHoursCol = db->resolveColumn(courseTable, {"lab_hours", "labHours"});
    string practicalHoursCol = db->resolveColumn(courseTable, {"practical_hours", "practicalHours"});
    string maxMarksCol = db->resolveColumn(courseTable, {"max_marks", "maxMarks"});
    string assignment1Col = db->resolveColumn(courseTable, {"assignment1_marks", "assignment1Marks", "assignment1"});
    string assignment2Col = db->resolveColumn(courseTable, {"assignment2_marks", "assignment2Marks", "assignment2"});
    string yearWorkCol = db->resolveColumn(courseTable, {"year_work_marks", "yearWorkMarks", "yearWork"});
    string finalExamCol = db->resolveColumn(courseTable, {"final_exam_marks", "finalExamMarks", "finalExam"});

    string query = "SELECT * FROM " + courseTable + " WHERE " + semesterCol + " = " + to_string(semesterNum) + " ORDER BY " + codeCol;

    auto results = db->executeQuery(query);

    if (results) {
        while (results->next()) {
            courses.push_back(buildCourse(results.get(), idCol, codeCol, nameArCol, nameEnCol, descriptionCol,
                                          departmentCol, levelCol, semesterCol, courseTypeCol, creditHoursCol,
                                          lectureHoursCol, tutorialHoursCol, labHoursCol, practicalHoursCol,
                                          maxMarksCol, assignment1Col, assignment2Col, yearWorkCol, finalExamCol));
        }
    }

    return courses;
}

vector<Course> CourseRepository::listAll() {
    vector<Course> courses;

    if (!db || !db->isConnected()) {
        if (!db->connect()) return courses;
    }

    string courseTable = db->resolveTable({"courses"});
    string idCol = db->resolveColumn(courseTable, {"id", "course_id", "courseId"});
    string codeCol = db->resolveColumn(courseTable, {"code", "course_code", "courseCode"});
    string nameArCol = db->resolveColumn(courseTable, {"name_ar", "nameAr"});
    string nameEnCol = db->resolveColumn(courseTable, {"name_en", "nameEn", "name"});
    string descriptionCol = db->resolveColumn(courseTable, {"description"});
    string departmentCol = db->resolveColumn(courseTable, {"department_id", "departmentId"});
    string levelCol = db->resolveColumn(courseTable, {"academic_level_id", "academicLevelId", "level_id", "levelId"});
    string semesterCol = db->resolveColumn(courseTable, {"semester_number", "semesterNumber"});
    string courseTypeCol = db->resolveColumn(courseTable, {"course_type_id", "courseTypeId"});
    string creditHoursCol = db->resolveColumn(courseTable, {"credit_hours", "creditHours"});
    string lectureHoursCol = db->resolveColumn(courseTable, {"lecture_hours", "lectureHours"});
    string tutorialHoursCol = db->resolveColumn(courseTable, {"tutorial_hours", "tutorialHours"});
    string labHoursCol = db->resolveColumn(courseTable, {"lab_hours", "labHours"});
    string practicalHoursCol = db->resolveColumn(courseTable, {"practical_hours", "practicalHours"});
    string maxMarksCol = db->resolveColumn(courseTable, {"max_marks", "maxMarks"});
    string assignment1Col = db->resolveColumn(courseTable, {"assignment1_marks", "assignment1Marks", "assignment1"});
    string assignment2Col = db->resolveColumn(courseTable, {"assignment2_marks", "assignment2Marks", "assignment2"});
    string yearWorkCol = db->resolveColumn(courseTable, {"year_work_marks", "yearWorkMarks", "yearWork"});
    string finalExamCol = db->resolveColumn(courseTable, {"final_exam_marks", "finalExamMarks", "finalExam"});

    string query = "SELECT * FROM " + courseTable + " ORDER BY " + codeCol;

    auto results = db->executeQuery(query);

    if (results) {
        while (results->next()) {
            courses.push_back(buildCourse(results.get(), idCol, codeCol, nameArCol, nameEnCol, descriptionCol,
                                          departmentCol, levelCol, semesterCol, courseTypeCol, creditHoursCol,
                                          lectureHoursCol, tutorialHoursCol, labHoursCol, practicalHoursCol,
                                          maxMarksCol, assignment1Col, assignment2Col, yearWorkCol, finalExamCol));
        }
    }

    return courses;
}

bool CourseRepository::createCourse(const Course& course) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return false;
    }

    string courseTable = db->resolveTable({"courses"});
    string idCol = db->resolveColumn(courseTable, {"id", "course_id", "courseId"});
    string codeCol = db->resolveColumn(courseTable, {"code", "course_code", "courseCode"});
    string nameArCol = db->resolveColumn(courseTable, {"name_ar", "nameAr"});
    string nameEnCol = db->resolveColumn(courseTable, {"name_en", "nameEn", "name"});
    string descriptionCol = db->resolveColumn(courseTable, {"description"});
    string departmentCol = db->resolveColumn(courseTable, {"department_id", "departmentId"});
    string levelCol = db->resolveColumn(courseTable, {"academic_level_id", "academicLevelId", "level_id", "levelId"});
    string semesterCol = db->resolveColumn(courseTable, {"semester_number", "semesterNumber"});
    string courseTypeCol = db->resolveColumn(courseTable, {"course_type_id", "courseTypeId"});
    string creditHoursCol = db->resolveColumn(courseTable, {"credit_hours", "creditHours"});
    string lectureHoursCol = db->resolveColumn(courseTable, {"lecture_hours", "lectureHours"});
    string tutorialHoursCol = db->resolveColumn(courseTable, {"tutorial_hours", "tutorialHours"});
    string labHoursCol = db->resolveColumn(courseTable, {"lab_hours", "labHours"});
    string practicalHoursCol = db->resolveColumn(courseTable, {"practical_hours", "practicalHours"});
    string maxMarksCol = db->resolveColumn(courseTable, {"max_marks", "maxMarks"});
    string assignment1Col = db->resolveColumn(courseTable, {"assignment1_marks", "assignment1Marks", "assignment1"});
    string assignment2Col = db->resolveColumn(courseTable, {"assignment2_marks", "assignment2Marks", "assignment2"});
    string yearWorkCol = db->resolveColumn(courseTable, {"year_work_marks", "yearWorkMarks", "yearWork"});
    string finalExamCol = db->resolveColumn(courseTable, {"final_exam_marks", "finalExamMarks", "finalExam"});

    vector<string> cols;
    vector<string> vals;
    cols.push_back(idCol);
    vals.push_back("'" + db->escapeString(course.id) + "'");
    cols.push_back(codeCol);
    vals.push_back("'" + db->escapeString(course.code) + "'");
    cols.push_back(nameEnCol);
    vals.push_back("'" + db->escapeString(course.nameEn) + "'");
    if (db->columnExists(courseTable, nameArCol)) {
        cols.push_back(nameArCol);
        vals.push_back("'" + db->escapeString(course.nameAr) + "'");
    }
    if (db->columnExists(courseTable, descriptionCol)) {
        cols.push_back(descriptionCol);
        vals.push_back("'" + db->escapeString(course.description) + "'");
    }
    if (db->columnExists(courseTable, departmentCol)) {
        cols.push_back(departmentCol);
        vals.push_back("'" + db->escapeString(course.departmentId) + "'");
    }
    if (db->columnExists(courseTable, levelCol)) {
        cols.push_back(levelCol);
        vals.push_back("'" + db->escapeString(course.academicLevelId) + "'");
    }
    if (db->columnExists(courseTable, semesterCol)) {
        cols.push_back(semesterCol);
        vals.push_back(to_string(course.semesterNumber));
    }
    if (db->columnExists(courseTable, courseTypeCol)) {
        cols.push_back(courseTypeCol);
        vals.push_back("'" + db->escapeString(course.courseTypeId) + "'");
    }
    if (db->columnExists(courseTable, creditHoursCol)) {
        cols.push_back(creditHoursCol);
        vals.push_back(to_string(course.creditHours));
    }
    if (db->columnExists(courseTable, lectureHoursCol)) {
        cols.push_back(lectureHoursCol);
        vals.push_back(to_string(course.lectureHours));
    }
    if (db->columnExists(courseTable, tutorialHoursCol)) {
        cols.push_back(tutorialHoursCol);
        vals.push_back(to_string(course.tutorialHours));
    }
    if (db->columnExists(courseTable, labHoursCol)) {
        cols.push_back(labHoursCol);
        vals.push_back(to_string(course.labHours));
    }
    if (db->columnExists(courseTable, practicalHoursCol)) {
        cols.push_back(practicalHoursCol);
        vals.push_back(to_string(course.practicalHours));
    }
    if (db->columnExists(courseTable, maxMarksCol)) {
        cols.push_back(maxMarksCol);
        vals.push_back(to_string(course.maxMarks));
    }
    if (db->columnExists(courseTable, assignment1Col)) {
        cols.push_back(assignment1Col);
        vals.push_back(to_string(course.assignment1Marks));
    }
    if (db->columnExists(courseTable, assignment2Col)) {
        cols.push_back(assignment2Col);
        vals.push_back(to_string(course.assignment2Marks));
    }
    if (db->columnExists(courseTable, yearWorkCol)) {
        cols.push_back(yearWorkCol);
        vals.push_back(to_string(course.yearWorkMarks));
    }
    if (db->columnExists(courseTable, finalExamCol)) {
        cols.push_back(finalExamCol);
        vals.push_back(to_string(course.finalExamMarks));
    }

    string query = "INSERT INTO " + courseTable + " (";
    for (size_t i = 0; i < cols.size(); ++i) {
        if (i > 0) query += ", ";
        query += cols[i];
    }
    query += ") VALUES (";
    for (size_t i = 0; i < vals.size(); ++i) {
        if (i > 0) query += ", ";
        query += vals[i];
    }
    query += ")";

    return db->executeUpdate(query) > 0;
}

bool CourseRepository::updateCourse(const string& courseId, const Course& course) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return false;
    }

    string courseTable = db->resolveTable({"courses"});
    string idCol = db->resolveColumn(courseTable, {"id", "course_id", "courseId"});
    string codeCol = db->resolveColumn(courseTable, {"code", "course_code", "courseCode"});
    string nameArCol = db->resolveColumn(courseTable, {"name_ar", "nameAr"});
    string nameEnCol = db->resolveColumn(courseTable, {"name_en", "nameEn", "name"});
    string descriptionCol = db->resolveColumn(courseTable, {"description"});
    string departmentCol = db->resolveColumn(courseTable, {"department_id", "departmentId"});
    string levelCol = db->resolveColumn(courseTable, {"academic_level_id", "academicLevelId", "level_id", "levelId"});
    string semesterCol = db->resolveColumn(courseTable, {"semester_number", "semesterNumber"});
    string courseTypeCol = db->resolveColumn(courseTable, {"course_type_id", "courseTypeId"});
    string creditHoursCol = db->resolveColumn(courseTable, {"credit_hours", "creditHours"});
    string lectureHoursCol = db->resolveColumn(courseTable, {"lecture_hours", "lectureHours"});
    string tutorialHoursCol = db->resolveColumn(courseTable, {"tutorial_hours", "tutorialHours"});
    string labHoursCol = db->resolveColumn(courseTable, {"lab_hours", "labHours"});
    string practicalHoursCol = db->resolveColumn(courseTable, {"practical_hours", "practicalHours"});
    string maxMarksCol = db->resolveColumn(courseTable, {"max_marks", "maxMarks"});
    string assignment1Col = db->resolveColumn(courseTable, {"assignment1_marks", "assignment1Marks", "assignment1"});
    string assignment2Col = db->resolveColumn(courseTable, {"assignment2_marks", "assignment2Marks", "assignment2"});
    string yearWorkCol = db->resolveColumn(courseTable, {"year_work_marks", "yearWorkMarks", "yearWork"});
    string finalExamCol = db->resolveColumn(courseTable, {"final_exam_marks", "finalExamMarks", "finalExam"});

    vector<string> updates;
    if (db->columnExists(courseTable, codeCol)) {
        updates.push_back(codeCol + " = '" + db->escapeString(course.code) + "'");
    }
    if (db->columnExists(courseTable, nameEnCol)) {
        updates.push_back(nameEnCol + " = '" + db->escapeString(course.nameEn) + "'");
    }
    if (db->columnExists(courseTable, nameArCol)) {
        updates.push_back(nameArCol + " = '" + db->escapeString(course.nameAr) + "'");
    }
    if (db->columnExists(courseTable, descriptionCol)) {
        updates.push_back(descriptionCol + " = '" + db->escapeString(course.description) + "'");
    }
    if (db->columnExists(courseTable, departmentCol)) {
        updates.push_back(departmentCol + " = '" + db->escapeString(course.departmentId) + "'");
    }
    if (db->columnExists(courseTable, levelCol)) {
        updates.push_back(levelCol + " = '" + db->escapeString(course.academicLevelId) + "'");
    }
    if (db->columnExists(courseTable, semesterCol)) {
        updates.push_back(semesterCol + " = " + to_string(course.semesterNumber));
    }
    if (db->columnExists(courseTable, courseTypeCol)) {
        updates.push_back(courseTypeCol + " = '" + db->escapeString(course.courseTypeId) + "'");
    }
    if (db->columnExists(courseTable, creditHoursCol)) {
        updates.push_back(creditHoursCol + " = " + to_string(course.creditHours));
    }
    if (db->columnExists(courseTable, lectureHoursCol)) {
        updates.push_back(lectureHoursCol + " = " + to_string(course.lectureHours));
    }
    if (db->columnExists(courseTable, tutorialHoursCol)) {
        updates.push_back(tutorialHoursCol + " = " + to_string(course.tutorialHours));
    }
    if (db->columnExists(courseTable, labHoursCol)) {
        updates.push_back(labHoursCol + " = " + to_string(course.labHours));
    }
    if (db->columnExists(courseTable, practicalHoursCol)) {
        updates.push_back(practicalHoursCol + " = " + to_string(course.practicalHours));
    }
    if (db->columnExists(courseTable, maxMarksCol)) {
        updates.push_back(maxMarksCol + " = " + to_string(course.maxMarks));
    }
    if (db->columnExists(courseTable, assignment1Col)) {
        updates.push_back(assignment1Col + " = " + to_string(course.assignment1Marks));
    }
    if (db->columnExists(courseTable, assignment2Col)) {
        updates.push_back(assignment2Col + " = " + to_string(course.assignment2Marks));
    }
    if (db->columnExists(courseTable, yearWorkCol)) {
        updates.push_back(yearWorkCol + " = " + to_string(course.yearWorkMarks));
    }
    if (db->columnExists(courseTable, finalExamCol)) {
        updates.push_back(finalExamCol + " = " + to_string(course.finalExamMarks));
    }

    if (updates.empty()) {
        return false;
    }

    string query = "UPDATE " + courseTable + " SET ";
    for (size_t i = 0; i < updates.size(); ++i) {
        if (i > 0) query += ", ";
        query += updates[i];
    }
    query += " WHERE " + idCol + " = '" + db->escapeString(courseId) + "'";

    return db->executeUpdate(query) > 0;
}

bool CourseRepository::deleteCourse(const string& courseId) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return false;
    }

    string courseTable = db->resolveTable({"courses"});
    string idCol = db->resolveColumn(courseTable, {"id", "course_id", "courseId"});
    string query = "DELETE FROM " + courseTable + " WHERE " + idCol + " = '" + db->escapeString(courseId) + "'";
    return db->executeUpdate(query) > 0;
}
