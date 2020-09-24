/** -------------------------------------------------------------- *
 * ИВМО-01-20, Павел Комаров, Вариант 4 = 0100
 * Гарвардская архитектура, 2-х адрес. пространство
 * Максимальное значение в массиве
 *
 * Характеристики устройства:
 *
 * 1. Размер команды - 32 бита
 * |---------------|---------|-------------|------------|
 * | Номер команды | Литерал | 1-ый адрес  | 2-ой адрес |
 * |---------------|---------|-------------|------------|
 * |     5 бита    | 17 бит  |    5 бит    |    5 бит   |
 * |---------------|---------|-------------|------------|
 *
 * 2. Кол-во регистров общего назначения (РОН) - 16.
 * Размерность регистров - 16 бит.
 *
 * 3. Память команд - 32 ячейки по 32 бита = 1024 бита = 128 байт
 *    Память данных -  64 ячейки по 16 бит = 1024 бита = 128 байт
 *
 * 4. Флаги - рав-во нулю, больше нуля, меньше нуля, переполнение.
 * --------------------------------------------------------------- */

#include <conio.h>      // Библиотека для работы с функцией getch
#include <cstdlib>      // Библиотека для работы с функцией system
#include <iostream>     // Библиотека для работы с консольными потоками ввода/вывода
#include <fstream>      // Библиотека для работы с файловыми потоками ввода/вывода
#include "print_lib.h"  // Библиотека для дебаг-вывода
#include <windows.h>    // Библиотека для изменения размеров консольного окна
#include <string>       // Библиотека для типа "строка"
#include <fstream>      // Библиотека для чтения/записи файлов
#include <regex>        // Библиотека для регулярных выражений
#include <algorithm>    // Библиотека для трансформации строки в нижний регистр
#include <map>          // Библиотека для использования ассоциативных массивов "Map"
#include <utility>      // Библиотека для использования типа "Pair"
#include <vector>       // Библиотека для использования динамических массивов "Vector"
#include <ctype.h>

// Флаги условий
#define FLAG 15 // Номер регистра флагов
#define EZ 1    // Флаг " == 0"
#define GZ 2    // Флаг "  > 0"
#define LZ 4    // Флаг "  < 0"

// Коды клавиш для действий
// внутри контроллера
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_O 111
#define KEY_Y 121
#define KEY_N 110
#define KEY_Q 113

#define KEY_0 48
#define KEY_1 49
#define KEY_2 50
#define KEY_3 51
#define KEY_4 52
#define KEY_5 53
#define KEY_6 54

// Коды команд
#define EMPTY 0
#define ADD   1
#define SUB   2
#define AND   3
#define OR    4
#define XOR   5
#define NOT   6
#define SHL   7
#define SHR   8
#define IREG  9
#define IMEM  10
#define RTR   11
#define MTM   12
#define RTM   13
#define MTR   14
#define MTRA  15
#define JEZ   16
#define JGZ   17
#define JLZ   18
#define JP    19

// Инициализация памяти команд
const unsigned short int cmem_size = 32;
const unsigned short int cmem_addr_size = 5;
const unsigned short int cmem_data_size = 32;
unsigned int cmem[cmem_size] = {};

// Инициализация памяти данных
const unsigned short int dmem_size = 32;
const unsigned short int dmem_addr_size = 5;
const unsigned short int dmem_data_size = 16;
short int dmem[dmem_size] = {};

// Инициализация массива регистров
const unsigned short int reg_size = 16;
const unsigned short int reg_addr_size = 4;
const unsigned short int reg_data_size = 16;
short int reg[reg_size] = {};

// Размеры частей команды
unsigned int cmdtype_size = 5,  // Тип команды
             literal_size = 17, // Литерал
                 op1_size = 5,  // 1-й адрес
                 op2_size = 5;  // 2-й адрес

