/** -------------------------------------------------------------- *
 * ����-01-20, ����� �������, ������� 4 = 0100
 * ����������� �����������, 2-� �����. ������������
 * ������������ �������� � �������
 *
 * �������������� ����������:
 *
 * 1. ������ ������� - 32 ����
 * |---------------|---------|-------------|------------|
 * | ����� ������� | ������� | 1-�� �����  | 2-�� ����� |
 * |---------------|---------|-------------|------------|
 * |     5 ����    | 17 ���  |    5 ���    |    5 ���   |
 * |---------------|---------|-------------|------------|
 *
 * 2. ���-�� ��������� ������ ���������� (���) - 16.
 * ����������� ��������� - 16 ���.
 *
 * 3. ������ ������ - 32 ������ �� 32 ���� = 1024 ���� = 128 ����
 *    ������ ������ -  64 ������ �� 16 ��� = 1024 ���� = 128 ����
 *
 * 4. ����� - ���-�� ����, ������ ����, ������ ����, ������������.
 * --------------------------------------------------------------- */

#include <conio.h>      // ���������� ��� ������ � �������� getch
#include <cstdlib>      // ���������� ��� ������ � �������� system
#include <iostream>     // ���������� ��� ������ � ����������� �������� �����/������
#include <fstream>      // ���������� ��� ������ � ��������� �������� �����/������
#include "print_lib.h"  // ���������� ��� �����-������
#include <windows.h>    // ���������� ��� ��������� �������� ����������� ����
#include <string>
#include <fstream>
#include <regex>
#include <algorithm>

// ����� �������
#define FLAG 15 // ����� �������� ������
#define EZ 1    // ���� " == 0"
#define GZ 2    // ���� "  > 0"
#define LZ 3    // ���� "  < 0"

// ���� ������ ��� ��������
// ������ �����������
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_O 79
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
#define JEZ   15
#define JGZ   16
#define JLZ   17
#define JP    18

// ������������� ������ ������
const unsigned short int cmem_size = 32;
const unsigned short int cmem_addr_size = 5;
const unsigned short int cmem_data_size = 32;
unsigned int cmem[cmem_size] = {};

// ������������� ������ ������
const unsigned short int dmem_size = 32;
const unsigned short int dmem_addr_size = 5;
const unsigned short int dmem_data_size = 16;
short int dmem[dmem_size] = {};

// ������������� ������� ���������
const unsigned short int reg_size = 16;
const unsigned short int reg_addr_size = 4;
const unsigned short int reg_data_size = 16;
short int reg[reg_size] = {};

// ������� ������ �������
int cmdtype_size = 5; int literal_size = 17;
int op1_size = 5; int op2_size = 5;

int cmd_count = 19;
string cmd_list[] = {"empty", "add", "sub", "and", "or", "xor", "not",
                     "shl", "shr", "ireg", "imem", "rtr", "mtm", "rtm",
                     "mtr", "jez", "jgz", "jlz", "jp"};

using namespace std;

//const int data_addr_begin = // ������ ����� � �� ��� ������ ������
//const int prog_data_begin = // ������ ����� � �� ��� ������ ���������

/** ------------------ *
    ��������� �������
  * ------------------ */

// ������� ��� ��������� ���������� �� ����� � ��������/���������
string get_info_from_file();

// ������� ��� ���������� ���������� � ���� � ��������/���������
void save_info_in_file(string info);

// ������� ��� �������� ����������� ��������� ����������
void load_saved_state();

// ������� ��� ����������� ���������� ���� ������ � �����������
void show_menu();

// ������� ��� ��������� ������ ����������
void config_output_debug();

// ������� ������ �����������
void architecture_work();

// ������� ���������� ��������� �����������
void save_current_state();

// ������� ��� �������� ������������ ���� � �������� ��� � �������� ���
string parse_program(string program);

// ������� ��� �������� ��������� (� �������� ����) � ������
void load_program_to_memory(string program);

// ������� ��� �������� ������ � ������
void load_data_to_memory(string data);

// ������� ��� ���������� �������
void output_debug(char settings);

char output_debug_settings = 0xFF;

/** ----------------- *
    �������� �������
  * ----------------- */

int main() {
    HWND console = GetConsoleWindow();
    RECT ConsoleRect;
    GetWindowRect(console, &ConsoleRect);
    MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 1280, 720, TRUE);
    setlocale(LC_ALL, "Russian"); // ������� ����

    cout << "���������� �������� ������ ������������� ����." << endl << endl;
    load_saved_state();
    show_menu(); // ����������� ����
    cout << "\n\n������ �������� ������ ����������...\n\n";
    architecture_work(); // �������� ������ ����������
    cout << "\n������ � ����������� ���������.\n";
    save_current_state();
}

