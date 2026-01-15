#include "tesseract_helper.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream>
#include <fstream>
#include "openai_client.h"

std::string extractTextFromImage(const std::string& imagePath) {
    tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
    const char* tessDataPath = "C:/Program Files/Tesseract-OCR/tessdata"; // 👈 Update if needed
    if (api->Init(tessDataPath, "eng")) {
        std::cerr << "Could not initialize tesseract using path: " << tessDataPath << "\n";
        exit(1);
    }

    Pix* image = pixRead(imagePath.c_str());
    api->SetImage(image);
    std::string outText = api->GetUTF8Text();
    api->End();
    pixDestroy(&image);
    return outText;
}

void saveTextToFile(const std::string& text, const std::string& filename) {
    std::ofstream outFile(filename);
    outFile << text;
    outFile.close();
}
