# 🧾 Receipt/Expense Analyzer (C++ Version)

A C++ application that extracts text from receipt images using **Tesseract OCR** and categorizes expenses automatically using the **OpenAI GPT API**.  
It helps users analyze receipts, detect vendors, total amounts, and assign expense categories such as *Shopping*, *Food*, or *Utilities*.

---

## 🚀 Features

- 🖼️ **OCR Extraction:** Reads text from receipt images using Tesseract + Leptonica  
- 🤖 **AI Categorization:** Sends extracted text to the OpenAI API for structured analysis  
- 🧠 **JSON Output:** Generates structured JSON including vendor, date, total, and category  
- ⚙️ **Error Handling:** Detects missing, empty, or unreadable image files gracefully  
- 🔐 **Secure Key Management:** Loads OpenAI API key from a `.env` file  
- 📊 **Expense Summary:** Displays categorized totals and spending summaries  

---

## 🧰 Tech Stack

- **Language:** C++  
- **Libraries:** Tesseract OCR, Leptonica, libcurl, JSON  
- **API:** OpenAI GPT-4o-mini  
- **Tools:** Visual Studio / g++ compiler  

---

## 📁 Project Structure

```plaintext
Project2/
 ├─ x64/
 │   ├─ Debug/
 │   │   ├─ Project2.exe
 │   │   ├─ .env
 │   │   ├─ receipts/
 │   │   │   └─ receipt1.jpg
 │   │   └─ (other files)
 ├─ main.cpp
 └─ README.md
