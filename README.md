# Analizator Wydatków - Aplikacja GUI

Aplikacja do zarządzania wydatkami z interfejsem graficznym stworzonym w Qt.

## Wymagania

- Qt 5 lub Qt 6
- Kompilator C++11 lub nowszy

## Kompilacja

### Linux/macOS:

```bash
qmake ExpenseManager.pro
make
```

### Windows (MinGW):

```bash
qmake ExpenseManager.pro
mingw32-make
```

### Windows (MSVC):

```bash
qmake ExpenseManager.pro
nmake
```

## Uruchomienie

### macOS (po kompilacji):

Najprostszy sposób - kliknij dwukrotnie na `ExpenseManager.app` w Finderze lub użyj:

```bash
open ExpenseManager.app
```

Alternatywnie z terminala:

```bash
./ExpenseManager.app/Contents/MacOS/ExpenseManager
```

### Linux:

```bash
./ExpenseManager
```

### Windows:

```bash
ExpenseManager.exe
```

## Szybki start

**Jeśli aplikacja jest już skompilowana**, po prostu uruchom:
```bash
open ExpenseManager.app    # macOS
```

**Jeśli zmieniłeś kod**, najpierw skompiluj ponownie:
```bash
qmake ExpenseManager.pro
make
```

Następnie uruchom aplikację jak wyżej.

## Funkcjonalności

- **Wyświetlanie wydatków** - wszystkie wydatki z pliku CSV w tabeli
- **Dodawanie wydatków** - możliwość dodania nowego wydatku przez GUI
- **Sumowanie wydatków** - łączna suma wszystkich wydatków
- **Sumowanie per kategoria** - suma wydatków dla każdej kategorii
- **Największe wydatki** - wyświetlanie kategorii z największymi wydatkami
- **Odświeżanie danych** - przycisk do odświeżenia danych z pliku CSV

## Format pliku CSV

Plik `expenses.csv` powinien mieć format:
```
Category,Amount
Food,25.50
Transport,12.00
...
```

