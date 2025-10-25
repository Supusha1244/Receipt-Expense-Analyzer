#include <curl/curl.h>
#include <iostream>
#include <string>

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string categorizeExpense(const std::string& receiptText, const std::string& apiKey) {
    CURL* curl = curl_easy_init();
    std::string response;

    if (!curl) {
        std::cerr << "[ERROR] Failed to initialize CURL\n";
        return "";
    }

    std::string prompt = "Extract structured data from this receipt text:\n" + receiptText +
        "\nReturn JSON like: {\"vendor\": \"\", \"date\": \"\", \"total_amount\": \"\", \"category\": \"\", \"suggestion\": \"\"}";

    std::string postData = "{"
        "\"model\": \"gpt-4o-mini\","
        "\"messages\": [{\"role\": \"user\", \"content\": \"" + prompt + "\"}]"
        "}";

    std::string authHeader = "Authorization: Bearer " + apiKey;
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, authHeader.c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
        std::cerr << "[CURL ERROR] " << curl_easy_strerror(res) << "\n";

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    return response;
}
