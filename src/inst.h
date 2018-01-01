#pragma once

#include <cstdint>
#include <vector>

class EncodedMethod;

/** 
 * Representing one instruction
 * Use `InstFormat` to understand opcode.
 **/
struct Inst {
    uint8_t op = 0;
    int len = 0;
    uint64_t a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0;  // operands

    /** Load one instruction from given address **/
    Inst(const uint8_t * bytes);

    /** Check if this instruction is loading a const number **/
    bool is_const() const;
    /** Check if this instruction is loading a const string **/
    bool is_const_string() const;
    /** Check if this instruction is invoking a function **/
    bool is_invoke() const;

    /** Return the index of invoked method for a invoking instruction **/
    int invoke_target() const { return b; }

    /** Load a method; return a vector of its instructions **/
    static std::vector<Inst> load_method(const EncodedMethod & method);


    /** LibRadar cannot detect ranged and polymorphic invocations **/
    bool is_libradar_invoke() const;
};
