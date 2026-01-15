#include <curl/curl.h>
#include <iostream>
#include <string>

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

static std::string escapeJson(const std::string& input) {
    std::string out;
    for (char c : input) {
        switch (c) {
        case '\"': out += "\\\""; break;
        case '\\': out += "\\\\"; break;
        case '\n': out += "\\n"; break;
        case '\t': out += "\\t"; break;
        case '\r': break;
        default: out += c;
        }
    }
    return out;
}

std::string categorizeExpense(const std::string& receiptText, const std::string& apiKey) {
    CURL* curl = curl_easy_init();
    std::string response;

    if (!curl) {
        std::cerr << "[ERROR] CURL initialization failed\n";
        return "";
    }

    std::string escapedText = escapeJson(receiptText);

    // STRICT JSON ONLY prompt
    std::string prompt =
        "Return ONLY valid JSON. No markdown, no explanation.\n"
        "Format exactly:\n"
        "{\"vendor\":\"\",\"date\":\"\",\"totalAmount\":0,\"category\":\"\"}\n\n"
        "Receipt text:\n" + escapedText;

    std::string postData =
        "{"
        "\"model\":\"llama-3.1-8b-instant\","
        "\"messages\":[{"
        "\"role\":\"user\","
        "\"content\":\"" + escapeJson(prompt) + "\""
        "}]"
        "}";

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.groq.com/openai/v1/chat/completions");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "[CURL ERROR] " << curl_easy_strerror(res) << "\n";
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    return response;
}



