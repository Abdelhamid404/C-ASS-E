#include "ProfessorRepository.h"

namespace {
Professor buildProfessor(ResultSet* results,
                         const std::string& idCol,
                         const std::string& titleCol,
                         const std::string& firstNameCol,
                         const std::string& lastNameCol,
                         const std::string& emailCol,
                         const std::string& phoneCol,
                         const std::string& departmentCol,
                         const std::string& hireDateCol,
                         const std::string& specializationCol) {
    Professor professor;
    professor.id = results->getString(idCol);
    professor.userId = results->getString("user_id").empty() ? 0 : std::stoi(results->getString("user_id"));
    professor.title = results->getString(titleCol);
    professor.firstName = results->getString(firstNameCol);
    professor.lastName = results->getString(lastNameCol);
    professor.email = results->getString(emailCol);
    professor.phone = results->getString(phoneCol);
    professor.departmentId = results->getString(departmentCol);
    professor.hireDate = results->getString(hireDateCol);
    professor.specialization = results->getString(specializationCol);
    professor.fullName = results->getString("full_name");
    return professor;
}
}

ProfessorRepository::ProfessorRepository(Database* database) : db(database) {
}

bool ProfessorRepository::getProfessorById(const string& professorId, Professor& professor) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return false;
    }

    string professorTable = db->resolveTable({"professors"});
    string userTable = db->resolveTable({"users"});
    string idCol = db->resolveColumn(professorTable, {"id", "professor_id", "professorId"});
    string userIdCol = db->resolveColumn(professorTable, {"user_id", "userId"});
    string titleCol = db->resolveColumn(professorTable, {"title"});
    string firstNameCol = db->resolveColumn(professorTable, {"first_name", "firstName"});
    string lastNameCol = db->resolveColumn(professorTable, {"last_name", "lastName"});
    string emailCol = db->resolveColumn(professorTable, {"email"});
    string phoneCol = db->resolveColumn(professorTable, {"phone"});
    string departmentCol = db->resolveColumn(professorTable, {"department_id", "departmentId"});
    string hireDateCol = db->resolveColumn(professorTable, {"hire_date", "hireDate"});
    string specializationCol = db->resolveColumn(professorTable, {"specialization"});
    string userPkCol = db->resolveColumn(userTable, {"id", "user_id", "userId"});

    string query = "SELECT p.*, u." + userPkCol + " as user_id, "
                  "CONCAT(COALESCE(p." + titleCol + ", ''), ' ', p." + firstNameCol + ", ' ', p." + lastNameCol + ") as full_name "
                  "FROM " + professorTable + " p "
                  "LEFT JOIN " + userTable + " u ON p." + userIdCol + " = u." + userPkCol + " "
                  "WHERE p." + idCol + " = '" + db->escapeString(professorId) + "'";

    auto results = db->executeQuery(query);

    if (results && results->next()) {
        professor = buildProfessor(results.get(), idCol, titleCol, firstNameCol, lastNameCol, emailCol,
                                   phoneCol, departmentCol, hireDateCol, specializationCol);
        return true;
    }

    return false;
}

bool ProfessorRepository::getProfessorByCode(const string& code, Professor& professor) {
    return getProfessorById(code, professor);
}

