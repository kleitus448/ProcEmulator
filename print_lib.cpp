#include <iostream>
#include <conio.h>
#include <string>

using namespace std;

/**
 * ������� �������� ������ ����� � �������� ���
 *
 * @param value - ��������,
 * @param  size - �����������
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
 * ������� ������ �������� � ������� � �������� ����
 *
 * @param value - ��������,
 * @param  size - �����������
 */
void print_binary(int value, int size) {
    std::cout << get_binary(value, size);
}

/**
 * ������� ������ ������ ������ � �������
 *
 * @param mem[] - ������ ������
 * @param mem_size - ������ ������� ������
 * @param addr_size - ����������� ������ ������ ������
 * @param value_size - ����������� �������� � ������ ������
 * @param restrict - ���-�� ����� � ����� ������ ��� ������
 */
/*template<class T, int N>
void print_mem(T (&mem)[N],
               unsigned short int mem_size,
               unsigned short int addr_size,
               unsigned short int value_size,
               unsigned short int restrict) {

    // ����� ��������� "�����"
    cout << "�����";
    for (int j = 0; j < addr_size/4; j++)
        cout << "\t";
    cout << "| ";

    // ����� ��������� "��������"
    for (int i = 0; i < restrict; i++) {
        string value_name = "��������(+" + to_string(i) + ")";
        cout << value_name;
        for (unsigned int j = 0; j < value_size-value_name.size(); j++)
            cout << " ";
        cout << " | ";
    }
    cout << endl;

    // ����� ������� � ������
    for (int i = 0; i < mem_size/restrict; i++) {

        // ����� ������� �������� ������� ������
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
    setlocale(LC_ALL, "Russian"); // ������� ����

    // ���������� ������� ������ ������
    // � ������������� ��� ������
    const int dmem_size = 64;
    int dmem[dmem_size];
    for (int i = 0; i < dmem_size; i++) dmem[i] = 0;

    // ���������� ������� ������
    int addr_size = 0; int dmem_size_temp = dmem_size;
    while(dmem_size_temp != 1) {dmem_size_temp >>= 1; addr_size++;}

    cout << "������ ������: " << addr_size << endl << endl;

    dmem[5] = 250;

    print_mem(dmem, dmem_size, addr_size, PROC_BIT, 4);
}*/



