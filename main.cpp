#include <iostream>
#include <fstream>
#include "tesseract_helper.h"
#include "openai_client.h"
#include "db_helper.h"
#include <string>

std::string getAPIKey() {
    std::ifstream file(".env");
    std::string key;
    getline(file, key);
    return key;
}

int main() {
    std::cout << " Receipt/Expense Analyzer (C++ Version)\n";
    std::string path;
    std::cout << "Enter receipt image path: ";
    std::cin >> path;

    std::string apiKey = getAPIKey();
    if (apiKey.empty()) {
        std::cerr << " Missing API key in .env file\n";
        return 1;
    }

    std::cout << "\n Extracting text...\n";
    std::string text = extractTextFromImage(path);
    std::cout << " Extracted Text:\n" << text << "\n";

    std::cout << "\n Sending to OpenAI for categorization...\n";
    std::string result = categorizeExpense(text, apiKey);
    std::cout << " GPT Result:\n" << result << "\n";

    // Example: Simulated save (in real case, parse JSON)
    saveExpense("Amazon", "Shopping", 1250.75);

    std::cout << "\n Showing summary...\n";
    showSummary();
}
