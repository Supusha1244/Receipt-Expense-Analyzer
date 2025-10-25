#ifndef TESSERACT_HELPER_H
#define TESSERACT_HELPER_H

#include <string>

std::string extractTextFromImage(const std::string& imagePath);
void saveTextToFile(const std::string& text, const std::string& filename);

#endif

