#include "GradeService.h"

namespace {
string buildGradePointsCase(const string& percentageExpr) {
    return "CASE "
           "WHEN " + percentageExpr + " >= 90 THEN 4.0 "
           "WHEN " + percentageExpr + " >= 85 THEN 3.7 "
           "WHEN " + percentageExpr + " >= 80 THEN 3.3 "
           "WHEN " + percentageExpr + " >= 75 THEN 3.0 "
           "WHEN " + percentageExpr + " >= 70 THEN 2.7 "
           "WHEN " + percentageExpr + " >= 65 THEN 2.3 "
           "WHEN " + percentageExpr + " >= 60 THEN 2.0 "
           "WHEN " + percentageExpr + " >= 55 THEN 1.7 "
           "WHEN " + percentageExpr + " >= 50 THEN 1.3 "
           "WHEN " + percentageExpr + " >= 45 THEN 1.0 "
           "WHEN " + percentageExpr + " >= 40 THEN 0.7 "
           "ELSE 0.0 END";
}
}

GradeService::GradeService(Database* database, GradeRepository* gradeRepository) 
    : db(database), gradeRepo(gradeRepository) {
}

GradeCalculation GradeService::calculateGrade(double assignment1, double assignment2, double yearWork,
                                              double midterm, double finalExam, int maxMarks) {
    GradeCalculation calc;
    
    // Calculate total marks
    calc.totalMarks = assignment1 + assignment2 + yearWork + midterm + finalExam;
    
    // Calculate percentage
    calc.percentage = (calc.totalMarks / maxMarks) * 100.0;
    
    // Get letter grade, evaluation, and grade points
    calc.letterGrade = getLetterGrade(calc.percentage);
    calc.evaluation = getEvaluation(calc.percentage);
    calc.gradePoints = getGradePoints(calc.percentage);
    
    return calc;
}

string GradeService::getLetterGrade(double percentage) {
    if (percentage >= 90) return "A+";
    if (percentage >= 85) return "A";
    if (percentage >= 80) return "A-";
    if (percentage >= 75) return "B+";
    if (percentage >= 70) return "B";
    if (percentage >= 65) return "B-";
    if (percentage >= 60) return "C+";
    if (percentage >= 55) return "C";
    if (percentage >= 50) return "C-";
    if (percentage >= 45) return "D+";
    if (percentage >= 40) return "D";
    return "F";
}

string GradeService::getEvaluation(double percentage) {
    if (percentage >= 90) return "Excellent";
    if (percentage >= 80) return "Very Good";
    if (percentage >= 70) return "Good";
    if (percentage >= 60) return "Fair";
    if (percentage >= 50) return "Pass";
    if (percentage >= 40) return "Weak";
    return "Fail";
}

double GradeService::getGradePoints(double percentage) {
    if (percentage >= 90) return 4.0;
    if (percentage >= 85) return 3.7;
    if (percentage >= 80) return 3.3;
    if (percentage >= 75) return 3.0;
    if (percentage >= 70) return 2.7;
    if (percentage >= 65) return 2.3;
    if (percentage >= 60) return 2.0;
    if (percentage >= 55) return 1.7;
    if (percentage >= 50) return 1.3;
    if (percentage >= 45) return 1.0;
    if (percentage >= 40) return 0.7;
    return 0.0;
}

double GradeService::calculateGPA(const string& studentId, const string& semesterId) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return 0.0;
    }

    string gradeTable = db->resolveTable({"grades", "student_grades", "grade"});
    string courseTable = db->resolveTable({"courses", "course"});
    if (!db->tableExists(gradeTable) || !db->tableExists(courseTable)) {
        return 0.0;
    }

    string gradePointsCol = db->findColumn(gradeTable, {"grade_points", "gradePoints", "grade_point", "gradePoint", "points", "gpa"});
    string gradeCourseIdCol = db->findColumn(gradeTable, {"course_id", "courseId"});
    string gradeCourseCodeCol = db->findColumn(gradeTable, {"course_code", "courseCode", "course"});
    string gradeStudentCol = db->findColumn(gradeTable, {"student_id", "studentId", "student_code", "studentCode"});
    string gradeSemesterCol = db->findColumn(gradeTable, {"semester_id", "semesterId", "term_id", "termId"});
    string percentageCol = db->findColumn(gradeTable, {"percentage", "percent"});
    string totalMarksCol = db->findColumn(gradeTable, {"total_marks", "totalMarks", "total"});

    string courseIdCol = db->findColumn(courseTable, {"id", "course_id", "courseId"});
    string courseCodeCol = db->findColumn(courseTable, {"code", "course_code", "courseCode"});
    string creditHoursCol = db->findColumn(courseTable, {"credit_hours", "creditHours", "credit_hour", "credit", "credits"});
    string courseMaxCol = db->findColumn(courseTable, {"max_marks", "maxMarks", "maximum_marks", "max_mark"});

    if (gradeStudentCol.empty()) {
        return 0.0;
    }

    string gradeCourseCol;
    string courseJoinCol;
    if (!gradeCourseIdCol.empty() && !courseIdCol.empty()) {
        gradeCourseCol = gradeCourseIdCol;
        courseJoinCol = courseIdCol;
    } else if (!gradeCourseCodeCol.empty() && !courseCodeCol.empty()) {
        gradeCourseCol = gradeCourseCodeCol;
        courseJoinCol = courseCodeCol;
    } else if (!gradeCourseIdCol.empty() && !courseCodeCol.empty()) {
        gradeCourseCol = gradeCourseIdCol;
        courseJoinCol = courseCodeCol;
    } else if (!gradeCourseCodeCol.empty() && !courseIdCol.empty()) {
        gradeCourseCol = gradeCourseCodeCol;
        courseJoinCol = courseIdCol;
    } else {
        return 0.0;
    }

    string gradePointsExpr;
    if (!gradePointsCol.empty()) {
        gradePointsExpr = "g." + gradePointsCol;
    } else {
        string percentageExpr;
        if (!percentageCol.empty()) {
            percentageExpr = "g." + percentageCol;
        } else if (!totalMarksCol.empty() && !courseMaxCol.empty()) {
            percentageExpr = "(g." + totalMarksCol + " / NULLIF(c." + courseMaxCol + ", 0)) * 100";
        } else {
            return 0.0;
        }
        gradePointsExpr = buildGradePointsCase(percentageExpr);
    }

    string creditHoursExpr = creditHoursCol.empty() ? "1" : ("c." + creditHoursCol);

    string query = "SELECT SUM(" + gradePointsExpr + " * " + creditHoursExpr + ") as total_points, "
                  "SUM(" + creditHoursExpr + ") as total_hours "
                  "FROM " + gradeTable + " g "
                  "JOIN " + courseTable + " c ON g." + gradeCourseCol + " = c." + courseJoinCol + " "
                  "WHERE g." + gradeStudentCol + " = '" + db->escapeString(studentId) + "'";
    if (!gradeSemesterCol.empty() && !semesterId.empty()) {
        query += " AND g." + gradeSemesterCol + " = '" + db->escapeString(semesterId) + "'";
    }

    auto results = db->executeQuery(query);
    
    if (results && results->next()) {
        double totalPoints = results->getDouble("total_points");
        double totalHours = results->getDouble("total_hours");
        if (totalHours > 0) {
            return totalPoints / totalHours;
        }
    }

    return 0.0;
}

