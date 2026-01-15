#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <sstream>
#include <ctime>

#include "tesseract_helper.h"
#include "openai_client.h"
#include "db_helper.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

// ---------- Read API key from .env ----------
string getAPIKey() {
    ifstream file(".env");
    if (!file.is_open()) return "";

    string line;
    while (getline(file, line)) {
        if (line.find("GROQ_API_KEY=") == 0) {
            return line.substr(string("GROQ_API_KEY=").length());
        }
    }
    return "";
}

// ---------- Get current date (YYYY-MM-DD) ----------
string getCurrentDate() {
    time_t now = time(0);
    tm localTime;
    localtime_s(&localTime, &now);

    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", &localTime);
    return string(buffer);
}

// ---------- Extract total amount (fallback) ----------
double extractTotalAmount(const string& text) {
    regex pattern("(Total|TOTAL|Amount|AMOUNT)[^0-9]*([0-9]+)");
    smatch match;

    if (regex_search(text, match, pattern))
        return stod(match[2]);

    return 0.0;
}

// ---------- Vendor auto-detection (fallback) ----------
string detectVendor(const string& text) {
    istringstream iss(text);
    string line;

    while (getline(iss, line)) {
        if (line.length() > 2 && isalpha(line[0])) {
            return line;
        }
    }
    return "Unknown";
}

// ---------- Offline category fallback ----------
string offlineCategory(const string& text) {
    if (text.find("Milk") != string::npos || text.find("Bread") != string::npos)
        return "Grocery";
    if (text.find("Amazon") != string::npos)
        return "Shopping";
    if (text.find("Uber") != string::npos || text.find("Taxi") != string::npos)
        return "Travel";
    return "Other";
}

int main() {
    string apiKey = getAPIKey();
    if (apiKey.empty()) {
        cerr << "Could not open .env file OR API key missing\n";
        return 1;
    }

    while (true) {
        cout << "\n====== Expense Analyzer ======\n";
        cout << "1. Add new receipt\n";
        cout << "2. View summary\n";
        cout << "3. Export CSV\n";
        cout << "4. Exit\n";
        cout << "Choose option: ";

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string path;
            cout << "\nEnter receipt image path: ";
            getline(cin, path);

            cout << "\nExtracting text...\n";
            string text = extractTextFromImage(path);

            if (text.empty()) {
                cout << "OCR failed. Image not found or unreadable.\n";
                continue;
            }

            cout << "Extracted Text:\n" << text << "\n";

            cout << "\nSending to Groq AI for categorization...\n";
            string aiResponse = categorizeExpense(text, apiKey);
            cout << "AI Result:\n" << aiResponse << "\n";

            string vendor = detectVendor(text);
            string date = getCurrentDate();
            double amount = extractTotalAmount(text);
            string category = offlineCategory(text);

            // ---------- Parse AI JSON properly ----------
            try {
                json root = json::parse(aiResponse);

                string content = root["choices"][0]["message"]["content"];
                json data = json::parse(content);

                vendor = data.value("vendor", vendor);
                date = data.value("date", date);
                amount = data.value("totalAmount", amount);
                category = data.value("category", category);

            }
            catch (...) {
                cout << "AI parsing failed. Using offline categorization...\n";
            }

            if (amount <= 0) {
                cout << "Amount not detected. Skipping save.\n";
                continue;
            }

            saveExpense(vendor, category, amount, date);
            cout << "Expense saved successfully\n";
        }

        else if (choice == 2) {
            showSummary();
        }

        else if (choice == 3) {
            exportToCSV();
        }

        else if (choice == 4) {
            cout << "Exiting application...\n";
            break;
        }

        else {
            cout << "Invalid option. Try again.\n";
        }
    }

    return 0;
}



