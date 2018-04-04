#include "inst.h"
#include "inst_enum.h"

#include <cassert>

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

        assert(false);
        return -1;
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

bool Inst::is_libradar_invoke() const
{
    return (InvokeVirtual <= op() && op() <= InvokeInterface);
}