/** ----------------------------------------- *
    ����������� ������� ��� ������ � �������
  * ----------------------------------------- */

// ������� ��� ��������� ���������� �� ����� � ��������/���������
string get_info_from_file() {

    bool file_flag = false;
    string path, text, line;

    while (!file_flag) {
        cout << endl << "������� ���� � ����� (������ ��� ������������ ���������� ���������): ";
        cin >> path;
        ifstream file(path);
        if (file.is_open()) {
            while (getline(file, line))
                text += line + "\n";
            file.close();
            file_flag = true;
        }
        else {
            cout << "������������ ����. ���������� ��� ���,\n";
        }
    }
    return text;
}

// ������� ��� ���������� ���������� � ���� � ��������/���������
void save_info_in_file(string info) {

    bool file_flag = false;
    string path, text, line;

    while (!file_flag) {
        cout << "������� ���� � ����� (������ ��� ������������ ���������� ���������): ";
        cin >> path;
        ofstream file(path);
        file << info;
        file.close();
        file_flag = true;
    }
}

/** -------------------------------------------------------- *
    ����������� ������� ��� ������ � ��������� � �����������
  * -------------------------------------------------------- */

// ������� ��� �������� ������������ ���� � �������� ��� � �������� ���
string parse_program(string program) {
    regex label("^[A-Za-z0-9]+:(.*)");
    regex addr2_regex("^(add|sub|and|or|xor|shl|shr|rtr|mtm|rtm|mtr){1}(\\s)+([0-9])+,(\\s)*([0-9])+$");
    regex addr1_regex("^(not|jez|jgz|jlz|jp){1}(\\s)+([0-9])+$");
    regex literal_regex("^(ireg|imem){1}(\\s)+[0-9]+,(\\s)*([0-9])+$");
    regex cmd_regex("[^\\n]+");

    string binary_program = "";
    string cmdtype, literal, op1, op2;
    string space = " ";

    auto program_begin = sregex_iterator(program.begin(), program.end(), cmd_regex);
    auto program_end = sregex_iterator();

    cout << program << endl;


    bool label_match, addr1_match, addr2_match, literal_match;
    for (auto i = program_begin; i != program_end; ++i) {
        string cmd = (*i).str();
        cout << "CURRENT CMD: " << cmd << endl;
        label_match = regex_match(cmd, label);
        addr2_match = regex_match(cmd, addr2_regex);
        addr1_match = regex_match(cmd, addr1_regex);
        literal_match = regex_match(cmd, literal_regex);
        cout << "AFTER REGEX: " << addr2_match << addr1_match << literal_match << endl;

        if (!(addr1_match || addr2_match || literal_match)) {
            cout << endl << "error" << endl;
            return "ERROR";
        }

        // ������� ���� �������
        cmdtype = cmd.substr(0, cmd.find(space));
        for (int j = 0; j < cmd_count; j++) {
            transform(cmdtype.begin(), cmdtype.end(), cmdtype.begin(), ::tolower);
            if (cmdtype.compare(cmd_list[j]) == 0) {
                cmdtype = get_binary(j, cmdtype_size);
                break;
            }
        }
        cmd.erase(0, cmd.find(space) + 1);

        // ������� 1-�� ������
        op1 = get_binary(stoi(cmd.substr(0, cmd.find(space))), op1_size);
        cmd.erase(0, cmd.find(space) + 1);

        // ������� 2-�� ������
        if (addr2_match)
            op2 = get_binary(stoi(cmd.substr(0, cmd.find(space))), op2_size);
        else
            op2 = get_binary(0, op2_size);

        // ������� ��������
        if (literal_match)
            literal = get_binary(stoi(cmd.substr(0, cmd.find(space))), literal_size);
        else
            literal = get_binary(0, literal_size);

        binary_program += cmdtype + literal + op1 + op2 + "\n";
        cout << "BINARY: " << cmdtype << " ";
        cout << literal << " " << op1 << " " << op2 << endl << endl;
        getch();
    }

    return binary_program;
};

