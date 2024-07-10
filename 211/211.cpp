#include <iostream>
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
            //код для игры
            break;
        case 2:
            std::cout << "Помощь" << std::endl;
           gameRules.displayRules();
           
            break;
        case 3:
            std::cout << "Рекорды" << std::endl;
            //код для таблицы рекордов
            
            break;
        case 4:
           std::cout << "Досвидание!" << std::endl;
            return 0;
        default:
            std::cout << "Неправельный =выбор. Пожалуйста введите номер от 1 до 4" << std::endl;
            break;
        }
        system("pause");
        clearConsole();
    }

    return 0;
}
