#include "CSVreader.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

// Czytanie pliku CSV i dodawanie wydatków do wektora
bool CSVReader::readCSV(const string& filename) {
    // Otwieranie pliku
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Nie mozna otworzyc pliku: " << filename << endl;
        return false;
    }

    // Czytanie linii po linii
    string line;
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string category, amountStr;

        if (!getline(ss, category, ',')) continue; // Czytanie kategorii
        if (!getline(ss, amountStr, ',')) continue; // Czytanie kwoty

        Expense exp;
        exp.category = category; // Dodawanie kategorii do wydatku
        exp.amount = stod(amountStr); // Dodawanie kwoty do wydatku - konwersja stringa na double

        expenses.push_back(exp); // Dodawanie wydatku do wektora
    }

    file.close();
    return true;
}

// Grupuje wszystkich wydatkow według kategorii
map<string, vector<Expense>> CSVReader::getExpensesByCategory() const {
    map<string, vector<Expense>> categorized;
    for (const auto& exp : expenses) {
        categorized[exp.category].push_back(exp);
    }
    return categorized;
}

// Dodaje nowy wydatek do pliku CSV
bool CSVReader::addExpense(const string& filename, const string& category, double amount) {
    // Sprawdzanie, czy plik kończy się znakiem nowej linii
    ifstream checkFile(filename);
    bool needsNewline = false;
    if (checkFile.is_open()) {
        checkFile.seekg(0, ios::end);
        streampos fileSize = checkFile.tellg();
        if (fileSize > 0) {
            checkFile.seekg(-1, ios::end);
            char lastChar;
            checkFile.get(lastChar);
            if (lastChar != '\n') {
                needsNewline = true;
            }
        }
        checkFile.close();
    }

    // Otwieranie pliku w trybie append (dopisywanie na końcu)
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cerr << "Nie mozna otworzyc pliku do zapisu: " << filename << endl;
        return false;
    }

    // Dodawanie znaku nowej linii jeśli potrzeba, a następnie nowej linii z kategorią i kwotą
    if (needsNewline) {
        file << "\n";
    }
    file << category << "," << amount << "\n";
    file.close();

    // Aktualizacja wektora wydatków w pamięci
    Expense exp;
    exp.category = category;
    exp.amount = amount;
    expenses.push_back(exp);

    return true;
}