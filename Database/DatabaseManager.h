//
// Created by Aditya Shrey on 6/27/24.
//

#ifndef QUIZLITE_DATABASEMANAGER_H
#define QUIZLITE_DATABASEMANAGER_H

#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>
#include <map>

/**
 * @class DatabaseManager
 * @brief Manages a SQLite database connection using the singleton pattern.
 */
class DatabaseManager {
public:
    sqlite3* db;                    // Pointer to the SQLite database connection
    std::string dbName;             // Name of the database

    static DatabaseManager* instancePtr;

    /**
     * @brief Private constructor to initialize the database name and set the database pointer to nullptr.
     * @param databaseName The name of the database to manage.
     */
    explicit DatabaseManager(std::string databaseName);

    /**
     * @brief Destructor to ensure the database connection is closed when the DatabaseManager object is destroyed.
     */
    ~DatabaseManager();

    /**
     * @brief Static method to get the single instance of DatabaseManager, creating it if it doesn't exist.
     * @param databaseName The name of the database to manage.
     * @return The single instance of DatabaseManager.
     */
    static DatabaseManager* getDatabaseManager(const std::string& databaseName);

    /**
     * @brief Opens the SQLite database with the name stored in dbName.
     * @return True if the database was successfully opened, false otherwise.
     */
    bool openDatabase();

    /**
     * @brief Closes the SQLite database connection if it is open.
     */
    void closeDatabase();

    /**
     * @brief Executes an SQL query on the database.
     * @param query The SQL query to execute.
     * @return True if the query was successfully executed, false otherwise.
     */
    bool executeQuery(const std::string& query);

    /**
     * @brief Executes an SQL query on the database.
     * @param query The SQL query to execute.
     * @return  A vector of maps where each map represents a row in the result set,
     *         with column names as keys and column values as values.
     */
    std::vector<std::map<std::string, std::string>> executeQueryWithResults(const std::string& query);

    /**
     * @brief Prints all rows from the specified table.
     * @param tableName The name of the table to print.
     */
    void printDatabaseTable(const std::string& tableName);

    friend class UserSessionInfo;
public:
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;
};

#endif //QUIZLITE_DATABASEMANAGER_H