int cmd_count = 20; // кол-во команд
string cmd_list[] = {"empty", "add", "sub", "and", "or", "xor",
                     "not", "shl", "shr", "ireg", "imem", "rtr",
                     "mtm", "rtm", "mtr", "mtra", "jez", "jgz",
                     "jlz", "jp"};

using namespace std;

//const int data_addr_begin = // Первый адрес в ОП для записи данных
//const int prog_data_begin = // Первый адрес в ОП для записи программы

/** ------------------ *
    ПРОТОТИПЫ ФУНКЦИЙ
  * ------------------ */

// Функция для получения информации из файла с даннными/командами
vector<string> get_info_from_file();

// Функция для сохранения информации в файл с даннными/командами
void save_info_in_file(string info);

// Функция для загрузки сохранённого состояния устройства
void load_saved_state();

// Функция для отображения начального меню работы с процессором
void show_menu();

// Функция для настройки работы процессора
void config_output_debug();

// Функция работы архитектуры
void architecture_work();

// Функция сохранения состояния архитектуры
void save_current_state();

// Фунцкия для парсинга программного кода и перевода его в двоичный код
vector<string> parse_program(vector<string> program);

// Функция для загрузки программы (в двоичном коде) в память
void load_program_to_memory(vector<string> program);

// Функция для загрузки данных в память
void load_data_to_memory(vector<string> data);

// Функция для отладочных выводов
void output_debug(char settings);

char output_debug_settings = 0xFF;

/** ----------------- *
    ОСНОВНАЯ ФУНКЦИЯ
  * ----------------- */

int main() {
    HWND console = GetConsoleWindow();
    RECT ConsoleRect;
    GetWindowRect(console, &ConsoleRect);
    MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 1280, 720, TRUE);
    setlocale(LC_ALL, "Russian"); // русский язык

    cout << "Консольная эмуляция модели процессорного ядра." << endl << endl;
    load_saved_state();
    show_menu(); // Отображение меню
    cout << "\n\nНачало эмуляции работы устройства...\n\n";
    architecture_work(); // Эмуляция работы устройства
    cout << "\nРабота с устройством завершена.\n";
    save_current_state();
}

/** ----------------------------------------- *
    ОПРЕДЕЛЕНИЕ ФУНКЦИЙ ДЛЯ РАБОТЫ С ФАЙЛАМИ
  * ----------------------------------------- */

// Функция для получения информации из файла с даннными/командами
vector<string> get_info_from_file() {

    bool file_flag = false; // Флаг работы с файлом
    string path; // путь к файлу
    string line; // строка файла
    vector<string> text; // ассоциативный массив строк из файла

    while (!file_flag) {
        cout << endl << "Укажите путь к файлу (полный или относительно директории эмулятора): ";
        cin >> path; // Ввод пути

        ifstream file(path);
        // Если файл открыт
        if (file.is_open()) {

            // Подсчёт количества строк файла
            int line_count = 0;
            while (getline(file, line)) line_count++;
            text.resize(line_count);

            // Запись строк из файла в массив
            file.clear(); file.seekg(0);
            for (int i = 0; i < line_count; i++) {
                getline(file, line);
                text[i] = line;
            }

            // Закрытие файла
            file.close();
            file_flag = true;
        }
        else {
            cout << "Неправильный путь. Попробуйте ещё раз,\n";
        }
    }
    return text;
}

// Функция для сохранения информации в файл с даннными/командами
void save_info_in_file(string info) {

    bool file_flag = false;
    string path; // Путь к файлу

    while (!file_flag) {
        cout << "Укажите путь к файлу (полный или относительно директории эмулятора): ";
        cin >> path; // Ввод пути

        // Запись строк в файл
        ofstream file(path);
        file << info;

        // Закрытие файла
        file.close();
        file_flag = true;
    }
}

/** ------------------------------------------- *
    ОПРЕДЕЛЕНИЕ ФУНКЦИЙ ДЛЯ ПАРСИНГА ПРОГРАММЫ
  * ------------------------------------------- */

