#pragma once

#include "insttype.h"

#include <cstdint>
#include <vector>

using std::vector;

class EncodedMethod;

/** 
 * Representing one instruction
 * Use `InstFormat` to understand opcode.
 **/
struct Inst {
    const uint8_t * bytes;

    int op() const { return bytes[0]; }
    int length() const;

    uint64_t get_a() const { return inst_types[op()].get->a(bytes); }
    uint64_t get_b() const { return inst_types[op()].get->b(bytes); }
    uint64_t get_c() const { return inst_types[op()].get->c(bytes); }
    uint64_t get_d() const { return inst_types[op()].get->d(bytes); }
    uint64_t get_e() const { return inst_types[op()].get->e(bytes); }
    uint64_t get_f() const { return inst_types[op()].get->f(bytes); }
    uint64_t get_g() const { return inst_types[op()].get->g(bytes); }
    uint64_t get_h() const { return inst_types[op()].get->h(bytes); }

    /** Load one instruction from given address **/
    Inst(const uint8_t * bytes) : bytes(bytes) { }

    Inst next() const { return Inst(bytes + length()); }

    /** Check if this instruction is loading a const number **/
    bool is_const() const;
    /** Check if this instruction is loading a const string **/
    bool is_const_string() const;
    /** Check if this instruction is invoking a function **/
    bool is_invoke() const;
    bool is_return() const;
    bool is_throw() const;
    bool is_goto() const;
    bool is_branch() const;
    bool is_switch() const;

    /** Return the index of invoked method for a invoking instruction **/
    int invoke_target() const { return get_b(); }
    Inst goto_target() const { return Inst(bytes + (int64_t) get_a() * 2); }
    Inst branch_target() const;
    vector<Inst> switch_targets() const;

    /** LibRadar cannot detect ranged and polymorphic invocations **/
    bool is_libradar_invoke() const;

    bool operator!= (Inst i) const { return bytes != i.bytes; }
    bool operator< (Inst i) const { return bytes < i.bytes; }
};

struct InstIter {
    InstIter(const uint8_t * bytes) : bytes(bytes) { }
    const uint8_t * bytes;
    Inst operator * () const { return Inst(bytes); }
    InstIter & operator ++ () { bytes += Inst(bytes).length(); return * this; }
    InstIter operator ++ (int) { return InstIter { bytes + Inst(bytes).length() }; }
    bool operator == (InstIter it) { return bytes == it.bytes; }
    bool operator != (InstIter it) { return bytes != it.bytes; }
};
