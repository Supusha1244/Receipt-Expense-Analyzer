#include <sqlite3.h>
#include <iostream>
#include <fstream>
#include <string>
#include "db_helper.h"

/* ---------- Database path ---------- */
const char* DB_PATH =
"C:/Users/ilp22/source/repos/Recipt Analyzer/x64/Debug/expenses.db";

/* ---------- Duplicate check ---------- */
bool isDuplicate(sqlite3* db,
    const std::string& vendor,
    double amount,
    const std::string& date) {

    const char* query =
        "SELECT COUNT(*) FROM expenses "
        "WHERE vendor=? AND amount=? AND date=?;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, vendor.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 2, amount);
    sqlite3_bind_text(stmt, 3, date.c_str(), -1, SQLITE_TRANSIENT);

    sqlite3_step(stmt);
    int count = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);
    return count > 0;
}

/* ---------- Save expense ---------- */
void saveExpense(const std::string& vendor,
    const std::string& category,
    double amount,
    const std::string& date) {

    sqlite3* db;
    if (sqlite3_open(DB_PATH, &db) != SQLITE_OK) {
        std::cerr << " Cannot open database\n";
        return;
    }

    const char* createTable =
        "CREATE TABLE IF NOT EXISTS expenses ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "vendor TEXT, "
        "category TEXT, "
        "amount REAL, "
        "date TEXT);";

    sqlite3_exec(db, createTable, nullptr, nullptr, nullptr);

    if (isDuplicate(db, vendor, amount, date)) {
        std::cout << " Duplicate entry. Skipping save.\n";
        sqlite3_close(db);
        return;
    }

    const char* insertSQL =
        "INSERT INTO expenses (vendor, category, amount, date) "
        "VALUES (?, ?, ?, ?);";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, vendor.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, category.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 3, amount);
    sqlite3_bind_text(stmt, 4, date.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        std::cout << " Saved: "
            << vendor << " | "
            << category << " | Rs "
            << amount << " | "
            << date << "\n";
    }
    else {
        std::cerr << " Insert failed\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

/* ---------- Category summary ---------- */
void showSummary() {
    sqlite3* db;
    sqlite3_stmt* stmt;

    sqlite3_open(DB_PATH, &db);

    const char* query =
        "SELECT category, SUM(amount) "
        "FROM expenses GROUP BY category;";

    sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);

    std::cout << "\n Category Summary:\n";
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::cout << "- "
            << sqlite3_column_text(stmt, 0)
            << ": Rs "
            << sqlite3_column_double(stmt, 1)
            << "\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

/* ---------- CSV Export ---------- */
void exportToCSV() {
    sqlite3* db;
    sqlite3_stmt* stmt;

    sqlite3_open(DB_PATH, &db);

    std::ofstream file("expenses.csv");
    if (!file.is_open()) {
        std::cerr << " Cannot open CSV file\n";
        return;
    }

    file << "Vendor,Category,Amount,Date\n";

    const char* query =
        "SELECT vendor, category, amount, date FROM expenses;";

    sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);

    int rows = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        rows++;
        file << sqlite3_column_text(stmt, 0) << ","
            << sqlite3_column_text(stmt, 1) << ","
            << sqlite3_column_double(stmt, 2) << ","
            << sqlite3_column_text(stmt, 3) << "\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    file.close();

    std::cout << " CSV exported (" << rows << " rows)\n";
}