// Функция генерации ошибки парсинга
vector<string> generate_parse_error(vector<string> program, string error_type, int index) {
     cout << endl << error_type << " в " << index << endl;
     program.resize(1);
     program[0] = "ERROR";
     return program;
}

// Функция для чистки строки от пробелов и табуляций
string clear_string_from_spaces(string str) {
    int pointer = 0;
    while (!isalpha(str[pointer])) pointer++;
    if (pointer > 0) str.erase(0, pointer);
    return str;
}

// Фунцкия для парсинга программного кода и перевода его в двоичный код
vector<string> parse_program(vector<string> program) {
    // TODO: Добавить возможность писать команды с табуляциями и пробелами (даже между частями)

    string cmdtype, literal, op1, op2;  // Строки для отделения частей команды
    string part_sep = " ";              // Разделители частей команды

    string label_sep = ":";             // Разделитель метки
    map <string, int> label_map;        // Ассоциативный массив хранения меток <"метка", "номер строки">

    // Регулярные выражения для всех возможных типов команд
    regex addr2_regex("^(\\s)*(add|sub|and|or|xor|shl|shr|rtr|mtm|rtm|mtr|mtra){1}(\\s)+([0-9])+,(\\s)*([0-9]$)+$");
    regex addr1_regex("^(\\s)*(not|jez|jgz|jlz|jp){1}(\\s)+([0-9])+$");
    regex condition_mark_regex("^(\\s)*(jez|jgz|jlz|jp){1}(\\s)+([A-Za-z])+$");
    regex literal_regex("^(\\s)*(ireg|imem){1}(\\s)+[0-9]+,(\\s)*([0-9])+$");
    regex empty_regex("^(\\s)*empty(\\s)*$");
    regex cmd_regex("[^\\n]+");
    regex no_mean_regex("(^(//.*)$)|(^[(\\t)(\\s)]*$)");

    // Флаги соответствия команды регулярным выражениям
    bool condition_mark_match, addr1_match, addr2_match,
         literal_match, label_match, empty_match, no_mean_match;

    // Регулярное выражение для выделения метки
    regex label_regex("[A-Za-z0-9]+:(.*)");

    /** ----------------------------------------------------- *
        Первый цикл проходится по программе и ищет метки.
        Если они находятся, они добавляются в ассоциативный
        контейнер label_map <"название метки", "номер строки">
      * ----------------------------------------------------- */
    unsigned int bin_pointer = 0;
    string cmd;
    for (unsigned int i = 0; i < program.size(); i++) {ФS
        cmd = program[i];
        cout << cmd << endl;

        no_mean_match = regex_match(cmd, no_mean_regex);
        if (!no_mean_match) { // Строка кода не пустая
            cmd = clear_string_from_spaces(cmd);

            // Если метка найдена, происходит её отделение от
            // команды и занесение в ассоциативный массив
            label_match = regex_match(cmd, label_regex);

            if (label_match) { // Строка имеет метку
                string label = cmd.substr(0, cmd.find(label_sep));
                cmd.erase(0, cmd.find(label_sep) + 1);
                cmd = clear_string_from_spaces(cmd);
                label_map[label] = i;
            }
            program[bin_pointer] = cmd;
            bin_pointer++;
        }
    }
    cout << "Размер программы: " << program.size() << endl;
    cout << "Размер реальной программы: " << bin_pointer << endl;
    program.resize(bin_pointer);

    cout << (label_map.size() > 0 ? "Метки найдены" : "Метки НЕ найдены");
    cout << endl << endl;
    getch();

    /** ------------------------------------------------------------ *
        Второй цикл проходится по программе и парсит каждую команду,
        разделяя её на код команды, литерал и адреса/метки адресов
      * ------------------------------------------------------------ */
    for (unsigned int i = 0; i < program.size(); i++) {
        string cmd = program[i];
        cout << "CURRENT CMD: " << cmd << endl;
        // Вывод текущей команды
        // cout << "CURRENT CMD: " << cmd << endl;

        // Проверяем соответствие команды
        // с регулярными значениями
        addr2_match = regex_match(cmd, addr2_regex);
        addr1_match = regex_match(cmd, addr1_regex);
        literal_match = regex_match(cmd, literal_regex);
        condition_mark_match = regex_match(cmd, condition_mark_regex);
        empty_match = regex_match(cmd, empty_regex);
        cout << "AFTER REGEX: " << addr2_match << addr1_match;
        cout << literal_match << condition_mark_match;
        cout << empty_match << endl;

        // Если соответствий не найдено, возвращается ошибка
        if (!(addr1_match || addr2_match || literal_match ||
              condition_mark_match || empty_match))
            generate_parse_error(program, "error_match", i);
        else if (empty_match)
            program[i] = get_binary(0, cmdtype_size + literal_size + op1_size + op2_size);
        else {
            /** -------------------- *
                ПАРСИНГ КОДА КОМАНДЫ
              * -------------------- */
            cmdtype = cmd.substr(0, cmd.find(part_sep));
            cout << "cmd_type: " << cmdtype << endl;

                // Убираем из кода команды пробелы и табуляции, удаляем его из команды
                cmdtype.erase(remove_if(cmdtype.begin(), cmdtype.end(), ::isspace), cmdtype.end());
                cmd.erase(0, cmd.find(part_sep) + 1);

                // Сводим команду к единому регистру (маленькие буквы) и ищем
                // её в массиве команд. Индекс команды в массиве - код этой команды
                transform(cmdtype.begin(), cmdtype.end(), cmdtype.begin(), ::tolower);
                for (int j = 0; j < cmd_count; j++)
                    if (cmdtype.compare(cmd_list[j]) == 0) {
                        cmdtype = get_binary(j, cmdtype_size);
                        break;
                    }

            /** ----------------------------- *
                ПАРСИНГ 1-ГО АДРЕСА В КОМАНДЕ
              * ----------------------------- */
            op1 = cmd.substr(0, cmd.find(part_sep));
            cout << "op1: " << op1 << endl;

                // Если вместо адреса стоит метка, то производится подстановка
                // адреса вместо неё (для условных и безусловных переходов)
                if (condition_mark_match) {
                    // Убираем из метки пробелы и табуляции
                    op1.erase(remove_if(op1.begin(), op1.end(), ::isspace), op1.end());

                    // Инициализируем итератор массива меток
                    map <string, int> :: iterator iter = label_map.begin();

                    // Пробегаемся по массиву меток и ищем совпадение ключа
                    // с названием метки из команды
                    for (; iter != label_map.end(); iter++)
                        if (op1.compare(iter->first) == 0) {
                            op1 = get_binary(iter->second, op1_size);
                            break;
                        }

                    // Если итератор дошёл до конца, значит метка
                    // не была найдена. Выбрасываем ошибку.
                    if (iter == label_map.end())
                        generate_parse_error(program, "error_condition", i);
                }

                // Если команда не относится к БП или УП,
                // конвертируем адрес в бинарный код
                else op1 = get_binary(stoi(op1), op1_size);

                // Удаляем часть из команды
                cmd.erase(0, cmd.find(part_sep) + 1);

            /** -------------------------------------------- *
                ПАРСИНГ 2-ГО АДРЕСА В КОМАНДЕ (ЕСЛИ ОН ЕСТЬ)
              * -------------------------------------------- */
            if (addr2_match)
                op2 = get_binary(stoi(cmd.substr(0, cmd.find(part_sep))), op2_size);
            else
                op2 = get_binary(0, op2_size);

            /** ----------------------------------------- *
                ПАРСИНГ ЛИТЕРАЛА В КОМАНДЕ (ЕСЛИ ОН ЕСТЬ)
              * ----------------------------------------- */
            if (literal_match)
                literal = get_binary(stoi(cmd.substr(0, cmd.find(part_sep))), literal_size);
            else
                literal = get_binary(0, literal_size);

            program[i] = cmdtype + literal + op1 + op2;
            cout << "BINARY: " << cmdtype << " ";
            cout << literal << " " << op1 << " " << op2 << endl << endl;
        }
    }
    return program;
};

