#include <iostream>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <fstream>

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
        << "Кости придется передвигать раз за разом, сдвигая цифры в нужном направлении.\n"
        << "Для передвижения пустой плитки используйте кнопки стрелок.\n"
        << "Кнопка 'S' - сохранение игры и возвращение в главное меню.\n"
        << "Кнопка 'Q' - возвращение в главное меню.\n";
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
        if (key == 'q' || key == 'Q') {
            return true; 
        }
        else if (key == 's' || key == 'S') {
            
            ofstream saveFile("savegame.txt");
            saveFile << size << endl;
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    saveFile << field[i][j] << " ";
                }
                saveFile << endl;
            }
            saveFile << zero_i << " " << zero_j << endl;
            saveFile.close();
            cout << "Игра сохранена! Возвращаемся в главное меню..." << endl;
            system("pause");
            return true; 
        }
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
            return false; 
        }
    }
    return false;
}

bool check_win(int field[][6], int size) { // проверка победы
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

bool loadGame(int field[][6], int& size, int& zero_i, int& zero_j) {
    ifstream loadFile("savegame.txt");
    if (!loadFile.is_open()) {
        return false; 
    }

    loadFile >> size;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            loadFile >> field[i][j];
            if (field[i][j] == 0) {
                zero_i = i;
                zero_j = j;
            }
        }
    }

    loadFile.close();
    return true; 
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

void startGame(bool load_saved_game = false) {
    srand(time(0));
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false; // скрыть курсор
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    while (true) {
        int size;
        int field[6][6];
        int zero_i, zero_j;

        if (load_saved_game) {
            if (!loadGame(field, size, zero_i, zero_j)) {
                cout << "Нет сохраненной игры!" << endl;
                system("pause");
                load_saved_game = false;
                continue;
            }
        }
        else {
            vector<string> size_options = { "4x4", "5x5", "6x6" };
            int selected_size_option = select_option(size_options, "Выберите размер игрового поля:");
            size = selected_size_option + 4;
            create_field(field, size, zero_i, zero_j);
        }

        system("cls");

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
            if (move(field, size, zero_i, zero_j)) {
                cursorInfo.bVisible = true; // вернуть курсор
                SetConsoleCursorInfo(hConsole, &cursorInfo);
                return; 
            }
        }
    }

    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

int main() {
    setlocale(LC_ALL, "Russian");
    GameRules gameRules;
    vector<string> main_menu_options = { "Начать игру", "Загрузить игру", "Информация", "Рекорды", "Выход из игры" };
    int choice;

    while (true) {
        choice = select_option(main_menu_options, "Добро пожаловать в Console Game!");
        switch (choice) {
        case 0:
            std::cout << "Игра начинается..." << std::endl;
            startGame();
            break;
        case 1:
            std::cout << "Загрузка игры..." << std::endl;
            startGame(true);
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
            std::cout << "До свидания!" << std::endl;
            return 0;
        default:
            std::cout << "Неправильный выбор. Пожалуйста, введите номер от 1 до 5." << std::endl;
            break;
        }
        system("pause");
        clearConsole();
    }

    return 0;
}