double GradeService::calculateCGPA(const string& studentId) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return 0.0;
    }

    string gradeTable = db->resolveTable({"grades", "student_grades", "grade"});
    string courseTable = db->resolveTable({"courses", "course"});
    if (!db->tableExists(gradeTable) || !db->tableExists(courseTable)) {
        return 0.0;
    }

    string gradePointsCol = db->findColumn(gradeTable, {"grade_points", "gradePoints", "grade_point", "gradePoint", "points", "gpa"});
    string gradeCourseIdCol = db->findColumn(gradeTable, {"course_id", "courseId"});
    string gradeCourseCodeCol = db->findColumn(gradeTable, {"course_code", "courseCode", "course"});
    string gradeStudentCol = db->findColumn(gradeTable, {"student_id", "studentId", "student_code", "studentCode"});
    string percentageCol = db->findColumn(gradeTable, {"percentage", "percent"});
    string totalMarksCol = db->findColumn(gradeTable, {"total_marks", "totalMarks", "total"});

    string courseIdCol = db->findColumn(courseTable, {"id", "course_id", "courseId"});
    string courseCodeCol = db->findColumn(courseTable, {"code", "course_code", "courseCode"});
    string creditHoursCol = db->findColumn(courseTable, {"credit_hours", "creditHours", "credit_hour", "credit", "credits"});
    string courseMaxCol = db->findColumn(courseTable, {"max_marks", "maxMarks", "maximum_marks", "max_mark"});

    if (gradeStudentCol.empty()) {
        return 0.0;
    }

    string gradeCourseCol;
    string courseJoinCol;
    if (!gradeCourseIdCol.empty() && !courseIdCol.empty()) {
        gradeCourseCol = gradeCourseIdCol;
        courseJoinCol = courseIdCol;
    } else if (!gradeCourseCodeCol.empty() && !courseCodeCol.empty()) {
        gradeCourseCol = gradeCourseCodeCol;
        courseJoinCol = courseCodeCol;
    } else if (!gradeCourseIdCol.empty() && !courseCodeCol.empty()) {
        gradeCourseCol = gradeCourseIdCol;
        courseJoinCol = courseCodeCol;
    } else if (!gradeCourseCodeCol.empty() && !courseIdCol.empty()) {
        gradeCourseCol = gradeCourseCodeCol;
        courseJoinCol = courseIdCol;
    } else {
        return 0.0;
    }

    string gradePointsExpr;
    if (!gradePointsCol.empty()) {
        gradePointsExpr = "g." + gradePointsCol;
    } else {
        string percentageExpr;
        if (!percentageCol.empty()) {
            percentageExpr = "g." + percentageCol;
        } else if (!totalMarksCol.empty() && !courseMaxCol.empty()) {
            percentageExpr = "(g." + totalMarksCol + " / NULLIF(c." + courseMaxCol + ", 0)) * 100";
        } else {
            return 0.0;
        }
        gradePointsExpr = buildGradePointsCase(percentageExpr);
    }

    string creditHoursExpr = creditHoursCol.empty() ? "1" : ("c." + creditHoursCol);

    string query = "SELECT SUM(" + gradePointsExpr + " * " + creditHoursExpr + ") as total_points, "
                  "SUM(" + creditHoursExpr + ") as total_hours "
                  "FROM " + gradeTable + " g "
                  "JOIN " + courseTable + " c ON g." + gradeCourseCol + " = c." + courseJoinCol + " "
                  "WHERE g." + gradeStudentCol + " = '" + db->escapeString(studentId) + "'";

    auto results = db->executeQuery(query);
    
    if (results && results->next()) {
        double totalPoints = results->getDouble("total_points");
        double totalHours = results->getDouble("total_hours");
        if (totalHours > 0) {
            return totalPoints / totalHours;
        }
    }

    return 0.0;
}