/** ---------------------------------------------------------------- *
    ОПРЕДЕЛЕНИЕ ФУНКЦИЙ ДЛЯ РАБОТЫ С ПАМЯТЬЮ ДАННЫХ И ПАМЯТЬЮ КОМАНД
  * ---------------------------------------------------------------- */

// Функция для загрузки данных в память
void load_data_to_memory(vector<string> data) {
    string colon = ":";
    /*regex line_regex("[^\\n]+");

    auto file_begin = sregex_iterator(data.begin(), data.end(), line_regex);
    auto file_end = sregex_iterator();*/
    for (unsigned int i = 0; i < data.size(); i++) {
        string data_elem = data[i];
        string addr = data_elem.substr(0, data_elem.find(colon));
        data_elem.erase(0, data_elem.find(colon) + 1);
        dmem[stoi(addr)] = stoi(data_elem);
    }
};

// Функция для загрузки программы (в двоичном коде) в память
void load_program_to_memory(vector<string> program) {
    if (program[0] == "ERROR") {
        cout << endl << program[0] << endl;
    }
    else {
        int cmem_pointer = 0; // ?
        cout << "Записанная программа:" << endl;
        for (unsigned int i = 0; i < program.size(); i++) {
            cout << program[i] << endl;
            cmem[cmem_pointer + i] = stoll(program[i], 0, 2);
        }
    }
        /*cout << "Программа заняла " << "n" << " ячеек." << endl;
    cout << "Начальный адрес: " << "" << endl;
    cout << " Конечный адрес: " << "" << endl;
    if (false) {
        cout << "Программа не помещается в память, начиная с посл. адреса записи." << endl;
        cout << "Дальнейшие действия:";
        cout << "1. Записать программу с 0-го адреса памяти команд.";
        cout << "2. Продолжить запись (непоместившиеся команды запишутся с 0-го адреса).";
        cout << "3. Отмена записи";
        cout << "Выберите действие: ";
        while(key_flag) {}
    }*/
};

