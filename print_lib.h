#ifndef PRINT_LIB
#define PRINT_LIB
#include <string>

using namespace std;

/**
 * ������� �������� ������ ����� � �������� ���
 *
 * @param value - ��������,
 * @param  size - �����������
 */
string get_binary(int value, int size);

/**
 * ������� ������ �������� � ������� � �������� ����
 *
 * @param value - ��������,
 * @param  size - �����������
 */
void print_binary(int value, int size);

/**
 * ������� ������ ������ ������ � �������
 *
 * @param mem[] - ������ ������
 * @param mem_size - ������ ������� ������
 * @param addr_size - ����������� ������ ������ ������
 * @param value_size - ����������� �������� � ������ ������
 * @param restrict - ���-�� ����� � ����� ������ ��� ������
 */
template<class T, int N>
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
}

#endif // PRINT_LIB
