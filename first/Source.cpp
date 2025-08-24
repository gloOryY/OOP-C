#define NOMINMAX
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <sstream>
#include <windows.h>

using namespace std;

// Структура для товара
struct Product {
    string name;    // Название товара (используем std::string вместо char*)
    double price;   // Цена товара
    int quant;      // Количество товара
};

// Функция для добавления нового товара в базу данных
void addProduct(vector<Product>& products) {
    Product product;

    cout << "Введите название товара: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, product.name);

    // Проверка на пустое название
    if (product.name.empty()) {
        cout << "Ошибка: название товара не может быть пустым!" << endl;
        return;
    }

    cout << "Введите цену товара: ";
    while (!(cin >> product.price) || product.price < 0) {
        cout << "Ошибка: введите корректную цену (положительное число): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Введите количество товара: ";
    while (!(cin >> product.quant) || product.quant < 0) {
        cout << "Ошибка: введите корректное количество (целое неотрицательное число): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    products.push_back(product);
    cout << "Товар успешно добавлен!" << endl;
}

// Функция для распечатки базы данных товаров в табличном виде
void printProducts(const vector<Product>& products) {
    if (products.empty()) {
        cout << "База данных пуста." << endl;
        return;
    }

    cout << "---------------------------------------------------------------------------------------------------------" << endl;
    cout << setw(30) << left << "Название товара" << setw(10) << left << "Цена"
        << setw(15) << left << "Количество" << setw(15) << left << "Общая сумма" << endl;
    cout << "---------------------------------------------------------------------------------------------------------" << endl;

    double totalSum = 0;
    for (const auto& product : products) {
        double sum = product.price * product.quant;
        totalSum += sum;
        cout << setw(30) << left << product.name
            << setw(10) << left << product.price
            << setw(15) << left << product.quant
            << setw(15) << left << sum << endl;
    }

    cout << "---------------------------------------------------------------------------------------------------------" << endl;
    cout << "Всего товаров на сумму: " << totalSum << endl;
    cout << "Количество записей в базе: " << products.size() << endl;
    cout << "База данных: Склад (товары)." << endl;
}

// Функция для поиска товара по названию
void searchByName(const vector<Product>& products, const string& name) {
    cout << "Результаты поиска по названию \"" << name << "\":" << endl;
    cout << "---------------------------------------------------------------------------------------------------------" << endl;
    cout << setw(30) << left << "Название товара" << setw(10) << left << "Цена"
        << setw(15) << left << "Количество" << setw(15) << left << "Общая сумма" << endl;
    cout << "---------------------------------------------------------------------------------------------------------" << endl;

    double totalSum = 0;
    int foundCount = 0;
    bool found = false;

    for (const auto& product : products) {
        if (product.name.find(name) != string::npos) { // Поиск подстроки в названии
            double sum = product.price * product.quant;
            totalSum += sum;
            foundCount++;
            cout << setw(30) << left << product.name
                << setw(10) << left << product.price
                << setw(15) << left << product.quant
                << setw(15) << left << sum << endl;
            found = true;
        }
    }

    cout << "---------------------------------------------------------------------------------------------------------" << endl;
    if (!found) {
        cout << "Товары с таким названием не найдены." << endl;
    }
    else {
        cout << "Найдено товаров: " << foundCount << endl;
        cout << "Общая сумма найденных товаров: " << totalSum << endl;
    }
}

// Функция для поиска всех товаров с количеством меньше заданного
void findProductsLessThan(const vector<Product>& products, int quantity) {
    cout << "Товары с количеством меньше " << quantity << ":" << endl;
    cout << "---------------------------------------------------------------------------------------------------------" << endl;
    cout << setw(30) << left << "Название товара" << setw(10) << left << "Цена"
        << setw(15) << left << "Количество" << setw(15) << left << "Общая сумма" << endl;
    cout << "---------------------------------------------------------------------------------------------------------" << endl;

    double totalSum = 0;
    int foundCount = 0;
    bool found = false;

    for (const auto& product : products) {
        if (product.quant < quantity) {
            double sum = product.price * product.quant;
            totalSum += sum;
            foundCount++;
            cout << setw(30) << left << product.name
                << setw(10) << left << product.price
                << setw(15) << left << product.quant
                << setw(15) << left << sum << endl;
            found = true;
        }
    }

    cout << "---------------------------------------------------------------------------------------------------------" << endl;
    if (!found) {
        cout << "Товары с количеством меньше " << quantity << " не найдены." << endl;
    }
    else {
        cout << "Найдено товаров: " << foundCount << endl;
        cout << "Общая сумма найденных товаров: " << totalSum << endl;
    }
}

// Функция для сохранения базы данных на диске
bool saveDatabase(const vector<Product>& products, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка: невозможно открыть файл для записи: " << filename << endl;
        return false;
    }

    for (const auto& product : products) {
        file << product.name << " " << product.price << " " << product.quant << endl;
    }

    file.close();
    if (file.fail()) {
        cout << "Ошибка при сохранении файла." << endl;
        return false;
    }

    cout << "База данных успешно сохранена в файл: " << filename << endl;
    return true;
}

// Функция для загрузки базы данных из файла
bool loadDatabase(vector<Product>& products, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Файл с базой данных не найден. Будет создана новая пустая база." << endl;
        return false;
    }

    products.clear(); // Очищаем текущие данные

    string line;
    int lineNumber = 0;
    while (getline(file, line)) {
        lineNumber++;
        if (line.empty()) continue;

        istringstream iss(line);
        Product product;

        // Читаем название (может содержать пробелы)
        string namePart;
        string fullName;
        iss >> fullName; // Первое слово названия

        // Читаем остальные части названия до числа
        while (iss >> namePart) {
            // Проверяем, не является ли часть числом (ценой)
            try {
                stod(namePart); // Если преобразуется в число - это цена
                break;
            }
            catch (...) {
                fullName += " " + namePart; // Добавляем к названию
            }
        }

        product.name = fullName;

        // Пытаемся прочитать цену и количество
        if (!(iss >> product.price >> product.quant)) {
            cout << "Ошибка формата в строке " << lineNumber << ": " << line << endl;
            continue;
        }

        products.push_back(product);
    }

    file.close();
    cout << "Загружено записей: " << products.size() << endl;
    return true;
}

