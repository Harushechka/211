#include <iostream>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <fstream>
#include <sstream>

#if defined(_WIN32)
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

using namespace std;

void clearConsole() { // очищает консоль
    system(CLEAR);
}

class GameRules {
public:                         // класс для отображения правил игры
    void displayRules() const;
};

void GameRules::displayRules() const { // реализация метода для отображения правил игры
    cout << "Правила игры:\n"
        << "Выставить по порядку «квадратики», с пометкой от одного до пятнадцати.\n"
        << "Одна только трудность: пустующее окошко лишь одно.\n"
        << "Кости придется передвигать раз за разом, сдвигая цифры в нужном направлении.\n"
        << "Для передвижения пустой плитки используйте кнопки стрелок.\n"
        << "Кнопка 'S' - сохранение игры и возвращение в главное меню.\n"
        << "Кнопка 'Q' - возвращение в главное меню.\n";
}

// функция для отображения игрового поля
void print_field(int field[][6], int size, HANDLE hConsole) {
    // устанавливает курсор в начало консоли
    COORD coord = { 0, 0 };
    SetConsoleCursorPosition(hConsole, coord);

    // отображает строки игрового поля
    for (int i = 0; i < size; i++) {  // size - размер игрового поля
        for (int j = 0; j < size; j++) {
            cout << "+----";
        }
        cout << "+" << endl;

        // отображает значение плиток
        for (int j = 0; j < size; j++) {
            if (field[i][j] == 0)
                cout << "|    "; // пустая плитка
            else
                cout << "| " << (field[i][j] < 10 ? " " : "") << field[i][j] << " ";
        }
        cout << "|" << endl;
    }

    // заключительная строка игрового поля
    for (int j = 0; j < size; j++) {
        cout << "+----";
    }
    cout << "+" << endl;
}

// функция перемещения пустой плитки
// возращает true, если нажата кнопка выхода или сохранения
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

        // перемещение плиток
        int new_i = zero_i, new_j = zero_j;  //zero_i,zero_j - координаты пустой плитки
        switch (key) {
        case 72: new_i--; break; // стрелка вверх
        case 80: new_i++; break; // стрелка вниз
        case 75: new_j--; break; // стрелка влево
        case 77: new_j++; break; // стрелка вправо
        default: return false; // ничего не делать, если нажата другая кнопка
        }
        // обновление положения плиток
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

// функция проверки победы
// возвращает true, если плиты стоят по порядку
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

// функция для создания игрового поля и размещения плиток
void create_field(int field[][6], int size, int& zero_i, int& zero_j) {
    vector<int> tiles(size * size);
    for (int i = 0; i < size * size; i++) {
        tiles[i] = i;
    }

    random_shuffle(tiles.begin(), tiles.end()); // перемешивание плиток

    for (int i = 0; i < size * size; i++) {
        field[i / size][i % size] = tiles[i];
        if (tiles[i] == 0) {
            zero_i = i / size;
            zero_j = i % size; // запоминает позицию пустой плитки
        }
    }
}

// функция для загрузки сохраненной игры
// возвращает true, если загрузка успешна
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
                zero_j = j; //запоминает позицию пустой плитки
            }
        }
    }
    loadFile.close();
    return true;
}

