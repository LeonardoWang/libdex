#include "inst.h"
#include "inst_enum.h"

#include "reader.h"

#include <cstdio>

int Inst::length() const
{
    if (op() == 0) {
        if (bytes[1] == 0) {  // nop
            return 2;

        } else if (bytes[1] == 1) {  // packed-switch-payload
            uint32_t size = (bytes[3] << 8) + bytes[2];
            return 8 + 4 * size;

        } else if (bytes[1] == 2) {  // sparse-switch-payload
            uint32_t size = (bytes[3] << 8) + bytes[2];
            return 4 + 8 * size;

        } else if (bytes[1] == 3) {  // fill-array-data-payload
            uint32_t w = (bytes[3] << 8) + bytes[2];
            uint32_t n = (bytes[7] << 24) + (bytes[6] << 16) + (bytes[5] << 8) + bytes[4];
            int len = 8 + w * n;
            if (len % 2 == 1) ++len;  // padding
            return len;

        }

        fprintf(stderr, "Unexpected NOP type %02x\n", bytes[1]);
        return 2;
    }

    return inst_types[op()].get->length();
}

bool Inst::is_const() const
{
    return (Const_4 <= op() && op() <= ConstClass);
}

bool Inst::is_const_string() const
{
    return (op() == ConstString || op() == ConstString_jumbo);
}

//bool Inst::is_assign() const
//{
//    if (Move <= op && op <= MoveException) return true;
//    if (InstanceOf <= op && op <= FilledNewArray_range) return true;
//}

bool Inst::is_invoke() const
{
    if (InvokeVirtual <= op() && op() <= InvokeInterface) return true;
    if (InvokeVirtual_range <= op() && op() <= InvokeInterface_range) return true;
    if (op() == InvokePolymorphic || op() == InvokePolymorphic_range) return true;
    //if (op() == InvokeCustom || op() == InvokeCustom_range) return true;
    return false;
}

bool Inst::is_return() const
{
    return (ReturnVoid <= op() && op() <= ReturnObject);
}

bool Inst::is_throw() const
{
    return (op() == Throw);
}

bool Inst::is_goto() const
{
    return (Goto <= op() && op() <= Goto_32);
}

bool Inst::is_branch() const
{
    return (IfEq <= op() && op() <= IfLez);
}

bool Inst::is_switch() const
{
    return (op() == PackedSwitch || op() == SparseSwitch);
}

Inst Inst::branch_target() const
{
    if (IfEq <= op() && op() <= IfLe)
        return Inst(bytes + (int64_t) get_c() * 2);
    else if (IfEqz <= op() && op() <= IfLez)
        return Inst(bytes + (int64_t) get_b() * 2);
    fputs("Call branch_target() on non-branch instruction\n", stderr);
    return Inst(nullptr);
}

namespace {
uint16_t read_uint16(const uint8_t * data) { return data[0] + (data[1] << 8); }
uint32_t read_uint32(const uint8_t * data) { return data[0] + (data[1] << 8) + (data[2] << 16) + (data[3] << 24); }
int32_t read_int32(const uint8_t * data) { return (int32_t) read_uint32(data); }
}

vector<Inst> Inst::switch_targets() const
{
    vector<Inst> ret;
    const uint8_t * payload = bytes + get_b() * 2;

    if (op() == PackedSwitch) {
        if (payload[0] != 0 || payload[1] != 1) {
            fputs("Bad target of packed-switch\n", stderr);
            return ret;
        }

        int size = read_uint16(payload + 2);
        for (int i = 0; i < size; ++i) {
            int offset = read_int32(payload + 8 + i * 4);
            ret.emplace_back(bytes + offset * 2);
        }
        return ret;

    } else if (op() == SparseSwitch) {
        if (payload[0] != 0 || payload[1] != 2) {
            fputs("Bad target of sparse-switch\n", stderr);
            return ret;
        }

        int size = read_uint16(payload + 2);
        const uint8_t * targets = payload + 4 + size * 4;
        for (int i = 0; i < size; ++i) {
            int offset = read_int32(targets + i * 4);
            ret.emplace_back(bytes + offset * 2);
        }
        return ret;

    } else {
        fputs("Call switch_taget() on non-switch instruction\n", stderr);
        return vector<Inst>();
    }
}

bool Inst::is_libradar_invoke() const
{
    return (InvokeVirtual <= op() && op() <= InvokeInterface);
}
