#include <iostream>
#include <conio.h>
#include <string>

using namespace std;

/**
 * Функция перевода целого числа в бинарный вид
 *
 * @param value - значение,
 * @param  size - размерность
 */
string get_binary(int value, int size) {
    int i = 0;
    string binary_string = "";
    while (i < size) {
        if (value == 0) {binary_string = "0" + binary_string;}
        else {binary_string = (char(value & 1) ? "1" : "0") + binary_string;}
        value >>= 1; i++;
    }
    return binary_string;
}

/**
 * Функция вывода значения в консоль в бинарном виде
 *
 * @param value - значение,
 * @param  size - размерность
 */
void print_binary(int value, int size) {
    std::cout << get_binary(value, size);
}

/**
 * Функция вывода данных памяти в консоль
 *
 * @param mem[] - массив памяти
 * @param mem_size - размер массива памяти
 * @param addr_size - размерность адреса ячейки памяти
 * @param value_size - размерность значения в ячейке памяти
 * @param restrict - кол-во ячеек в одной строке при выводе
 */
/*template<class T, int N>
void print_mem(T (&mem)[N],
               unsigned short int mem_size,
               unsigned short int addr_size,
               unsigned short int value_size,
               unsigned short int restrict) {

    // Вывод заголовка "Адрес"
    cout << "Адрес";
    for (int j = 0; j < addr_size/4; j++)
        cout << "\t";
    cout << "| ";

    // Вывод заголовка "Значение"
    for (int i = 0; i < restrict; i++) {
        string value_name = "Значение(+" + to_string(i) + ")";
        cout << value_name;
        for (unsigned int j = 0; j < value_size-value_name.size(); j++)
            cout << " ";
        cout << " | ";
    }
    cout << endl;

    // Вывод адресов и данных
    for (int i = 0; i < mem_size/restrict; i++) {

        // Вывод старших разрядов первого адреса
        int begin_address = i * restrict;
        print_binary(begin_address, addr_size);
        cout << "\t| ";

        for (int j = 0; j < restrict; j++) {
            print_binary(mem[begin_address+j], value_size);
            cout << " | ";
        }
        cout << endl;
    }
}*/

/*int main() {
    setlocale(LC_ALL, "Russian"); // русский язык

    // Объявление массива памяти данных
    // и инициализация его нулями
    const int dmem_size = 64;
    int dmem[dmem_size];
    for (int i = 0; i < dmem_size; i++) dmem[i] = 0;

    // Нахождение размера адреса
    int addr_size = 0; int dmem_size_temp = dmem_size;
    while(dmem_size_temp != 1) {dmem_size_temp >>= 1; addr_size++;}

    cout << "Размер адреса: " << addr_size << endl << endl;

    dmem[5] = 250;

    print_mem(dmem, dmem_size, addr_size, PROC_BIT, 4);
}*/



