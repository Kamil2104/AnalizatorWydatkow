#include "mainwindow.h"
#include <QHeaderView>
#include <QString>
#include <QAbstractItemView>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    reader = new CSVReader();
    setupUI();
    loadExpenses();
}

MainWindow::~MainWindow()
{
    delete reader;
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Sekcja dodawania wydatków
    QGroupBox *addExpenseGroup = new QGroupBox("Dodaj nowy wydatek", this);
    QHBoxLayout *addLayout = new QHBoxLayout(addExpenseGroup);

    addLayout->addWidget(new QLabel("Kategoria:", this));
    categoryInput = new QLineEdit(this);
    addLayout->addWidget(categoryInput);

    addLayout->addWidget(new QLabel("Kwota:", this));
    amountInput = new QDoubleSpinBox(this);
    amountInput->setMinimum(0.0);
    amountInput->setMaximum(999999.99);
    amountInput->setDecimals(2);
    addLayout->addWidget(amountInput);

    addButton = new QPushButton("Dodaj wydatek", this);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addExpense);
    addLayout->addWidget(addButton);

    refreshButton = new QPushButton("Odśwież", this);
    connect(refreshButton, &QPushButton::clicked, this, &MainWindow::loadExpenses);
    addLayout->addWidget(refreshButton);

    mainLayout->addWidget(addExpenseGroup);

    // Tabela wydatków
    QGroupBox *expensesGroup = new QGroupBox("Wszystkie wydatki", this);
    QVBoxLayout *expensesLayout = new QVBoxLayout(expensesGroup);

    expensesTable = new QTableWidget(this);
    expensesTable->setColumnCount(2);
    expensesTable->setHorizontalHeaderLabels(QStringList() << "Kategoria" << "Kwota");
    expensesTable->horizontalHeader()->setStretchLastSection(true);
    expensesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    expensesLayout->addWidget(expensesTable);

    mainLayout->addWidget(expensesGroup);

    // Statystyki
    QGroupBox *statsGroup = new QGroupBox("Statystyki", this);
    QVBoxLayout *statsLayout = new QVBoxLayout(statsGroup);

    totalSumLabel = new QLabel("Łączna suma wydatków: 0.00", this);
    statsLayout->addWidget(totalSumLabel);
    
    maxCategoryLabel = new QLabel("Kategoria z największymi wydatkami: -", this);
    statsLayout->addWidget(maxCategoryLabel);
    
    // Tabela sum per kategoria
    categoryTable = new QTableWidget(this);
    categoryTable->setColumnCount(2);
    categoryTable->setHorizontalHeaderLabels(QStringList() << "Kategoria" << "Suma");
    categoryTable->horizontalHeader()->setStretchLastSection(true);
    statsLayout->addWidget(categoryTable);
    
    mainLayout->addWidget(statsGroup);
    
    setWindowTitle("Menedżer Wydatków");
    resize(800, 600);
}

QString MainWindow::findCSVFile() const
{
    // Lista miejsc, gdzie szukać pliku
    QStringList searchPaths;

    // 1. Katalog roboczy (gdzie uruchomiono aplikację)
    searchPaths << QDir::current().absoluteFilePath("expenses.csv");

    // 2. Katalog aplikacji (dla .app bundle na macOS)
    QString appDir = QCoreApplication::applicationDirPath();
    searchPaths << QDir(appDir).absoluteFilePath("expenses.csv");

    // 3. Katalog obok aplikacji (dla macOS .app bundle)
    #ifdef Q_OS_MACOS
    QDir dir(appDir);
    if (dir.dirName() == "MacOS") {
        dir.cdUp(); // Contents
        dir.cdUp(); // ExpenseManager.app
        searchPaths << dir.absoluteFilePath("expenses.csv");
    }
    #endif

    // 4. Katalog domowy użytkownika
    searchPaths << QDir::home().absoluteFilePath("expenses.csv");

    // Sprawdź każdą ścieżkę
    for (const QString &path : searchPaths) {
        if (QFileInfo::exists(path)) {
            return path;
        }
    }

    // Jeśli nie znaleziono, zwróć względną ścieżkę
    return "expenses.csv";
}

void MainWindow::loadExpenses()
{
    QString csvPath = findCSVFile();
    if (!reader->readCSV(csvPath.toStdString())) {
        QMessageBox::warning(this, "Błąd",
            QString("Nie można otworzyć pliku expenses.csv\nSzukano w:\n%1").arg(csvPath));
        return;
    }

    refreshTable();
    calculateStatistics();
}

void MainWindow::refreshTable()
{
    auto categorized = reader->getExpensesByCategory();

    int rowCount = 0;
    for (const auto& pair : categorized) {
        rowCount += pair.second.size();
    }

    expensesTable->setRowCount(rowCount);

    int row = 0;
    for (const auto& pair : categorized) {
        for (const auto& exp : pair.second) {
            expensesTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(exp.category)));
            expensesTable->setItem(row, 1, new QTableWidgetItem(QString::number(exp.amount, 'f', 2)));
            row++;
        }
    }
}

void MainWindow::calculateStatistics()
{
    auto categorized = reader->getExpensesByCategory();

    std::map<std::string, double> categorySums;
    double totalSum = 0.0;

    for (const auto& pair : categorized) {
        double categorySum = 0.0;
        for (const auto& exp : pair.second) {
            categorySum += exp.amount;
            totalSum += exp.amount;
        }
        categorySums[pair.first] = categorySum;
    }

    // Aktualizacja łącznej sumy
    totalSumLabel->setText(QString("Łączna suma wydatków: %1").arg(totalSum, 0, 'f', 2));

    // Znajdowanie kategorii z największymi wydatkami
    std::string maxCategory;
    double maxSum = 0.0;
    for (const auto& pair : categorySums) {
        if (pair.second > maxSum) {
            maxSum = pair.second;
            maxCategory = pair.first;
        }
    }

    if (!maxCategory.empty()) {
        maxCategoryLabel->setText(QString("Kategoria z największymi wydatkami: %1 (%2)")
                                  .arg(QString::fromStdString(maxCategory))
                                  .arg(maxSum, 0, 'f', 2));
    }

    // Aktualizacja tabeli kategorii
    categoryTable->setRowCount(categorySums.size());
    int row = 0;
    for (const auto& pair : categorySums) {
        categoryTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(pair.first)));
        categoryTable->setItem(row, 1, new QTableWidgetItem(QString::number(pair.second, 'f', 2)));
        row++;
    }
}

void MainWindow::addExpense()
{
    QString category = categoryInput->text().trimmed();
    double amount = amountInput->value();

    if (category.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Proszę podać kategorię!");
        return;
    }

    if (amount <= 0) {
        QMessageBox::warning(this, "Błąd", "Kwota musi być większa od zera!");
        return;
    }

    QString csvPath = findCSVFile();
    if (reader->addExpense(csvPath.toStdString(), category.toStdString(), amount)) {
        QMessageBox::information(this, "Sukces", "Wydatek został dodany pomyślnie!");
        categoryInput->clear();
        amountInput->setValue(0.0);
        loadExpenses(); // Odświeżenie danych
    } else {
        QMessageBox::warning(this, "Błąd", "Nie udało się dodać wydatku!");
    }
}

void MainWindow::updateStatistics()
{
    calculateStatistics();
}

