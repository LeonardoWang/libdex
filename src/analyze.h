#pragma once

#include "inst.h"
#include <vector>

using std::vector;

class EncodedMethod;

struct BasicBlock {
    const uint8_t * begin_ptr;
    const uint8_t * end_ptr;

    BasicBlock(Inst begin_i, Inst end_i)
        : begin_ptr(begin_i.bytes), end_ptr(end_i.bytes)
    { }

    InstIter begin() const { return InstIter(begin_ptr); }
    InstIter end() const { return InstIter(end_ptr); }
};

extern vector<BasicBlock> split_basic_blocks(const EncodedMethod & method);