/** ------------------------------------------------------ *
    ОПРЕДЕЛЕНИЕ ФУНКЦИЙ ДЛЯ НАСТРОЙКИ И РАБОТЫ УСТРОЙСТВА
  * ------------------------------------------------------ */

// Функция для отображения начального меню работы с архитектурой
void show_menu() {
    bool start_processor = false; // Флаг запуска работы процессора

    // Ввод программы вручную
    // Ввод данных вручную

    // Запуск программы
    cout << "0. Настройки отладочных выводов." << endl;
    cout << "1. Загрузить программу из файла (двоичный код)." << endl;
    cout << "2. Загрузить программу из файла (Assembler)." << endl;
    cout << "3. Загрузить данные для ОП." << endl;
    cout << "4. Вывести память команд." << endl;
    cout << "5. Вывести память данных." << endl;
    cout << "6. Начать работу процессора." << endl;

    cout << endl << "Выберите действие: ";
    int key; //  Код нажатой кнопки
    while (!start_processor) {
        switch(key = getch()) {
            case KEY_0: { // Настройки отладочных выводов
                config_output_debug();
                cout << endl << "Выберите действие: ";
                break;
            }

            case KEY_1: { // Загрузить программу из файла (двоичный код)
                load_program_to_memory(get_info_from_file());
                cout << endl << "Выберите действие: ";
                break;
            }

            case KEY_2: { // Загрузить программу из файла (Assembler)
                load_program_to_memory(parse_program(get_info_from_file()));
                cout << endl << "Выберите действие: ";
                break;
            }

            case KEY_3: { // Загрузить данные для ОП
                load_data_to_memory(get_info_from_file());
                cout << endl << "Выберите действие: ";
                break;
            }

            case KEY_4: { // Вывести память команд
                output_debug(2);
                cout << endl << "Выберите действие: ";
                break;
            }

            case KEY_5: { // Вывести память данных
                output_debug(4);
                cout << endl << "Выберите действие: ";
                break;
            }

            case KEY_6: { // Начать работу процессора
                start_processor = true;
                break;
            }
        }
    }
}

