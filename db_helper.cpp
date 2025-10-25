#include <sqlite3.h>
#include <iostream>
#include "db_helper.h"

void saveExpense(const std::string& vendor, const std::string& category, double amount) {
    sqlite3* db;
    char* errMsg = nullptr;
    int rc = sqlite3_open("expenses.db", &db);

    if (rc) {
        std::cerr << " Can't open database: " << sqlite3_errmsg(db) << "\n";
        return;
    }

    std::string createTable = "CREATE TABLE IF NOT EXISTS expenses("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "vendor TEXT, category TEXT, amount REAL);";
    sqlite3_exec(db, createTable.c_str(), nullptr, nullptr, &errMsg);

    std::string sql = "INSERT INTO expenses (vendor, category, amount) VALUES ('" + vendor + "', '" + category + "', " + std::to_string(amount) + ");";
    rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
        std::cerr << " SQL error: " << errMsg << "\n";

    sqlite3_close(db);
}

void showSummary() {
    sqlite3* db;
    sqlite3_open("expenses.db", &db);
    sqlite3_stmt* stmt;

    std::string query = "SELECT category, SUM(amount) FROM expenses GROUP BY category;";
    sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0);

    std::cout << "\n Category Summary:\n";
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string cat = (const char*)sqlite3_column_text(stmt, 0);
        double total = sqlite3_column_double(stmt, 1);
        std::cout << "- " << cat << ": ₹" << total << "\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
