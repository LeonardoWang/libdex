#include "inst.h"
#include "instformat.h"
#include "dex.h"
#include "inst_enum.h"

#include <cassert>

static inline uint64_t get_val(const uint8_t * bytes, int offset, int len)
{
    if (len == InstFormat::HighHalfByte)
        return bytes[offset] >> 4;

    if (len == InstFormat::LowHalfByte)
        return bytes[offset] & 0x0f;

    uint64_t ret = 0;
    for (int i = offset + len - 1; i >= offset; --i)
        ret = (ret << 8) + bytes[i];
    return ret;
}

Inst::Inst(const uint8_t * bytes)
{
    op = bytes[0];

    if (op == 0) {
        if (bytes[1] == 0) {  // nop
            len = 2;
            return;

        } else if (bytes[1] == 1) {  // packed-switch-payload
            uint32_t size = (bytes[3] << 8) + bytes[2];
            len = 8 + 4 * size;
            return;

        } else if (bytes[1] == 2) {  // sparse-switch-payload
            uint32_t size = (bytes[3] << 8) + bytes[2];
            len = 4 + 8 * size;
            return;

        } else if (bytes[1] == 3) {  // fill-array-data-payload
            uint32_t w = (bytes[3] << 8) + bytes[2];
            uint32_t n = (bytes[7] << 24) + (bytes[6] << 16) + (bytes[5] << 8) + bytes[4];
            len = 8 + w * n;
            if (len % 2 == 1) ++len;  // padding
            return;

        }

        assert(false);
        return;
    }

    const InstFormat & fmt = InstFormat::list[op];
    len = fmt.len;

    a = get_val(bytes, fmt.val_offset[0], fmt.val_len[0]);
    b = get_val(bytes, fmt.val_offset[1], fmt.val_len[1]);
    c = get_val(bytes, fmt.val_offset[2], fmt.val_len[2]);
    d = get_val(bytes, fmt.val_offset[3], fmt.val_len[3]);
    e = get_val(bytes, fmt.val_offset[4], fmt.val_len[4]);
    f = get_val(bytes, fmt.val_offset[5], fmt.val_len[5]);
    g = get_val(bytes, fmt.val_offset[6], fmt.val_len[6]);
    h = get_val(bytes, fmt.val_offset[7], fmt.val_len[7]);
}

bool Inst::is_const() const
{
    return (Const_4 <= op && op <= ConstClass);
}

bool Inst::is_const_string() const
{
    return (op == ConstString || op == ConstString_jumbo);
}

//bool Inst::is_assign() const
//{
//    if (Move <= op && op <= MoveException) return true;
//    if (InstanceOf <= op && op <= FilledNewArray_range) return true;
//}

bool Inst::is_invoke() const
{
    if (InvokeVirtual <= op && op <= InvokeInterface) return true;
    if (InvokeVirtual_range <= op && op <= InvokeInterface_range) return true;
    if (op == InvokePolymorphic || op == InvokePolymorphic_range) return true;
    //if (op == InvokeCustom || op == InvokeCustom_range) return true;
    return false;
}

vector<Inst> Inst::load_method(const EncodedMethod & method)
{
    vector<Inst> ret;

    unsigned off = 0;
    while (off < method.inst_size * 2) {
        ret.emplace_back(method.insts + off);
        off += ret.back().len;

        assert(ret.back().len > 0);
    }

    return ret;
}