// Функция для настройки работы с архитектурой
void config_output_debug() {
    string settings;

    cout << endl << "Настройка отладочных выводов." << endl;
    cout << "Текущие настройки: ";
    print_binary(output_debug_settings, 8); cout << endl;
    cout << "xx1 - вывод регистров РОН" << endl;
    cout << "x1x - вывод памяти команд" << endl;
    cout << "1xx - вывод памяти данных" << endl;
    cout << "Введите число в двоичном коде: ";

    cin >> settings;
    output_debug_settings = (char) stoi(settings, 0, 2);
}

// Функция работы архитектуры
void architecture_work() {

    // Вычисляем макс. значение типа команды и литерала
    unsigned int cmdtype_max = 1; for (unsigned int i = 0; i < cmdtype_size; i++) cmdtype_max *= 2;
    unsigned int literal_max = 1; for (unsigned int i = 0; i < literal_size; i++) literal_max *= 2;
    unsigned int op1_max = 1; for (unsigned int i = 0; i < op1_size; i++) op1_max *= 2;
    unsigned int op2_max = 1; for (unsigned int i = 0; i < op2_size; i++) op2_max *= 2;

    //bool  prev_flag = false;  // Флаг перехода к предыдущей команде
    bool  next_flag = false;  // Флаг перехода к следующей команде
    bool debug_flag = false;  // Флаг перехода к настройкам
    bool   end_flag = false;  // Флаг перехода к завершению работы

    unsigned int cmd; //буфер команд
    unsigned int pc = 0, old_pc; //счетчик команд

    //  Разбитая команда
    unsigned int cmdtype, op1, op2;
    int literal;

    int key = 0;
    while(!end_flag) {
        old_pc = pc;
        cout << "*";
        for (int i = 0; i < 150; i++) cout << "-";
        cout << "*" << endl << endl;

        // Выбираем команду по адресу из счётчика команд
        cmd = cmem[pc];

        // Путём сдвига и операцией "И" с битовой маской
        // выделяем два операнда, литерал и тип команды
        // (тип "0" - пустая команда)
            op2 = (cmd & (op1_max-1));
            op1 = (cmd >> op1_size) & (op2_max-1);
        literal = (cmd >> (op1_size + op2_size)) & (literal_max-1);
        cmdtype = (cmd >> (op1_size + op2_size + literal_size));

        // Арифметические и логические операции (1-8)
        if (cmdtype >= ADD && cmdtype <= SHR) {
            switch (cmdtype) {
                case ADD : reg[op1]  = reg[op1]  +  reg[op2]; break; // Сложение
                case SUB : reg[op1]  = reg[op1]  -  reg[op2]; break; // Вычитание
                case AND : reg[op1]  = reg[op1] and reg[op2]; break; // Побитовое "И"
                case OR  : reg[op1]  = reg[op1] or  reg[op2]; break; // Побитовое "ИЛИ"
                case XOR : reg[op1]  = reg[op1] xor reg[op2]; break; // Побитовое "Сложение по модулю 2"
                case NOT : reg[op1]  = not reg[op1];          break; // Отрицание
                case SHL : reg[op1]  = reg[op1] << reg[op2];  break; // Сдвиг на op2 разрядов влево
                case SHR : reg[op1]  = reg[op1] >> reg[op2];  break; // Сдвиг на op1 разрядов вправо
                 default : break;
            }

            // Выставление флагов после операций
            // TODO: Добавить переполнение
            if (reg[op1] == 0) reg[FLAG] = EZ;
            if (reg[op1]  > 0) reg[FLAG] = GZ;
            if (reg[op1]  < 0) reg[FLAG] = LZ;
        }

        // Другие операции (9-18)
        else {
            switch (cmdtype) {
                // Взаимодействие между регистрами/ячейками памяти данных
               case IREG :   reg[op1] =   literal;      break; // Копирование непосредственного значения в регистр
                case IMEM : dmem[op1] =   literal;      break; // Копирование непосредственного значения в память
                case RTR  :  reg[op1] =  reg[op2];      break; // Копирование с одного регистра на другой регистр
                case MTM  : dmem[op1] = dmem[op2];      break; // Копирование с одной ячейки памяти в другую ячейку памяти
                case RTM  : dmem[op1] = reg[op2];       break; // Копирование с регистра в ячейку памяти
                case MTR  :  reg[op1] = dmem[op2] ;     break; // Копирование из ячейки памяти (непосредственно) в регистр
                case MTRA :  reg[op1] = dmem[reg[op2]]; break; // Копирование из ячейки памяти (адрес в регистре) в регистр

                // Операции условного перехода (если выполняется, на счётчик команд поступает
                // значение литерала, в противном случае счётчик команд инкрементируется)
                case JEZ : pc = (reg[FLAG] & EZ) ? op1 : pc + 1; break; // Значение на reg[op1] == 0
                case JGZ : pc = (reg[FLAG] & GZ) ? op1 : pc + 1; break; // Значение на reg[op1] >  0
                case JLZ : pc = (reg[FLAG] & LZ) ? op1 : pc + 1; break; // Значение на reg[op1] <  0
                case JP  : pc = op1;                             break; // безусловный переход

                default : break;
            }
        }

        cout <<  "Счётчик команд: ";
        print_binary(old_pc, cmem_addr_size);
        cout << endl << "Текущая команда:" << endl;
        cout << "Тип команды | Литерал\t\t| 1-ый адрес | 2-ой адрес |" << endl;
        print_binary(cmdtype, cmem_addr_size);  cout << "\t    | ";
        print_binary(literal, literal_size);    cout << " | ";
        print_binary(op1, op1_size);            cout << "      | ";
        print_binary(op2, op2_size);            cout << "      |" << endl;

        output_debug(output_debug_settings);

        cout << endl;
        //cout << "\"Стрелка вверх\" - предыдущая команда, ";
        cout << "\"Стрелка вниз\" - следующая команда, ";
        cout << "\"O\" - настройки, ";
        cout << "\"Q\" - закончить работу процессора";
        while (!next_flag && !end_flag) {
            switch(key = getch()) {
                // Предыдущая команда?
                // case KEY_UP   :  prev_flag = true; break;
                case KEY_DOWN :  next_flag = true; break;
                case KEY_O    : debug_flag = true; break;
                case KEY_Q    :   end_flag = true; break;
                default: break;
            }

            if (debug_flag) {
                config_output_debug();
                debug_flag = false;
                cout << "\"Стрелка вниз\" - следующая команда, ";
                cout << "\"O\" - настройки, ";
                cout << "\"Q\" - закончить работу процессора";
            }
        }

        if (!end_flag) {
            key = 0;
            next_flag = false;
            debug_flag = false;
            if (!(cmdtype >= JEZ && cmdtype <= JP))
                pc = pc + 1;
            cout << '\r';
            // Сброс счётчик команд при достижении
            // последнего адреса в памяти команд
            if (pc > cmem_size) pc = 0;
        }
    }
}

