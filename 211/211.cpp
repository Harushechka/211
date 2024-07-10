#include <iostream>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <windows.h>

#if defined(_WIN32)
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

void clearConsole() {
    system(CLEAR);
}

class GameRules {
public:
    void displayRules() const;
};

void GameRules::displayRules() const {
    std::cout << "Правила игры:\n"
        << "Выставить по порядку «квадратики», с пометкой от одного до пятнадцати.\n"
        << "Одна только трудность: пустующее окошко лишь одно.\n"
        << "Кости придется передвигать раз за разом, сдвигая цифры в нужном направлении.\n";
}

using namespace std;

void print_field(int field[][6], int size, HANDLE hConsole) {
    COORD coord = { 0, 0 };
    SetConsoleCursorPosition(hConsole, coord);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << "+----";
        }
        cout << "+" << endl;

        for (int j = 0; j < size; j++) {
            if (field[i][j] == 0)
                cout << "|    ";
            else
                cout << "| " << (field[i][j] < 10 ? " " : "") << field[i][j] << " ";
        }
        cout << "|" << endl;
    }

    for (int j = 0; j < size; j++) {
        cout << "+----";
    }
    cout << "+" << endl;
}

bool move(int field[][6], int size, int& zero_i, int& zero_j) {
    if (_kbhit()) {
        char key = _getch();
        int new_i = zero_i, new_j = zero_j;
        switch (key) {
        case 72: new_i--; break; // стрелка вверх
        case 80: new_i++; break; // стрелка вниз
        case 75: new_j--; break; // стрелка влево
        case 77: new_j++; break; // стрелка вправо
        default: return false;
        }
        if (new_i >= 0 && new_i < size && new_j >= 0 && new_j < size) {
            field[zero_i][zero_j] = field[new_i][new_j];
            field[new_i][new_j] = 0;
            zero_i = new_i;
            zero_j = new_j;
            return true;
        }
    }
    return false;
}

bool check_win(int field[][6], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (field[i][j] != (i * size + j + 1) % (size * size)) {
                return false;
            }
        }
    }
    return true;
}

void create_field(int field[][6], int size, int& zero_i, int& zero_j) {
    vector<int> tiles(size * size);
    for (int i = 0; i < size * size; i++) {
        tiles[i] = i;
    }

    random_shuffle(tiles.begin(), tiles.end());

    for (int i = 0; i < size * size; i++) {
        field[i / size][i % size] = tiles[i];
        if (tiles[i] == 0) {
            zero_i = i / size;
            zero_j = i % size;
        }
    }
}

int select_option(const vector<string>& options, const string& message) {
    int selected = 0;
    while (true) {
        system("cls");
        cout << message << endl;
        for (int i = 0; i < options.size(); i++) {
            if (i == selected) {
                cout << "> " << options[i] << " <" << endl;
            }
            else {
                cout << "  " << options[i] << "  " << endl;
            }
        }
        char key = _getch();
        switch (key) {
        case 72: selected = (selected - 1 + options.size()) % options.size(); break; // стрелка вверх
        case 80: selected = (selected + 1) % options.size(); break; // стрелка вниз
        case 13: return selected; // Enter
        }
    }
}

void startGame() {
    srand(time(0));
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false; // скрыть курсор
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    while (true) {
        vector<string> size_options = { "4x4", "5x5", "6x6" };
        int selected_size_option = select_option(size_options, "Выберете размер игрового поля:");
        int size = selected_size_option + 4;

        system("cls");
        int field[6][6];
        int zero_i, zero_j;
        create_field(field, size, zero_i, zero_j);

        while (true) {
            print_field(field, size, hConsole);
            if (check_win(field, size)) {
                vector<string> win_options = { "Да", "Нет" };
                int selected_win_option = select_option(win_options, "Вы победили! Хотите продолжить?");
                if (selected_win_option == 0) {
                    break;
                }
                else {
                    cursorInfo.bVisible = true; // вернуть курсор
                    SetConsoleCursorInfo(hConsole, &cursorInfo);
                    return;
                }
            }
            move(field, size, zero_i, zero_j);
        }
    }

    cursorInfo.bVisible = true; // вернуть курсор
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

int main() {
    setlocale(LC_ALL, "Russian");
    GameRules gameRules;
    int choice;
    while (true) {
        clearConsole();
        std::cout << "Добро пожаловать в Console Game!" << std::endl;
        std::cout << "-----------------------------" << std::endl;
        std::cout << "1. Начать игру" << std::endl;
        std::cout << "2. Информация" << std::endl;
        std::cout << "3. Рекорды" << std::endl;
        std::cout << "4. Выход из игры" << std::endl;
        std::cout << "-----------------------------" << std::endl;
        std::cout << "Сделайте выбор: ";
        std::cin >> choice;
        std::cin.ignore();
        switch (choice) {
        case 1:
            std::cout << "Игра начинается..." << std::endl;
            startGame();
            break;
        case 2:
            std::cout << "Помощь" << std::endl;
            gameRules.displayRules();
            break;
        case 3:
            std::cout << "Рекорды" << std::endl;
            // код для таблицы рекордов
            break;
        case 4:
            std::cout << "Досвидание!" << std::endl;
            return 0;
        default:
            std::cout << "Неправельный выбор. Пожалуйста введите номер от 1 до 4" << std::endl;
            break;
        }
        system("pause");
        clearConsole();
    }

    return 0;
}
