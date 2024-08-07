//
// Created by Aditya Shrey on 6/27/24.
//
#include "DatabaseManager.h"

DatabaseManager* DatabaseManager::instancePtr = nullptr;
DatabaseManager::DatabaseManager(std::string databaseName)
        : db(nullptr)
        , dbName(std::move(databaseName))
{
}

DatabaseManager::~DatabaseManager()
{
    closeDatabase();
}

DatabaseManager* DatabaseManager::getDatabaseManager(
        const std::string& databaseName)
{
    if (instancePtr == nullptr) {
        instancePtr = new DatabaseManager(databaseName);
    }
    return (instancePtr);
}

bool DatabaseManager::openDatabase()
{
    int result = sqlite3_open(dbName.c_str(), &db);
    if (result != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    return true;
}

void DatabaseManager::closeDatabase()
{
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

int DatabaseManager::executeQuery(const std::string& query, std::vector<std::string> params) const
{
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    for (size_t i = 0; i < params.size(); ++i) {
        sqlite3_bind_text(stmt, static_cast<int>(i + 1), params[i].c_str(), -1, SQLITE_STATIC);
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return rc;
}


static int callbackStore(void* data, int argc, char** argv, char** azColName)
{
    auto* rows = static_cast<std::vector<std::map<std::string, std::string>>*>(data);
    std::map<std::string, std::string> row;
    for (int i = 0; i < argc; i++) {
        row[azColName[i]] = argv[i] ? argv[i] : "NULL";
    }
    rows->push_back(row);
    return 0;
}

std::vector<std::map<std::string, std::string>> DatabaseManager::executeQueryWithResults(const std::string& query, std::vector<std::string> params) const
{
    std::vector<std::map<std::string, std::string>> rows;
    if (!db) {
        std::cerr << "Database connection is NULL." << std::endl;
        return rows;
    }

    sqlite3_stmt *stmt;
    int result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        std::cerr << "SQL is error: " << sqlite3_errmsg(db) << std::endl;
        return rows;
    }

    for (size_t i = 0; i < params.size(); ++i) {
        sqlite3_bind_text(stmt, static_cast<int>(i + 1), params[i].c_str(), -1, SQLITE_STATIC);
    }

    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        std::map<std::string, std::string> row;
        for (int i = 0; i < sqlite3_column_count(stmt); ++i) {
            const char *columnName = sqlite3_column_name(stmt, i);
            const char *value = reinterpret_cast<const char *>(sqlite3_column_text(stmt, i));
            row[columnName] = value ? value : "NULL";
        }
        rows.push_back(row);
    }

    if (result != SQLITE_DONE) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return rows;
}

static int callbackPrint(void* data, int argc, char** argv, char** azColName)
{
    (void)data; // Suppress warning about unused variable
    for (int i = 0; i < argc; i++) {
        std::cout << (azColName[i] ? azColName[i] : "NULL") << ": "
                  << (argv[i] ? argv[i] : "NULL") << "\n";
    }
    std::cout << "\n";
    return 0;
}

void DatabaseManager::printDatabaseTable(const std::string &tableName) const
{
    std::string query = "SELECT * FROM " + tableName;
    char* errorMessage = nullptr;
    int result = sqlite3_exec(db, query.c_str(), callbackPrint, nullptr, &errorMessage);
    if (result != SQLITE_OK) {
        std::cerr << "SQL error: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
}

bool DatabaseManager::isTableEmpty(const std::string& tableName) const
{
    std::string query = "SELECT COUNT(*) FROM " + tableName;
    sqlite3_stmt* stmt;
    bool isEmpty = false;

    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        printDatabaseTable("set_names");
        isEmpty = (count == 0);
    } else {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
        isEmpty = false;
    }

    sqlite3_finalize(stmt);
    return isEmpty;
}

#ifdef TESTING
void DatabaseManager::resetInstance()
{
    if (instancePtr) {
        delete instancePtr;
        instancePtr = nullptr;
    }
}
#endif