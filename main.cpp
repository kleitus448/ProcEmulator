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
#include <string>       // ���������� ��� ���� "������"
#include <fstream>      // ���������� ��� ������/������ ������
#include <regex>        // ���������� ��� ���������� ���������
#include <algorithm>    // ���������� ��� ������������� ������ � ������ �������
#include <map>          // ���������� ��� ������������� ������������� �������� "Map"
#include <utility>      // ���������� ��� ������������� ���� "Pair"
#include <vector>       // ���������� ��� ������������� ������������ �������� "Vector"
#include <ctype.h>

// ����� �������
#define FLAG 15 // ����� �������� ������
#define EZ 1    // ���� " == 0"
#define GZ 2    // ���� "  > 0"
#define LZ 4    // ���� "  < 0"

// ���� ������ ��� ��������
// ������ �����������
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

// ���� ������
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
unsigned int cmdtype_size = 5,  // ��� �������
             literal_size = 17, // �������
                 op1_size = 5,  // 1-� �����
                 op2_size = 5;  // 2-� �����

int cmd_count = 20; // ���-�� ������
string cmd_list[] = {"empty", "add", "sub", "and", "or", "xor",
                     "not", "shl", "shr", "ireg", "imem", "rtr",
                     "mtm", "rtm", "mtr", "mtra", "jez", "jgz",
                     "jlz", "jp"};

using namespace std;

//const int data_addr_begin = // ������ ����� � �� ��� ������ ������
//const int prog_data_begin = // ������ ����� � �� ��� ������ ���������

/** ------------------ *
    ��������� �������
  * ------------------ */

// ������� ��� ��������� ���������� �� ����� � ��������/���������
vector<string> get_info_from_file();

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
vector<string> parse_program(vector<string> program);

// ������� ��� �������� ��������� (� �������� ����) � ������
void load_program_to_memory(vector<string> program);