// функция для выбора опции из списка
// возвращает индекс выбранной позиции
int select_option(const vector<string>& options, const string& message) {
    int selected = 0;
    while (true) {
        system("cls");
        cout << message << endl;
        for (int i = 0; i < options.size(); i++) {
            if (i == selected) {
                cout << "> " << options[i] << " <" << endl; // выделение выбранной позиции
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

// основная игровая функция
// запускает новую игру или загружают сохраненную игру
void startGame(bool load_saved_game = false) {
    time_t start_time = time(0); // запоминает время для начала игры
    int moves = 0; // счетчик ходов
    srand(time(0));
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // hConsole - дескриптор консоли для управления курсором
    CONSOLE_CURSOR_INFO cursorInfo; //информация о курсоре консоли
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false; // скрыть курсор
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    while (true) {
        int size;
        int field[6][6]; // игровое поле, field[6][6] - массив для хранения игрового поля
        int zero_i, zero_j; // позиция пустой плитки

        if (load_saved_game) {
            // загрузка сохраненной игры
            if (!loadGame(field, size, zero_i, zero_j)) {
                cout << "Нет сохраненной игры!" << endl;
                system("pause");
                load_saved_game = false;
                continue;
            }
        }
        else {
            // создание нового игрового поля
            vector<string> size_options = { "4x4", "5x5", "6x6" };
            int selected_size_option = select_option(size_options, "Выберите размер игрового поля:");
            size = selected_size_option + 4;
            create_field(field, size, zero_i, zero_j);
        }

        system("cls");

        // основной игровой цикл
        while (true) {
            print_field(field, size, hConsole); // отображение игрового поля
            if (check_win(field, size)) { // проверка победы
                vector<string> win_options = { "Да", "Нет" };
                int selected_win_option = select_option(win_options, "Вы победили! Хотите продолжить?");
                if (selected_win_option == 0) {
                    break; // продолжить игру
                }
                else {
                    // сохранение результатов игры
                    time_t end_time = time(0);
                    double time_spent = difftime(end_time, start_time);
                    ofstream recordFile("records.txt", ios::app);
                    if (recordFile.is_open()) {
                        recordFile << size << "x" << size << " "
                            << moves << " " << time_spent << endl;
                        recordFile.close();
                    }
                    cursorInfo.bVisible = true; // вернуть курсор
                    SetConsoleCursorInfo(hConsole, &cursorInfo);
                    return;
                }
            }
            if (move(field, size, zero_i, zero_j)) { // обработка перемещений и команд
                cursorInfo.bVisible = true; // вернуть курсор
                SetConsoleCursorInfo(hConsole, &cursorInfo);
                return;
            }
            moves++;
        }
    }

    cursorInfo.bVisible = true; // вернуть курсор
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

int main() {
    setlocale(LC_ALL, "Russian");
    GameRules gameRules; // экземпляр класса правил игры
    vector<string> main_menu_options = { "Начать игру", "Загрузить игру", "Информация", "Рекорды", "Выход из игры" };
    int choice;

    // основный цикл меню
    while (true) {
        choice = select_option(main_menu_options, "Добро пожаловать в Console Game!");
        switch (choice) {
        case 0:
            cout << "Игра начинается..." << endl;
            startGame(); // начать игру
            break;
        case 1:
            cout << "Загрузка игры..." << endl;
            startGame(true); // загрузить сохраненную игру
            break;
        case 2:
            cout << "Помощь" << endl;
            gameRules.displayRules(); // отобразить правила игры
            break;
        case 3:
            cout << "Рекорды" << endl;
            {
                // отобразить рекорды
                cout << "Рекорды" << endl;
                ifstream recordFile("records.txt");
                if (recordFile.is_open()) {
                    string line;
                    vector<string> records;
                    while (getline(recordFile, line)) {
                        records.push_back(line);
                    }
                    recordFile.close();
                    // сортировка рекордов
                    sort(records.begin(), records.end(), [](const string& a, const string& b) {
                        istringstream iss1(a);
                        istringstream iss2(b);
                        int size1, size2, moves1, moves2;
                        double time1, time2;
                        iss1 >> size1 >> moves1 >> time1;
                        iss2 >> size2 >> moves2 >> time2;
                        if (size1 != size2) {
                            return size1 > size2;
                        }
                        else if (moves1 != moves2) {
                            return moves1 < moves2;
                        }
                        else {
                            return time1 < time2;
                        }
                        });
                    for (const auto& record : records) {
                        cout << record << endl;
                    }
                }
                else {
                    cout << "Нет записей." << std::endl;
                }
                break;
            }
        case 4:
            cout << "До свидания!" << std::endl;
            return 0; // выход из игры
        default:
            cout << "Неправильный выбор. Пожалуйста, введите номер от 1 до 5." << std::endl;
            break;
        }
        system("pause");
        clearConsole();
    }

    return 0;
}
