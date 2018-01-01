#pragma once

#include <string>

/** Format of each instruction **/
struct InstFormat {
    /** Length of this instruction in byte **/
    int len = 0;
    /** Byte offset of each operand value; 0 if not exist **/
    int val_offset[8];
    /** Length of each operand value in byte; `HighHalfByte` or `LowHalfByte` if less than 1 byte **/
    int val_len[8];

    /** Mnemonic of opcode **/
    std::string mnemonic;
    /** Meanings of operand values **/
    std::string syntax;

    static const int HighHalfByte = -1;
    static const int LowHalfByte = -2;

    /** Number of all instruction types (including reserved ones) **/
    static const int op_num = 0x100;

    /** Opcode-to-format table **/
    static InstFormat list[op_num];

    /** Load instruction formats from config file **/
    static void load(const char * inst_list_file);

private:
    void load_fmt(const std::string & fmt);
};
