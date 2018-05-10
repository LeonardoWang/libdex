#pragma once

/** @file
 *  Deal with instructions.
 **/

#include "insttype.h"

#include <cstdint>
#include <vector>

using std::vector;

class EncodedMethod;

/** 
 *  Representing one instruction
 *
 *  Note that an instruction is identified by its position rather than its data.
 *  Thus two instructions with same op and args are not guaranteed to be "equal".
 *  All comparison operators over two instructions are based on their address.
 *
 *  This is a extremely "light" structure and feel free to copy instructions around.
 **/
struct Inst {
    /** Pointer to instruction's data in memory **/
    const uint8_t * bytes;

    /** The op code of this instruction **/
    int op() const { return bytes[0]; }
    /** Length of instruction in bytes **/
    ssize_t length() const;

    uint64_t get_a() const { return inst_types[op()].get->a(bytes); }   ///< Get oprand A
    uint64_t get_b() const { return inst_types[op()].get->b(bytes); }   ///< Get oprand B
    uint64_t get_c() const { return inst_types[op()].get->c(bytes); }   ///< Get oprand C
    uint64_t get_d() const { return inst_types[op()].get->d(bytes); }   ///< Get oprand D
    uint64_t get_e() const { return inst_types[op()].get->e(bytes); }   ///< Get oprand E
    uint64_t get_f() const { return inst_types[op()].get->f(bytes); }   ///< Get oprand F
    uint64_t get_g() const { return inst_types[op()].get->g(bytes); }   ///< Get oprand G
    uint64_t get_h() const { return inst_types[op()].get->h(bytes); }   ///< Get oprand H

    /** Load a instruction from given address **/
    Inst(const uint8_t * bytes) : bytes(bytes) { }

    /**
     *  Get following instrution
     *  This function does not check for boundary.
     **/
    Inst next() const { return Inst(bytes + length()); }

    /** Check if this instruction is loading a const number **/
    bool is_const() const;
    /** Check if this instruction is loading a const string **/
    bool is_const_string() const;
    /** Check if this instruction is invoking a function **/
    bool is_invoke() const;
    bool is_read_field() const;
    /** Check if this is a `return` instruction **/
    bool is_return() const;
    /** Check if this is a `throw` instruction **/
    bool is_throw() const;
    /**
     *  Check if this is a unconditional jump
     *  Use `goto_target()` to get target address.
     **/
    bool is_goto() const;
    /**
     *  Check if this is a conditional jump
     *  Use `branch_target()` to get target address.
     **/
    bool is_branch() const;
    /**
     *  Check if this is a `switch` instruction
     *  Use `switch_targets()` to get target address.
     **/
    bool is_switch() const;

    int string_idx() const { return get_b(); }
    /** Return the index of invoked method for a invoking instruction **/
    int invoke_target() const { return get_b(); }
    int field() const;
    /** Return the target address of a unconditional jump instruction **/
    Inst goto_target() const { return Inst(bytes + (int64_t) get_a() * 2); }
    /** Return the target address of a conditional jump instruction **/
    Inst branch_target() const;
    /** 
     *  Return all targets of a `switch` instruction
     *
     *  This function will check for boundaries and only returns targets lay in the containing method.
     *  Theoretically any jump instruction including `switch` can only target instructions in same method,
     *  however there exists malformated DEX files which do not obey this rule.
     *  
     *  @param  l   begin (first instruction) of the containing method
     *  @param  r   end (next of last instruction) of the containing method
     **/
    vector<Inst> switch_targets(Inst l, Inst r) const;

    /** LibRadar cannot detect ranged and polymorphic invocations **/
    bool is_libradar_invoke() const;

    bool operator!= (Inst i) const { return bytes != i.bytes; }
    bool operator<  (Inst i) const { return bytes <  i.bytes; }
    bool operator<= (Inst i) const { return bytes <= i.bytes; }
    bool operator>= (Inst i) const { return bytes >= i.bytes; }
};

/**
 *  An helper struct to use range-based for loops
 *  You should never need to used this directly.
 **/
struct InstIter {
    InstIter(const uint8_t * bytes) : bytes(bytes) { }
    const uint8_t * bytes;
    Inst operator * () const { return Inst(bytes); }
    InstIter & operator ++ () { bytes += Inst(bytes).length(); return * this; }
    InstIter operator ++ (int) { return InstIter { bytes + Inst(bytes).length() }; }
    bool operator == (InstIter it) { return bytes == it.bytes; }
    bool operator != (InstIter it) { return bytes < it.bytes; }
};