// ������� ��� �������� ��������� (� �������� ����) � ������
void load_program_to_memory(string program) {
    if (program == "ERROR") {
        cout << endl << program << endl;
    }
    else {
        regex cmd_regex("[^\\n]+");
        auto program_begin = sregex_iterator(program.begin(), program.end(), cmd_regex);
        auto program_end = sregex_iterator();
        int cmem_pointer = 0; // ?
        for (auto i = program_begin; i != program_end; ++i) {
            cmem[cmem_pointer] = stoi((*i).str(), 0, 2);
            cmem_pointer++;
        }
        cout << "���������� ���������:" << endl << program;
    }
        /*cout << "��������� ������ " << "n" << " �����." << endl;
    cout << "��������� �����: " << "" << endl;
    cout << " �������� �����: " << "" << endl;
    if (false) {
        cout << "��������� �� ���������� � ������, ������� � ����. ������ ������." << endl;
        cout << "���������� ��������:";
        cout << "1. �������� ��������� � 0-�� ������ ������ ������.";
        cout << "2. ���������� ������ (��������������� ������� ��������� � 0-�� ������).";
        cout << "3. ������ ������";
        cout << "�������� ��������: ";
        while(key_flag) {}
    }*/
};

/** -------------------------------------------------------- *
    ����������� ������� ��� ������ � ������� ������
  * -------------------------------------------------------- */

// ������� ��� �������� ������ � ������
void load_data_to_memory(string data) {
    string colon = ":";
    regex line_regex("[^\\n]+");

    auto file_begin = sregex_iterator(data.begin(), data.end(), line_regex);
    auto file_end = sregex_iterator();
    for (auto i = file_begin; i != file_end; ++i) {
        string data = (*i).str();
        string addr = data.substr(0, data.find(colon));
        data.erase(0, data.find(colon) + 1);
        dmem[stoi(addr)] = stoi(data);
    }
};

/** ------------------------------------------------------ *
    ����������� ������� ��� ��������� � ������ ����������
  * ------------------------------------------------------ */

// ������� ��� ����������� ���������� ���� ������ � ������������
void show_menu() {
    bool start_processor = false; // ���� ������� ������ ����������

    // ���� ��������� �������
    // ���� ������ �������

    // ������ ���������
    cout << "0. ��������� ���������� �������." << endl;
    cout << "1. ��������� ��������� �� ����� (�������� ���)." << endl;
    cout << "2. ��������� ��������� �� ����� (Assembler)." << endl;
    cout << "3. ��������� ������ ��� ��." << endl;
    cout << "4. ������� ������ ������." << endl;
    cout << "5. ������� ������ ������." << endl;
    cout << "6. ������ ������ ����������." << endl;

    cout << endl << "�������� ��������: ";
    int key; //  ��� ������� ������
    while (!start_processor) {
        switch(key = getch()) {
            case KEY_0: { // ��������� ���������� �������
                config_output_debug();
                cout << endl << "�������� ��������: ";
                break;
            }

            case KEY_1: { // ��������� ��������� �� ����� (�������� ���)
                load_program_to_memory(get_info_from_file());
                cout << endl << "�������� ��������: ";
                break;
            }

            case KEY_2: { // ��������� ��������� �� ����� (Assembler)
                load_program_to_memory(parse_program(get_info_from_file()));
                cout << endl << "�������� ��������: ";
                break;
            }

            case KEY_3: { // ��������� ������ ��� ��
                string data = get_info_from_file();
                load_data_to_memory(data);
                cout << endl << "�������� ��������: ";
                break;
            }

            case KEY_4: { // ������� ������ ������
                output_debug(2);
                cout << endl << "�������� ��������: ";
                break;
            }

            case KEY_5: { // ������� ������ ������
                output_debug(4);
                cout << endl << "�������� ��������: ";
                break;
            }

            case KEY_6: { // ������ ������ ����������
                start_processor = true;
                break;
            }

            /*case KEY_7: {
                start_processor = true;
                break;
            }
            default: break;*/
        }
    }
}

// ������� ��� ��������� ������ � ������������
void config_output_debug() {
    string settings;

    cout << endl << "��������� ���������� �������." << endl;
    cout << "������� ���������: ";
    print_binary(output_debug_settings, 8); cout << endl;
    cout << "xx1 - ����� ��������� ���" << endl;
    cout << "x1x - ����� ������ ������" << endl;
    cout << "1xx - ����� ������ ������" << endl;
    cout << "������� ����� � �������� ����: ";

    cin >> settings;
    output_debug_settings = (char) stoi(settings, 0, 2);
}