vector<Professor> ProfessorRepository::listAll() {
    vector<Professor> professors;

    if (!db || !db->isConnected()) {
        if (!db->connect()) return professors;
    }

    string professorTable = db->resolveTable({"professors"});
    string userTable = db->resolveTable({"users"});
    string idCol = db->resolveColumn(professorTable, {"id", "professor_id", "professorId"});
    string userIdCol = db->resolveColumn(professorTable, {"user_id", "userId"});
    string titleCol = db->resolveColumn(professorTable, {"title"});
    string firstNameCol = db->resolveColumn(professorTable, {"first_name", "firstName"});
    string lastNameCol = db->resolveColumn(professorTable, {"last_name", "lastName"});
    string emailCol = db->resolveColumn(professorTable, {"email"});
    string phoneCol = db->resolveColumn(professorTable, {"phone"});
    string departmentCol = db->resolveColumn(professorTable, {"department_id", "departmentId"});
    string hireDateCol = db->resolveColumn(professorTable, {"hire_date", "hireDate"});
    string specializationCol = db->resolveColumn(professorTable, {"specialization"});
    string userPkCol = db->resolveColumn(userTable, {"id", "user_id", "userId"});

    string query = "SELECT p.*, u." + userPkCol + " as user_id, "
                  "CONCAT(COALESCE(p." + titleCol + ", ''), ' ', p." + firstNameCol + ", ' ', p." + lastNameCol + ") as full_name "
                  "FROM " + professorTable + " p "
                  "LEFT JOIN " + userTable + " u ON p." + userIdCol + " = u." + userPkCol + " "
                  "ORDER BY p." + idCol;

    auto results = db->executeQuery(query);

    if (results) {
        while (results->next()) {
            professors.push_back(buildProfessor(results.get(), idCol, titleCol, firstNameCol, lastNameCol, emailCol,
                                               phoneCol, departmentCol, hireDateCol, specializationCol));
        }
    }

    return professors;
}

vector<Professor> ProfessorRepository::listByDepartment(const string& departmentId) {
    vector<Professor> professors;

    if (!db || !db->isConnected()) {
        if (!db->connect()) return professors;
    }

    string professorTable = db->resolveTable({"professors"});
    string userTable = db->resolveTable({"users"});
    string idCol = db->resolveColumn(professorTable, {"id", "professor_id", "professorId"});
    string userIdCol = db->resolveColumn(professorTable, {"user_id", "userId"});
    string titleCol = db->resolveColumn(professorTable, {"title"});
    string firstNameCol = db->resolveColumn(professorTable, {"first_name", "firstName"});
    string lastNameCol = db->resolveColumn(professorTable, {"last_name", "lastName"});
    string emailCol = db->resolveColumn(professorTable, {"email"});
    string phoneCol = db->resolveColumn(professorTable, {"phone"});
    string departmentCol = db->resolveColumn(professorTable, {"department_id", "departmentId"});
    string hireDateCol = db->resolveColumn(professorTable, {"hire_date", "hireDate"});
    string specializationCol = db->resolveColumn(professorTable, {"specialization"});
    string userPkCol = db->resolveColumn(userTable, {"id", "user_id", "userId"});

    string query = "SELECT p.*, u." + userPkCol + " as user_id, "
                  "CONCAT(COALESCE(p." + titleCol + ", ''), ' ', p." + firstNameCol + ", ' ', p." + lastNameCol + ") as full_name "
                  "FROM " + professorTable + " p "
                  "LEFT JOIN " + userTable + " u ON p." + userIdCol + " = u." + userPkCol + " "
                  "WHERE p." + departmentCol + " = '" + db->escapeString(departmentId) + "' "
                  "ORDER BY p." + idCol;

    auto results = db->executeQuery(query);

    if (results) {
        while (results->next()) {
            professors.push_back(buildProfessor(results.get(), idCol, titleCol, firstNameCol, lastNameCol, emailCol,
                                               phoneCol, departmentCol, hireDateCol, specializationCol));
        }
    }

    return professors;
}

