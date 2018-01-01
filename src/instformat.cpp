#include "instformat.h"

#include <cassert>
#include <fstream>

using namespace std;

InstFormat InstFormat::list[InstFormat::op_num];

void InstFormat::load_fmt(const string & fmt)
{
    int l[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };
    int r[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };

    for (unsigned i = 0; i < fmt.length(); ++i) {
        if ('A' <= fmt[i] && fmt[i] <= 'H') {
            int v = fmt[i] - 'A';
            if (l[v] == -1) l[v] = i;
            r[v] = i + 1;
        } else
            assert(fmt[i] == '0' || fmt[i] == 'o' || fmt[i] == 'p');
    }

    for (int i = 0; i < 8; ++i) {
        if (l[i] == -1) {
            val_offset[i] = 0;
            val_len[i] = 0;
            continue;
        }

        val_offset[i] = l[i] / 2;
        val_len[i] = (r[i] - l[i]) / 2;

        if (val_len[i] == 0) 
            val_len[i] = (l[i] % 2 == 0) ? HighHalfByte : LowHalfByte;
        else
            assert(l[i] % 2 == 0 && r[i] % 2 == 0);
    }

    len = fmt.length() / 2;
    assert(fmt.length() % 4 == 0);
}

void InstFormat::load(const char * inst_list_file)
{
    ifstream f(inst_list_file);

    string header;
    getline(f, header);

    int opcode;
    string fmt;

    while (f >> hex >> opcode) {
        assert(0 <= opcode && opcode <= op_num);
        f >> list[opcode].mnemonic >> fmt;
        list[opcode].load_fmt(fmt);
        f >> ws;
        if (fmt != "op00") {
            getline(f, list[opcode].syntax);
        }
        //cerr << list[opcode].mnemonic << '|' << fmt << '|' << endl;
    }
}