// ������� ������ �����������
void architecture_work() {

    // ��������� ����. �������� ���� ������� � ��������
    int cmdtype_max = 1; for (int i = 0; i < cmdtype_size; i++) cmdtype_max *= 2;
    int literal_max = 1; for (int i = 0; i < literal_size; i++) literal_max *= 2;
    int op1_max = 1; for (int i = 0; i < op1_size; i++) op1_max *= 2;
    int op2_max = 1; for (int i = 0; i < op2_size; i++) op2_max *= 2;

    bool  prev_flag = false;  // ���� �������� � ���������� �������
    bool  next_flag = false;  // ���� �������� � ��������� �������
    bool debug_flag = false;  // ���� �������� � ����������
    bool   end_flag = false;  // ���� �������� � ���������� ������

    int cmd; //����� ������
    int pc = 0; //������� ������

    //  �������� �������
    int cmdtype, literal, op1, op2;

    int key = 0;
    while(!end_flag) {
        cout << "*";
        for (int i = 0; i < 150; i++) cout << "-";
        cout << "*" << endl << endl;

        // �������� ������� �� ������ �� �������� ������
        cmd = cmem[pc];

        // ���� ������ � ��������� "�" � ������� ������
        // �������� ��� ��������, ������� � ��� �������
        // (��� "0" - ������ �������)
            op2 = (cmd & (op1_max-1));
            op1 = (cmd >> op1_size) & (op2_max-1);
        literal = (cmd >> (op1_size + op2_size)) & (literal_max-1);
        cmdtype = (cmd >> (op1_size + op2_size + literal_size));

        // �������������� � ���������� �������� (1-8)
        if (cmdtype >= ADD && cmdtype <= SHR) {
            switch (cmdtype) {
                case ADD : reg[op1]  = reg[op1]  +  reg[op2]; break; // ��������
                case SUB : reg[op1]  = reg[op1]  -  reg[op2]; break; // ���������
                case AND : reg[op1]  = reg[op1] and reg[op2]; break; // ��������� "�"
                case OR  : reg[op1]  = reg[op1] or  reg[op2]; break; // ��������� "���"
                case XOR : reg[op1]  = reg[op1] xor reg[op2]; break; // ��������� "�������� �� ������ 2"
                case NOT : reg[op1]  = not reg[op1];          break; // ���������
                case SHL : reg[op1]  = reg[op1] << reg[op2];  break; // ����� �� op2 �������� �����
                case SHR : reg[op1]  = reg[op1] >> reg[op2];  break; // ����� �� op1 �������� ������
                 default : break;
            }

            // ����������� ������ ����� ��������
            // TODO: �������� ������������
            if (reg[op1] == 0) reg[FLAG] |= EZ;
            if (reg[op1]  > 0) reg[FLAG] |= GZ;
            if (reg[op1]  < 0) reg[FLAG] |= EZ;
        }

        // ������ �������� (9-18)
        else {
            switch (cmdtype) {
                // �������������� ����� ����������/�������� ������ ������
                case IREG :  reg[op1] =   literal; break; // ����������� ����������������� �������� � �������
                case IMEM : dmem[op1] =   literal; break; // ����������� ����������������� �������� � ������
                case RTR  :  reg[op1] =  reg[op2]; break; // ����������� � ������ �������� �� ������ �������
                case MTM  : dmem[op1] = dmem[op2]; break; // ����������� � ����� ������ ������ � ������ ������ ������
                case RTM  :  reg[op1] = dmem[op2]; break; // ����������� � �������� � ������ ������
                case MTR  : dmem[op1] =  reg[op2]; break; // ����������� �� ������ ������ � �������

                // �������� ��������� �������� (���� �����������, �� ������� ������ ���������
                // �������� ��������, � ��������� ������ ������� ������ ����������������)
                case JEZ : pc = (reg[FLAG] & EZ) ? literal : pc + 1; break; // �������� �� reg[op1] >  0
                case JGZ : pc = (reg[FLAG] & GZ) ? literal : pc + 1; break; // �������� �� reg[op1] <  0
                case JLZ : pc = (reg[FLAG] & EZ) ? literal : pc + 1; break; // �������� �� reg[op1] == 0
                case JP  : pc = literal;                             break; // ����������� �������

                default : break;
            }
        }

        cout <<  "������� ������: ";
        print_binary(pc, cmem_addr_size);
        cout << endl << "������� �������:" << endl;
        cout << "��� ������� | �������\t\t| 1-�� ����� | 2-�� ����� |" << endl;
        print_binary(cmdtype, cmem_addr_size);  cout << "\t    | ";
        print_binary(literal, literal_size);    cout << " | ";
        print_binary(op1, op1_size);            cout << "      | ";
        print_binary(op2, op2_size);            cout << "       |" << endl;

        output_debug(output_debug_settings);

        cout << endl;
        //cout << "\"������� �����\" - ���������� �������, ";
        cout << "\"������� ����\" - ��������� �������, ";
        cout << "\"O\" - ���������, ";
        cout << "\"Q\" - ��������� ������ ����������";
        while (!next_flag && !end_flag) {
            switch(key = getch()) {
                // ���������� �������?
                // case KEY_UP   :  prev_flag = true; break;
                case KEY_DOWN :  next_flag = true; break;
                case KEY_O    : debug_flag = true; break;
                case KEY_Q    :   end_flag = true; break;
                default: break;
            }

            if (debug_flag) {
                config_output_debug();
                debug_flag = false;
            }
        }

        if (!end_flag) {
            key = 0;
            next_flag = false;
            debug_flag = false;
            pc = pc + 1;
            cout << '\r';
            // ����� ������� ������ ��� ����������
            // ���������� ������ � ������ ������
            if (pc > cmem_size) pc = 0;
        }
    }
}