bool ProfessorRepository::createProfessor(const Professor& professor, int createdBy) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return false;
    }

    string userTable = db->resolveTable({"users"});
    string professorTable = db->resolveTable({"professors"});

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
    userVals.push_back("'" + db->escapeString(professor.id) + "'");
    userCols.push_back(passwordCol);
    userVals.push_back("'prof123'");
    userCols.push_back(fullNameCol);
    userVals.push_back("'" + db->escapeString(professor.firstName + " " + professor.lastName) + "'");
    userCols.push_back(roleCol);
    userVals.push_back("'ROLE_PROFESSOR'");
    if (db->columnExists(userTable, emailCol)) {
        userCols.push_back(emailCol);
        userVals.push_back("'" + db->escapeString(professor.email) + "'");
    }
    if (db->columnExists(userTable, phoneCol)) {
        userCols.push_back(phoneCol);
        userVals.push_back("'" + db->escapeString(professor.phone) + "'");
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

    string idCol = db->resolveColumn(professorTable, {"id", "professor_id", "professorId"});
    string userIdCol = db->resolveColumn(professorTable, {"user_id", "userId"});
    string titleCol = db->resolveColumn(professorTable, {"title"});
    string firstNameCol = db->resolveColumn(professorTable, {"first_name", "firstName"});
    string lastNameCol = db->resolveColumn(professorTable, {"last_name", "lastName"});
    string emailColProf = db->resolveColumn(professorTable, {"email"});
    string phoneColProf = db->resolveColumn(professorTable, {"phone"});
    string departmentCol = db->resolveColumn(professorTable, {"department_id", "departmentId"});
    string hireDateCol = db->resolveColumn(professorTable, {"hire_date", "hireDate"});
    string specializationCol = db->resolveColumn(professorTable, {"specialization"});

    vector<string> profCols;
    vector<string> profVals;
    profCols.push_back(idCol);
    profVals.push_back("'" + db->escapeString(professor.id) + "'");
    if (db->columnExists(professorTable, userIdCol)) {
        profCols.push_back(userIdCol);
        profVals.push_back(to_string(userId));
    }
    if (db->columnExists(professorTable, titleCol)) {
        profCols.push_back(titleCol);
        profVals.push_back("'" + db->escapeString(professor.title) + "'");
    }
    profCols.push_back(firstNameCol);
    profVals.push_back("'" + db->escapeString(professor.firstName) + "'");
    profCols.push_back(lastNameCol);
    profVals.push_back("'" + db->escapeString(professor.lastName) + "'");
    if (db->columnExists(professorTable, emailColProf)) {
        profCols.push_back(emailColProf);
        profVals.push_back("'" + db->escapeString(professor.email) + "'");
    }
    if (db->columnExists(professorTable, phoneColProf)) {
        profCols.push_back(phoneColProf);
        profVals.push_back("'" + db->escapeString(professor.phone) + "'");
    }
    if (db->columnExists(professorTable, departmentCol)) {
        profCols.push_back(departmentCol);
        profVals.push_back("'" + db->escapeString(professor.departmentId) + "'");
    }
    if (db->columnExists(professorTable, hireDateCol)) {
        if (professor.hireDate.empty()) {
            profCols.push_back(hireDateCol);
            profVals.push_back("NULL");
        } else {
            profCols.push_back(hireDateCol);
            profVals.push_back("'" + db->escapeString(professor.hireDate) + "'");
        }
    }
    if (db->columnExists(professorTable, specializationCol)) {
        profCols.push_back(specializationCol);
        profVals.push_back("'" + db->escapeString(professor.specialization) + "'");
    }

    string profQuery = "INSERT INTO " + professorTable + " (";
    for (size_t i = 0; i < profCols.size(); ++i) {
        if (i > 0) profQuery += ", ";
        profQuery += profCols[i];
    }
    profQuery += ") VALUES (";
    for (size_t i = 0; i < profVals.size(); ++i) {
        if (i > 0) profQuery += ", ";
        profQuery += profVals[i];
    }
    profQuery += ")";

    return db->executeUpdate(profQuery) > 0;
}

