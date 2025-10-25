#ifndef DB_HELPER_H
#define DB_HELPER_H

#include <string>
void saveExpense(const std::string& vendor, const std::string& category, double amount);
void showSummary();

#endif
