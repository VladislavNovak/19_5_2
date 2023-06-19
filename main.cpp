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


template<typename T> T getUserChar(std::string const &range) {
    T input;
    bool proceed = false;
    const char* warning = "Error. Symbol %c not in range: %s Repeat: ";

    while (!proceed) {
        std::cin >> input;

        proceed = isIncludes(input, range);
        if (!proceed) printf(warning, input, range.c_str());

        // Сбрасываем коматозное состояние cin и очищаем потом ввода
        std::cin.clear();
        fflush(stdin);
    }

    return input;
}

int getTypeOfPath() {
    cout << "--------------- INFO --------------\n";
    cout << "Выберите месторасположение файла:\n";
    cout << "0 - если он текстовый и в той же директории,\n";
    cout << "1 - для любого другого файла или другой директорий. Например J:\\C\\19_5_2\\test1.txt\n";
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

void readFile(char* pathToFile) {
    long long bufferSize = 20;
    char* buffer = new char[bufferSize + 1];
    buffer[bufferSize] = 0;

    cout << "Файл найден. Открываю..." << endl;
    cout << "------------------------" << endl;

    std::ifstream fileReader;
    fileReader.open(pathToFile, std::ios::binary);
    while (!fileReader.eof()) {
        fileReader.read(buffer, bufferSize);
        cout << buffer;
    }
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    bool continueNext = false;
    vector<string> ROUTES = { R"(..\%s.txt)", R"(%s)" };

    while (!continueNext) {
        char userInput[128];
        char pathToFile[128];
        int typeOfPath = getTypeOfPath();

        const char* info = typeOfPath
                           ? "Введите полный путь, название файла и его расширение: "
                           : "Введите название файла (без расширения): ";
        cout << info;
        string pathCurrent = ROUTES[typeOfPath];

        cin.getline(userInput, sizeof(userInput), '\n');
        sprintf(pathToFile, pathCurrent.c_str(), userInput);
        std::cout << "Полный путь до файла: " << pathToFile << std::endl;

        if (isFileExist(pathToFile)) {
            readFile(pathToFile);
            continueNext = true;
        } else {
            std::cout << "Неверно. Указанного файла не обнаружено" << std::endl;
            std::cout << "Вы хотите повторить (y/n)?: ";
            if (isIncludes(getUserChar<char>("YyNn"), "Nn")) {
                continueNext = true;
            }
        }
    }

    cout << "\n------------------------" << endl;
    std::cout << "Чтение выполнено успешно" << std::endl;
}