bool ProfessorRepository::updateProfessor(const string& professorId, const Professor& professor, int updatedBy) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return false;
    }

    string professorTable = db->resolveTable({"professors"});
    string idCol = db->resolveColumn(professorTable, {"id", "professor_id", "professorId"});
    string titleCol = db->resolveColumn(professorTable, {"title"});
    string firstNameCol = db->resolveColumn(professorTable, {"first_name", "firstName"});
    string lastNameCol = db->resolveColumn(professorTable, {"last_name", "lastName"});
    string emailCol = db->resolveColumn(professorTable, {"email"});
    string phoneCol = db->resolveColumn(professorTable, {"phone"});
    string departmentCol = db->resolveColumn(professorTable, {"department_id", "departmentId"});
    string hireDateCol = db->resolveColumn(professorTable, {"hire_date", "hireDate"});
    string specializationCol = db->resolveColumn(professorTable, {"specialization"});

    vector<string> updates;
    if (db->columnExists(professorTable, titleCol)) {
        updates.push_back(titleCol + " = '" + db->escapeString(professor.title) + "'");
    }
    if (db->columnExists(professorTable, firstNameCol)) {
        updates.push_back(firstNameCol + " = '" + db->escapeString(professor.firstName) + "'");
    }
    if (db->columnExists(professorTable, lastNameCol)) {
        updates.push_back(lastNameCol + " = '" + db->escapeString(professor.lastName) + "'");
    }
    if (db->columnExists(professorTable, emailCol)) {
        updates.push_back(emailCol + " = '" + db->escapeString(professor.email) + "'");
    }
    if (db->columnExists(professorTable, phoneCol)) {
        updates.push_back(phoneCol + " = '" + db->escapeString(professor.phone) + "'");
    }
    if (db->columnExists(professorTable, departmentCol)) {
        updates.push_back(departmentCol + " = '" + db->escapeString(professor.departmentId) + "'");
    }
    if (db->columnExists(professorTable, hireDateCol)) {
        updates.push_back(hireDateCol + " = " + (professor.hireDate.empty() ? "NULL" : "'" + db->escapeString(professor.hireDate) + "'"));
    }
    if (db->columnExists(professorTable, specializationCol)) {
        updates.push_back(specializationCol + " = '" + db->escapeString(professor.specialization) + "'");
    }

    if (updates.empty()) {
        return false;
    }

    string query = "UPDATE " + professorTable + " SET ";
    for (size_t i = 0; i < updates.size(); ++i) {
        if (i > 0) query += ", ";
        query += updates[i];
    }
    query += " WHERE " + idCol + " = '" + db->escapeString(professorId) + "'";

    return db->executeUpdate(query) > 0;
}

bool ProfessorRepository::deleteProfessor(const string& professorId) {
    if (!db || !db->isConnected()) {
        if (!db->connect()) return false;
    }

    string professorTable = db->resolveTable({"professors"});
    string userTable = db->resolveTable({"users"});
    string idCol = db->resolveColumn(professorTable, {"id", "professor_id", "professorId"});
    string userIdCol = db->resolveColumn(professorTable, {"user_id", "userId"});
    string userPkCol = db->resolveColumn(userTable, {"id", "user_id", "userId"});
    string activeCol = db->resolveColumn(userTable, {"is_active", "active", "isActive"});

    string fetchQuery = "SELECT " + userIdCol + " as user_id FROM " + professorTable + " "
                        "WHERE " + idCol + " = '" + db->escapeString(professorId) + "'";
    auto fetchRes = db->executeQuery(fetchQuery);
    int userId = 0;
    if (fetchRes && fetchRes->next()) {
        userId = fetchRes->getInt("user_id");
    }

    if (userId > 0 && db->columnExists(userTable, activeCol)) {
        string updateUser = "UPDATE " + userTable + " SET " + activeCol + " = FALSE "
                            "WHERE " + userPkCol + " = " + to_string(userId);
        db->executeUpdate(updateUser);
    }

    string deleteQuery = "DELETE FROM " + professorTable + " WHERE " + idCol + " = '" + db->escapeString(professorId) + "'";
    return db->executeUpdate(deleteQuery) > 0;
}