int main() {
    // Установка кодировки консоли для корректного отображения кириллицы в Windows
    SetConsoleCP(CP_UTF8);        // Установка кодировки ввода в UTF-8
    SetConsoleOutputCP(CP_UTF8);  // Установка кодировки вывода в UTF-8
    
    vector<Product> products;
    const string filename = "products_database.txt";

    cout << "=== Система управления складом товаров ===" << endl;

    // Загрузка базы данных при запуске
    loadDatabase(products, filename);

    while (true) {
        cout << "\n=== Меню ===" << endl;
        cout << "1. Добавить товар" << endl;
        cout << "2. Распечатать базу товаров" << endl;
        cout << "3. Поиск товара по названию" << endl;
        cout << "4. Найти все товары с количеством меньше заданного" << endl;
        cout << "5. Сохранить и выйти" << endl;
        cout << "Выберите команду (1-5): ";

        int choice;
        if (!(cin >> choice)) {
            cout << "Ошибка: введите число от 1 до 5!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            addProduct(products);
            break;
        case 2:
            printProducts(products);
            break;
        case 3: {
            cout << "Введите название товара для поиска: ";
            string searchName;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, searchName);
            searchByName(products, searchName);
            break;
        }
        case 4: {
            cout << "Введите количество для поиска: ";
            int quantity;
            while (!(cin >> quantity) || quantity < 0) {
                cout << "Ошибка: введите корректное количество (целое неотрицательное число): ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            findProductsLessThan(products, quantity);
            break;
        }
        case 5:
            if (saveDatabase(products, filename)) {
                cout << "Данные сохранены. Выход из программы." << endl;
            }
            else {
                cout << "Предупреждение: данные не были сохранены!" << endl;
            }
            return 0;
        default:
            cout << "Неверный выбор. Введите число от 1 до 5." << endl;
        }
    }
}