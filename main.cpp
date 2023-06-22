#include <iostream>
#include <algorithm> // any_of
#include <fstream>
#include <windows.h>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

bool isIncludes(const char &item, const std::string &range) {
    return std::any_of(range.begin(),
                       range.end(),
                       [&item](const char &c) { return c == item; });
}

std::string getJoinRange(const std::string &range) {
    std::string joinRange;
    for (int i = 0; i < range.size(); ++i) {
        joinRange += range[i];
        joinRange += (i != range.size() - 1) ? ',' : '.';
    }

    return joinRange;
}

template<typename T> T getUserChar(std::string const &range) {
    T input;
    bool proceed = false;
    const char* warning = "Error. Symbol %c not in range: %s Repeat: ";

    while (!proceed) {
        std::cin >> input;

        proceed = isIncludes(input, range);
        if (!proceed) printf(warning, input, getJoinRange(range).c_str());

        // Сбрасываем коматозное состояние cin и очищаем потом ввода
        std::cin.clear();
        fflush(stdin);
    }

    return input;
}

int getTypeOfPath() {
    cout << "--------------- INFO --------------\n";
    cout << "Выберите месторасположение файла (должен быть текстовым):\n";
    cout << "0 - если в той же директории,\n";
    cout << "1 - для другой директорий. Например J:\\C\\19_5_2\\test1\n";
    cout << "Итак, выберите 0 или 1: ";
    return (getUserChar<char>("01") - '0');
}

bool isFileExist(const char* fileName) {
    bool isExist = false;
    std::ifstream fileReader;
    fileReader.open(fileName);

    if (fileReader.is_open() && !fileReader.bad()) {
        isExist = true;
    }

    fileReader.close();

    return isExist;
}

// Альтернатива функции readFileAsText
void readFile(const char* fileName) {
    long long bufferSize = 20;
    char* buffer = new char[bufferSize + 1];
    buffer[bufferSize] = 0;

    cout << "Файл найден. Открываю...\n------------------------" << endl;

    std::ifstream fileReader;
    fileReader.open(fileName, std::ios::binary);
    while (!fileReader.eof()) {
        fileReader.read(buffer, bufferSize);
        cout << buffer;
    }

    fileReader.close();
}

void readFileAsText(const char* fileName) {
    std::string textLine;

    cout << "Файл найден. Открываю...\n------------------------" << endl;

    std::ifstream fileReader;
    fileReader.open(fileName);
    while (std::getline(fileReader, textLine)) {
        cout << textLine << endl;
    }

    fileReader.close();
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    bool continueWithSuccess = false;
    vector<string> ROUTES_INFO = { "название файла", "полный путь и название файла" };
    vector<string> ROUTES = { R"(..\%s.txt)", R"(%s.txt)" };

    while (!continueWithSuccess) {
        char userInput[128];
        char pathToFile[128];
        int typeOfPath = getTypeOfPath();

        printf("Введите %s (без расширения): ", ROUTES_INFO[typeOfPath].c_str());
        string pathCurrent = ROUTES[typeOfPath];

        cin.getline(userInput, sizeof(userInput), '\n');
        sprintf(pathToFile, pathCurrent.c_str(), userInput);
        cout << "Полный путь до файла: " << pathToFile << endl;

        if (isFileExist(pathToFile)) {
            // Можем открыть и как бинарный файл:
            // readFile(pathToFile);
            // Можем открыть и считать построчно:
            readFileAsText(pathToFile);
            continueWithSuccess = true;
        } else {
            std::cout << "Неверно. Указанного файла не обнаружено" << std::endl;
            std::cout << "Вы хотите повторить (y/n)?: ";
            if (isIncludes(getUserChar<char>("YyNn"), "Nn")) {
                continueWithSuccess = true;
            }
            system("cls");
        }
    }

    cout << "\n------------------------" << endl;
    std::cout << "Чтение выполнено успешно" << std::endl;
}