// Функция для отладочных выводов
void output_debug(char settings) {

    if (settings & 1) {
        cout << endl << "Регистры:" << endl;
        cout << "Значения регистра флагов: ";
        print_binary(reg[FLAG], reg_data_size); cout << endl;
        print_mem(reg, reg_size, reg_addr_size, reg_data_size, 4);
}

    if (settings & 2) {
        cout << endl << "Память команд:" << endl;
        print_mem(cmem, cmem_size, cmem_addr_size, cmem_data_size, 4);
    }

    if (settings & 4) {
        cout << endl << "Память данных:" << endl;
        print_mem(dmem, dmem_size, dmem_addr_size, dmem_data_size, 4);
    }
}

/** -------------------------------------------------------------- *
    ОПРЕДЕЛЕНИЕ ФУНКЦИЙ ДЛЯ СОХРАНЕНИЯ И ЗАГРУЗКИ СОСТОЯНИЯ МОДЕЛИ
  * -------------------------------------------------------------- */

// Функция для загрузки сохранённого состояния модели
void load_saved_state() {
    bool accept_flag, key_flag;
    int key;

    cout << "Вы хотите загрузить состояние модели? (Y/N) ";
    while(!key_flag) {
        switch(key = getch()) {
            case KEY_Y:
                key_flag = true;
                accept_flag = true;
                break;
            case KEY_N:
                key_flag = true;
                break;
            default:
                break;
        }
    }

    if (accept_flag) {
        cout << endl << "Загрузка ранее сохранённого состояния памяти команд и памяти данных..." << endl;
        vector<string> saved_prog_memory, saved_data_memory;
        string colon = ":";
        regex line_regex("[^\\n]+");

        cout << "1. Файл памяти команд";
        saved_prog_memory = get_info_from_file();
        for (unsigned int i = 0; i < saved_prog_memory.size(); i++) {
            string cmd = saved_prog_memory[i];
            string addr = cmd.substr(0, cmd.find(colon));
            cmd.erase(0, cmd.find(colon) + 1);
            cmem[stoi(addr)] = stoll(cmd);
        }

        cout << "2. Файл памяти данных";
        load_data_to_memory(get_info_from_file());

        cout << "Сохранённое состояние памяти восстановлено.";
    }
    cout << endl << endl;
}

