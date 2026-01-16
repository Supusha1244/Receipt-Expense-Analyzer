# Receipt-Expense-Analyzer
# 🧾 Receipt / Expense Analyzer (C++ + OCR + Groq AI + SQLite)

A C++ based Receipt & Expense Analyzer that extracts text from receipt images using **Tesseract OCR**, categorizes expenses using **Groq AI (LLaMA 3.1)**, and stores data in **SQLite** with summary + CSV export features.

---

## 🚀 Features

- 📷 **Receipt Image OCR** using Tesseract
- 🤖 **AI-based categorization** using Groq (LLaMA 3.1)
- 🧠 **Offline fallback categorization** if AI fails
- 🗄️ **SQLite database storage**
- 📊 **Category-wise expense summary**
- 📁 **CSV Export** for reports

---

## 🛠️ Tech Stack

- **Language:** C++
- **OCR:** Tesseract + Leptonica
- **AI:** Groq API (OpenAI-compatible endpoint)
- **Database:** SQLite3
- **HTTP Client:** libcurl
- **Build:** Visual Studio (x64)

---
▶️ How to Run

Run the program and choose option 1 to add a receipt.

Example Input (Image Path)

Since the program runs from x64/Debug, use:

../../images/receipt1.jpg

🧪 Sample Output
====== Expense Analyzer ======
1. Add new receipt
2. View summary
3. Export CSV
4. Exit

Enter receipt image path: ../../images/receipt1.jpg

Extracting text...
Extracted Text:
SuperMart
Milk 45
Bread 30
Total 320

Sending to Groq AI for categorization...
Saved: SuperMart | Grocery | Rs 320 | 2026-01-15
Expense saved successfully
## 📸 Screenshots

![Screenshot 194](Screenshot%20(194).png)