// ������� ��� ���������� �������
void output_debug(char settings) {

    if (settings & 1) {
        cout << endl << "��������:" << endl;
        cout << "�������� �������� ������: ";
        print_binary(reg[FLAG], reg_data_size); cout << endl;
        print_mem(reg, reg_size, reg_addr_size, reg_data_size, 4);
}

    if (settings & 2) {
        cout << endl << "������ ������:" << endl;
        print_mem(cmem, cmem_size, cmem_addr_size, cmem_data_size, 4);
    }

    if (settings & 4) {
        cout << endl << "������ ������:" << endl;
        print_mem(dmem, dmem_size, dmem_addr_size, dmem_data_size, 4);
    }
}

/** -------------------------------------------------------------- *
    ����������� ������� ��� ���������� � �������� ��������� ������
  * -------------------------------------------------------------- */

// ������� ��� �������� ����������� ��������� ������
void load_saved_state() {
    bool accept_flag, key_flag;
    int key;

    cout << "�� ������ ��������� ��������� ������? (Y/N) ";
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
        cout << endl << "�������� ����� ����������� ��������� ������ ������ � ������ ������..." << endl;
        string saved_prog_memory, saved_data_memory;
        string colon = ":";
        regex line_regex("[^\\n]+");

        cout << "1. ���� ������ ������";
        saved_prog_memory = get_info_from_file();
        auto file_begin = sregex_iterator(saved_prog_memory.begin(), saved_prog_memory.end(), line_regex);
        auto file_end = sregex_iterator();
        for (auto i = file_begin; i != file_end; ++i) {
            string cmd = (*i).str();
            string addr = cmd.substr(0, cmd.find(colon));
            cmd.erase(0, cmd.find(colon) + 1);
            cmem[stoi(addr)] = stoi(cmd);
        }

        cout << "2. ���� ������ ������";
        saved_data_memory = get_info_from_file();
        file_begin = sregex_iterator(saved_data_memory.begin(), saved_data_memory.end(), line_regex);
        file_end = sregex_iterator();
        for (auto i = file_begin; i != file_end; ++i) {
            string data = (*i).str();
            string addr = data.substr(0, data.find(colon));
            data.erase(0, data.find(colon) + 1);
            dmem[stoi(addr)] = stoi(data);
        }

        cout << "���������� ��������� ������ �������������.";
    }
    cout << endl << endl;
}

// ������� ���������� ��������� ������
void save_current_state() {
    bool accept_flag, key_flag;
    int key;

    // ������ � ������������� ���������� ���������
    cout << "�� ������ ��������� ��������� ������? (Y/N) ";
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

    // ���� ���� ������������� ���������� ���������
    if (accept_flag) {
        string memory_info = "";
        for (int i = 0; i < cmem_size; i++)
            memory_info += to_string(i) + ":" + to_string(cmem[i]) + "\n";

        cout << endl << "���������� ������ ������." << endl;
        save_info_in_file(memory_info);

        memory_info = "";
        for (int i = 0; i < dmem_size; i++)
            memory_info += to_string(i) + ":" + to_string(dmem[i]) + "\n";

        cout << endl << "���������� ������ ������." << endl;
        save_info_in_file(memory_info);
    }
}





































/*// ���������� ������� ������ ������
unsigned short int compute_addr_size(unsigned short int mem_size) {
    int addr_size = 0;
    while(mem_size != 1) {mem_size >>= 1; addr_size++;}
    return addr_size;
}*/


  //   if(_getch()==32) system("Pause"); //��������� ��������� �� ����� �������� � ��������� ������� �������
  //    if(_getch()==83) system("cls"); //������� ������ ��� ������� ������� del
  //cmd = cmem[pc];
  //op1=(cmd>>4)&15 //���������� �������� ������ ���������� ����������� � �������� 7-4
  //op2=cmd&15 //���������� �������� ������ ���������� ����������� � �������� 3-0*/
                // �������� �����������
//TODO : template