// Функция сохранения состояния модели
void save_current_state() {
    bool accept_flag, key_flag;
    int key;

    // Вопрос о подтверждении сохранения состояния
    cout << "Вы хотите сохранить состояние модели? (Y/N) ";
    while(!key_flag) {
        switch(key = getch()) {
            case KEY_Y:
                key_flag = true;
                accept_flag = true;
                break;
            case KEY_N:
                key_flag = true;
                break;
            default:
                break;
        }
    }

    // Если есть подтверждение сохранения состояния
    if (accept_flag) {
        string memory_info = "";
        for (int i = 0; i < cmem_size; i++)
            memory_info += to_string(i) + ":" + to_string(cmem[i]) + "\n";

        cout << endl << "Сохранение памяти команд." << endl;
        save_info_in_file(memory_info);

        memory_info = "";
        for (int i = 0; i < dmem_size; i++)
            memory_info += to_string(i) + ":" + to_string(dmem[i]) + "\n";

        cout << endl << "Сохранение памяти данных." << endl;
        save_info_in_file(memory_info);
    }
}





































/*// Вычисление размера адреса памяти
unsigned short int compute_addr_size(unsigned short int mem_size) {
    int addr_size = 0;
    while(mem_size != 1) {mem_size >>= 1; addr_size++;}
    return addr_size;
}*/


  //   if(_getch()==32) system("Pause"); //установка программы на паузу пробелом с ожиданием нажатия клавиши
  //    if(_getch()==83) system("cls"); //очистка экрана при нажатии клавиши del
  //cmd = cmem[pc];
  //op1=(cmd>>4)&15 //сохранение значения первой переменной находящейся в разрядах 7-4
  //op2=cmd&15 //сохранение значения второй переменной находящейся в разрядах 3-0*/
                // Операции копирования
//TODO : template
