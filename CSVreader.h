#ifndef CSVREADER_H
#define CSVREADER_H

#include <string>
#include <vector>
#include <map>

using namespace std;

struct Expense {
    string category;
    double amount;
};

class CSVReader {
    public:
        bool readCSV(const string& filename); // Czy powiodło się odczytanie pliku
        map<string, vector<Expense>> getExpensesByCategory() const; // Zwraca mapę z kategoriami i wydatkami
        bool addExpense(const string& filename, const string& category, double amount); // Dodaje nowy wydatek do pliku CSV

    private:
        vector<Expense> expenses; // Wektor wydatków
    };

#endif