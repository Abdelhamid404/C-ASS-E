#ifndef GRADESERVICE_H
#define GRADESERVICE_H

#include <iostream>
#include <string>
#include "Database.h"
#include "repositories/GradeRepository.h"

using namespace std;

struct GradeCalculation {
    double totalMarks;
    double percentage;
    double gradePoints;
    string letterGrade;
    string evaluation;
};

class GradeService {
private:
    Database* db;
    GradeRepository* gradeRepo;

public:
    GradeService(Database* database, GradeRepository* gradeRepository);
    
    // Calculate grade components automatically
    GradeCalculation calculateGrade(double assignment1, double assignment2, double yearWork,
                                   double midterm, double finalExam, int maxMarks = 100);
    
    // Get letter grade from percentage
    string getLetterGrade(double percentage);
    
    // Get evaluation from percentage
    string getEvaluation(double percentage);
    
    // Get grade points from percentage
    double getGradePoints(double percentage);
    
    // Calculate GPA for a student for a semester
    double calculateGPA(const string& studentId, const string& semesterId);
    
    // Calculate cumulative GPA for a student
    double calculateCGPA(const string& studentId);
};

#endif // GRADESERVICE_H
