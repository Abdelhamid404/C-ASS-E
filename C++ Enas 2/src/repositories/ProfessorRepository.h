#ifndef PROFESSORREPOSITORY_H
#define PROFESSORREPOSITORY_H

#include <iostream>
#include <string>
#include <vector>
#include "../Database.h"

using namespace std;

struct Professor {
    string id;
    int userId;
    string title;
    string firstName;
    string lastName;
    string email;
    string phone;
    string departmentId;
    string hireDate;
    string specialization;
    string fullName;
};

class ProfessorRepository {
private:
    Database* db;

public:
    ProfessorRepository(Database* database);
    
    bool getProfessorById(const string& professorId, Professor& professor);
    bool getProfessorByCode(const string& code, Professor& professor);
    vector<Professor> listAll();
    vector<Professor> listByDepartment(const string& departmentId);
    bool createProfessor(const Professor& professor, int createdBy);
    bool updateProfessor(const string& professorId, const Professor& professor, int updatedBy);
    bool deleteProfessor(const string& professorId);
};

#endif // PROFESSORREPOSITORY_H
