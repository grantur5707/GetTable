/**
 * @file main.cpp
 * @brief Программа распознавания таблиц с помощью Tesseract и OpenCV.
 */

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <regex>

using namespace std;

/**
 * @struct TableInfo
 * @brief Структура для хранения информации о таблице.
 *
 * @var TableInfo::number
 * Номер таблицы в виде строки.
 * @var TableInfo::title
 * Название таблицы.
 */
struct TableInfo {
    string number;
    string title;
};

/**
 * @brief Извлекает информацию о таблицах из текста.
 * @param text Текст для анализа.
 * @return Вектор структур TableInfo с информацией о таблицах.
 */
vector<TableInfo> extractTableInfo(const string& text) {
    vector<TableInfo> tables;
    regex tablePattern(R"(Таблица\s+([\d.]+)\s+.*?\s+(.+))");
    auto matches_begin = sregex_iterator(text.begin(), text.end(), tablePattern);
    auto matches_end = sregex_iterator();

    for (sregex_iterator i = matches_begin; i != matches_end; ++i) {
        smatch match = *i;
        TableInfo table;
        table.number = match.str(1);
        table.title = match.str(2);
        tables.push_back(table);
    }

    return tables;
}

/**
 * @brief Находит таблицы, расположенные не по порядку.
 * @param tables Вектор структур TableInfo для анализа.
 * @return Вектор строк с номерами таблиц, расположенных не по порядку.
 */
vector<string> findMisorderedTables(const vector<TableInfo>& tables) {
    vector<string> misordered;
    string prevNumber = "0";  // Инициализация с нулевым номером для сравнения с первой таблицей

    for (const auto& table : tables) {
        if (table.number <= prevNumber) {
            misordered.push_back(prevNumber);
        }
        prevNumber = table.number;
    }

    return misordered;
}

/**
 * @brief Точка входа в программу.
 * Использует Tesseract и OpenCV для распознавания и анализа таблиц в изображении.
 * @return Код завершения программы.
 */
int main() {
    locale::global(locale("en_US.UTF-8"));

    cv::Mat img = cv::imread("4.png");

    if (img.empty()) {
        cerr << "Ошибка: изображение не загружено." << endl;
        return -1;
    }

    tesseract::TessBaseAPI* ocr = new tesseract::TessBaseAPI();

    const char* tessdata_path = "E:/vcpkg/installed/x64-windows/share/tessdata/";
    _putenv_s("TESSDATA_PREFIX", tessdata_path);

    if (ocr->Init(tessdata_path, "eng+rus", tesseract::OEM_LSTM_ONLY)) {
        cerr << "Не удалось инициализировать tesseract." << endl;
        return 1;
    }

    ocr->SetImage(img.data, img.cols, img.rows, img.channels(), img.step);

    char* outText = ocr->GetUTF8Text();
    string text(outText);

    replace(text.begin(), text.end(), '|', '1');

    vector<TableInfo> tables = extractTableInfo(text);
    vector<string> misorderedTables = findMisorderedTables(tables);

    for (const auto& table : tables) {
        cout << "Номер таблицы: " << table.number << endl;
        cout << "Название таблицы: " << table.title << endl;
        cout << "----" << endl;
    }

    if (!misorderedTables.empty()) {
        cout << "\nНеправильно пронумерованы таблицы: ";
        for (const string& number : misorderedTables) {
            cout << number << " ";
        }
        cout << endl;
    }
    
    ocr->End();

    cout << "Нажмите Enter, чтобы выйти...";
    cin.get();  // Добавлено ожидание ввода

    return 0;
}