#pragma once

/** @file
 *  Control flow analysis related contents.
 **/

#include "inst.h"

class EncodedMethod;

/**
 *  Representing a basic block of instructions
 *
 *  In Java a method will always return to caller's next instruction unless it throws.
 *  Thus we do not treat invoking instructions as exit point of basic blocks,
 *  as long as our project cannot handle inter-method flows.
 *
 *  This class offers `begin()` and `end()` functions in a container's manner and
 *  therefore you can get the instructions with a range-based for loop:
 *  ```
 *      for (Inst inst : basic_block) { ... }
 *  ```
 **/
struct BasicBlock {
    BasicBlock(Inst begin_i, Inst end_i)
        : begin_ptr(begin_i.bytes), end_ptr(end_i.bytes)
    { }

    InstIter begin() const { return InstIter(begin_ptr); }
    InstIter end() const { return InstIter(end_ptr); }

private:
    const uint8_t * begin_ptr;
    const uint8_t * end_ptr;
};

/** Split a method into vector of basic blocks. **/
vector<BasicBlock> split_basic_blocks(const EncodedMethod & method);
