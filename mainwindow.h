#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QTableWidget>
#include <QGroupBox>
#include <QMessageBox>
#include <QDir>
#include <QFileInfo>
#include "CSVreader.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadExpenses();
    void addExpense();
    void updateStatistics();

private:
    void setupUI();
    void refreshTable();
    void calculateStatistics();
    QString findCSVFile() const;

    CSVReader *reader;
    QTableWidget *expensesTable;
    QLineEdit *categoryInput;
    QDoubleSpinBox *amountInput;
    QPushButton *addButton;
    QPushButton *refreshButton;
    
    QLabel *totalSumLabel;
    QLabel *maxCategoryLabel;
    QTableWidget *categoryTable;
};

#endif // MAINWINDOW_H