// ������� ��� �������� ������ � ������
void load_data_to_memory(vector<string> data);

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
vector<string> get_info_from_file() {

    bool file_flag = false; // ���� ������ � ������
    string path; // ���� � �����
    string line; // ������ �����
    vector<string> text; // ������������� ������ ����� �� �����

    while (!file_flag) {
        cout << endl << "������� ���� � ����� (������ ��� ������������ ���������� ���������): ";
        cin >> path; // ���� ����

        ifstream file(path);
        // ���� ���� ������
        if (file.is_open()) {

            // ������� ���������� ����� �����
            int line_count = 0;
            while (getline(file, line)) line_count++;
            text.resize(line_count);

            // ������ ����� �� ����� � ������
            file.clear(); file.seekg(0);
            for (int i = 0; i < line_count; i++) {
                getline(file, line);
                text[i] = line;
            }

            // �������� �����
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
    string path; // ���� � �����

    while (!file_flag) {
        cout << "������� ���� � ����� (������ ��� ������������ ���������� ���������): ";
        cin >> path; // ���� ����

        // ������ ����� � ����
        ofstream file(path);
        file << info;

        // �������� �����
        file.close();
        file_flag = true;
    }
}

/** ------------------------------------------- *
    ����������� ������� ��� �������� ���������
  * ------------------------------------------- */

// ������� ��������� ������ ��������
vector<string> generate_parse_error(vector<string> program, string error_type, int index) {
     cout << endl << error_type << " � " << index << endl;
     program.resize(1);
     program[0] = "ERROR";
     return program;
}

// ������� ��� ������ ������ �� �������� � ���������
string clear_string_from_spaces(string str) {
    int pointer = 0;
    while (!isalpha(str[pointer])) pointer++;
    if (pointer > 0) str.erase(0, pointer);
    return str;
}

// ������� ��� �������� ������������ ���� � �������� ��� � �������� ���
vector<string> parse_program(vector<string> program) {
    // TODO: �������� ����������� ������ ������� � ����������� � ��������� (���� ����� �������)

    string cmdtype, literal, op1, op2;  // ������ ��� ��������� ������ �������
    string part_sep = " ";              // ����������� ������ �������

    string label_sep = ":";             // ����������� �����
    map <string, int> label_map;        // ������������� ������ �������� ����� <"�����", "����� ������">

    // ���������� ��������� ��� ���� ��������� ����� ������
    regex addr2_regex("^(\\s)*(add|sub|and|or|xor|shl|shr|rtr|mtm|rtm|mtr|mtra){1}(\\s)+([0-9])+,(\\s)*([0-9]$)+$");
    regex addr1_regex("^(\\s)*(not|jez|jgz|jlz|jp){1}(\\s)+([0-9])+$");
    regex condition_mark_regex("^(\\s)*(jez|jgz|jlz|jp){1}(\\s)+([A-Za-z])+$");
    regex literal_regex("^(\\s)*(ireg|imem){1}(\\s)+[0-9]+,(\\s)*([0-9])+$");
    regex empty_regex("^(\\s)*empty(\\s)*$");
    regex cmd_regex("[^\\n]+");
    regex no_mean_regex("(^(//.*)$)|(^[(\\t)(\\s)]*$)");

    // ����� ������������ ������� ���������� ����������
    bool condition_mark_match, addr1_match, addr2_match,
         literal_match, label_match, empty_match, no_mean_match;

    // ���������� ��������� ��� ��������� �����
    regex label_regex("[A-Za-z0-9]+:(.*)");

    /** ----------------------------------------------------- *
        ������ ���� ���������� �� ��������� � ���� �����.
        ���� ��� ���������, ��� ����������� � �������������
        ��������� label_map <"�������� �����", "����� ������">
      * ----------------------------------------------------- */
    unsigned int bin_pointer = 0;
    string cmd;
    for (unsigned int i = 0; i < program.size(); i++) {�S
        cmd = program[i];
        cout << cmd << endl;

        no_mean_match = regex_match(cmd, no_mean_regex);
        if (!no_mean_match) { // ������ ���� �� ������
            cmd = clear_string_from_spaces(cmd);

            // ���� ����� �������, ���������� � ��������� ��
            // ������� � ��������� � ������������� ������
            label_match = regex_match(cmd, label_regex);

            if (label_match) { // ������ ����� �����
                string label = cmd.substr(0, cmd.find(label_sep));
                cmd.erase(0, cmd.find(label_sep) + 1);
                cmd = clear_string_from_spaces(cmd);
                label_map[label] = i;
            }
            program[bin_pointer] = cmd;
            bin_pointer++;
        }
    }
    cout << "������ ���������: " << program.size() << endl;
    cout << "������ �������� ���������: " << bin_pointer << endl;
    program.resize(bin_pointer);

    cout << (label_map.size() > 0 ? "����� �������" : "����� �� �������");
    cout << endl << endl;
    getch();

    /** ------------------------------------------------------------ *
        ������ ���� ���������� �� ��������� � ������ ������ �������,
        �������� � �� ��� �������, ������� � ������/����� �������
      * ------------------------------------------------------------ */
    for (unsigned int i = 0; i < program.size(); i++) {
        string cmd = program[i];
        cout << "CURRENT CMD: " << cmd << endl;
        // ����� ������� �������
        // cout << "CURRENT CMD: " << cmd << endl;

        // ��������� ������������ �������
        // � ����������� ����������
        addr2_match = regex_match(cmd, addr2_regex);
        addr1_match = regex_match(cmd, addr1_regex);
        literal_match = regex_match(cmd, literal_regex);
        condition_mark_match = regex_match(cmd, condition_mark_regex);
        empty_match = regex_match(cmd, empty_regex);
        cout << "AFTER REGEX: " << addr2_match << addr1_match;
        cout << literal_match << condition_mark_match;
        cout << empty_match << endl;

        // ���� ������������ �� �������, ������������ ������
        if (!(addr1_match || addr2_match || literal_match ||
              condition_mark_match || empty_match))
            generate_parse_error(program, "error_match", i);
        else if (empty_match)
            program[i] = get_binary(0, cmdtype_size + literal_size + op1_size + op2_size);
        else {
            /** -------------------- *
                ������� ���� �������
              * -------------------- */
            cmdtype = cmd.substr(0, cmd.find(part_sep));
            cout << "cmd_type: " << cmdtype << endl;

                // ������� �� ���� ������� ������� � ���������, ������� ��� �� �������
                cmdtype.erase(remove_if(cmdtype.begin(), cmdtype.end(), ::isspace), cmdtype.end());
                cmd.erase(0, cmd.find(part_sep) + 1);

                // ������ ������� � ������� �������� (��������� �����) � ����
                // � � ������� ������. ������ ������� � ������� - ��� ���� �������
                transform(cmdtype.begin(), cmdtype.end(), cmdtype.begin(), ::tolower);
                for (int j = 0; j < cmd_count; j++)
                    if (cmdtype.compare(cmd_list[j]) == 0) {
                        cmdtype = get_binary(j, cmdtype_size);
                        break;
                    }

            /** ----------------------------- *
                ������� 1-�� ������ � �������
              * ----------------------------- */
            op1 = cmd.substr(0, cmd.find(part_sep));
            cout << "op1: " << op1 << endl;

                // ���� ������ ������ ����� �����, �� ������������ �����������
                // ������ ������ �� (��� �������� � ����������� ���������)
                if (condition_mark_match) {
                    // ������� �� ����� ������� � ���������
                    op1.erase(remove_if(op1.begin(), op1.end(), ::isspace), op1.end());

                    // �������������� �������� ������� �����
                    map <string, int> :: iterator iter = label_map.begin();

                    // ����������� �� ������� ����� � ���� ���������� �����
                    // � ��������� ����� �� �������
                    for (; iter != label_map.end(); iter++)
                        if (op1.compare(iter->first) == 0) {
                            op1 = get_binary(iter->second, op1_size);
                            break;
                        }

                    // ���� �������� ����� �� �����, ������ �����
                    // �� ���� �������. ����������� ������.
                    if (iter == label_map.end())
                        generate_parse_error(program, "error_condition", i);
                }

                // ���� ������� �� ��������� � �� ��� ��,
                // ������������ ����� � �������� ���
                else op1 = get_binary(stoi(op1), op1_size);

                // ������� ����� �� �������
                cmd.erase(0, cmd.find(part_sep) + 1);

            /** -------------------------------------------- *
                ������� 2-�� ������ � ������� (���� �� ����)
              * -------------------------------------------- */
            if (addr2_match)
                op2 = get_binary(stoi(cmd.substr(0, cmd.find(part_sep))), op2_size);
            else
                op2 = get_binary(0, op2_size);

            /** ----------------------------------------- *
                ������� �������� � ������� (���� �� ����)
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
    ����������� ������� ��� ������ � ������� ������ � ������� ������
  * ---------------------------------------------------------------- */

// ������� ��� �������� ������ � ������
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

// ������� ��� �������� ��������� (� �������� ����) � ������
void load_program_to_memory(vector<string> program) {
    if (program[0] == "ERROR") {
        cout << endl << program[0] << endl;
    }
    else {
        int cmem_pointer = 0; // ?
        cout << "���������� ���������:" << endl;
        for (unsigned int i = 0; i < program.size(); i++) {
            cout << program[i] << endl;
            cmem[cmem_pointer + i] = stoll(program[i], 0, 2);
        }
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
                load_data_to_memory(get_info_from_file());
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
    unsigned int cmdtype_max = 1; for (unsigned int i = 0; i < cmdtype_size; i++) cmdtype_max *= 2;
    unsigned int literal_max = 1; for (unsigned int i = 0; i < literal_size; i++) literal_max *= 2;
    unsigned int op1_max = 1; for (unsigned int i = 0; i < op1_size; i++) op1_max *= 2;
    unsigned int op2_max = 1; for (unsigned int i = 0; i < op2_size; i++) op2_max *= 2;

    //bool  prev_flag = false;  // ���� �������� � ���������� �������
    bool  next_flag = false;  // ���� �������� � ��������� �������
    bool debug_flag = false;  // ���� �������� � ����������
    bool   end_flag = false;  // ���� �������� � ���������� ������

    unsigned int cmd; //����� ������
    unsigned int pc = 0, old_pc; //������� ������

    //  �������� �������
    unsigned int cmdtype, op1, op2;
    int literal;

    int key = 0;
    while(!end_flag) {
        old_pc = pc;
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
            if (reg[op1] == 0) reg[FLAG] = EZ;
            if (reg[op1]  > 0) reg[FLAG] = GZ;
            if (reg[op1]  < 0) reg[FLAG] = LZ;
        }

        // ������ �������� (9-18)
        else {
            switch (cmdtype) {
                // �������������� ����� ����������/�������� ������ ������
               case IREG :   reg[op1] =   literal;      break; // ����������� ����������������� �������� � �������
                case IMEM : dmem[op1] =   literal;      break; // ����������� ����������������� �������� � ������
                case RTR  :  reg[op1] =  reg[op2];      break; // ����������� � ������ �������� �� ������ �������
                case MTM  : dmem[op1] = dmem[op2];      break; // ����������� � ����� ������ ������ � ������ ������ ������
                case RTM  : dmem[op1] = reg[op2];       break; // ����������� � �������� � ������ ������
                case MTR  :  reg[op1] = dmem[op2] ;     break; // ����������� �� ������ ������ (���������������) � �������
                case MTRA :  reg[op1] = dmem[reg[op2]]; break; // ����������� �� ������ ������ (����� � ��������) � �������

                // �������� ��������� �������� (���� �����������, �� ������� ������ ���������
                // �������� ��������, � ��������� ������ ������� ������ ����������������)
                case JEZ : pc = (reg[FLAG] & EZ) ? op1 : pc + 1; break; // �������� �� reg[op1] == 0
                case JGZ : pc = (reg[FLAG] & GZ) ? op1 : pc + 1; break; // �������� �� reg[op1] >  0
                case JLZ : pc = (reg[FLAG] & LZ) ? op1 : pc + 1; break; // �������� �� reg[op1] <  0
                case JP  : pc = op1;                             break; // ����������� �������

                default : break;
            }
        }

        cout <<  "������� ������: ";
        print_binary(old_pc, cmem_addr_size);
        cout << endl << "������� �������:" << endl;
        cout << "��� ������� | �������\t\t| 1-�� ����� | 2-�� ����� |" << endl;
        print_binary(cmdtype, cmem_addr_size);  cout << "\t    | ";
        print_binary(literal, literal_size);    cout << " | ";
        print_binary(op1, op1_size);            cout << "      | ";
        print_binary(op2, op2_size);            cout << "      |" << endl;

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
                cout << "\"������� ����\" - ��������� �������, ";
                cout << "\"O\" - ���������, ";
                cout << "\"Q\" - ��������� ������ ����������";
            }
        }

        if (!end_flag) {
            key = 0;
            next_flag = false;
            debug_flag = false;
            if (!(cmdtype >= JEZ && cmdtype <= JP))
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
        vector<string> saved_prog_memory, saved_data_memory;
        string colon = ":";
        regex line_regex("[^\\n]+");

        cout << "1. ���� ������ ������";
        saved_prog_memory = get_info_from_file();
        for (unsigned int i = 0; i < saved_prog_memory.size(); i++) {
            string cmd = saved_prog_memory[i];
            string addr = cmd.substr(0, cmd.find(colon));
            cmd.erase(0, cmd.find(colon) + 1);
            cmem[stoi(addr)] = stoll(cmd);
        }

        cout << "2. ���� ������ ������";
        load_data_to_memory(get_info_from_file());

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
