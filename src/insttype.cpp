#include "insttype.h"

namespace {

enum Format {
    op00,
    opAA,
    opBA,
    op00AAAA,
    opAABBBB,
    opAACCBB,
    opBACCCC,
    op00AAAAAAAA,
    op00AAAABBBB,
    opAABBBBBBBB,
    opAABBBBCCCC,
    opAGBBBBDCFE,
    opAABBBBCCCCHHHH,
    opAGBBBBDCFEHHHH,
    opAABBBBBBBBBBBBBBBB,
    _10t,
    _20t,
    _21t,
    _22t,
    _30t,
};

uint64_t _b(const uint8_t * d, int n) { return (uint64_t) d[n] << (n * 8); }
uint64_t read_l(const uint8_t * d) { return d[0] & 0x0f; }
uint64_t read_h(const uint8_t * d) { return d[0] >> 4; }
uint16_t read_2(const uint8_t * d) { return d[0] + _b(d, 1); }
uint32_t read_4(const uint8_t * d) { return d[0] + _b(d, 1) + _b(d, 2) + _b(d, 3); }
uint64_t read_8(const uint8_t * d) { return d[0] + _b(d, 1) + _b(d, 2) + _b(d, 3) + _b(d, 4) + _b(d, 5) + _b(d, 6) + _b(d, 7); }

struct Getter_op00 : public InstType::Getter {
    virtual int length() const override { return 2; }
};

struct Getter_opAA : public InstType::Getter {
    virtual uint64_t a(const uint8_t * data) const override { return data[1]; }
    virtual int length() const override { return 2; }
};

struct Getter_10t : public InstType::Getter {
    virtual uint64_t a(const uint8_t * data) const override { return (int8_t) data[1]; }
    virtual int length() const override { return 2; }
};

struct Getter_opBA : public InstType::Getter {
    virtual uint64_t a(const uint8_t * data) const override { return read_l(data + 1); }
    virtual uint64_t b(const uint8_t * data) const override { return read_h(data + 1); }
    virtual int length() const override { return 2; }
};

struct Getter_op00AAAA : public InstType::Getter {
    virtual uint64_t a(const uint8_t * data) const override { return read_2(data + 2); }
    virtual int length() const override { return 4; }
};

struct Getter_20t : public InstType::Getter {
    virtual uint64_t a(const uint8_t * data) const override { return (int16_t) read_2(data + 2); }
    virtual int length() const override { return 4; }
};

struct Getter_opAABBBB : public InstType::Getter {
    virtual uint64_t a(const uint8_t * data) const override { return data[1]; }
    virtual uint64_t b(const uint8_t * data) const override { return read_2(data + 2); }
    virtual int length() const override { return 4; }
};

struct Getter_21t : public InstType::Getter {
    virtual uint64_t a(const uint8_t * data) const override { return data[1]; }
    virtual uint64_t b(const uint8_t * data) const override { return (int16_t) read_2(data + 2); }
    virtual int length() const override { return 4; }
};

struct Getter_opAACCBB : public InstType::Getter {
    virtual uint64_t a(const uint8_t * data) const override { return data[1]; }
    virtual uint64_t b(const uint8_t * data) const override { return data[3]; }
    virtual uint64_t c(const uint8_t * data) const override { return data[2]; }
    virtual int length() const override { return 4; }
};

struct Getter_opBACCCC : public InstType::Getter {
    virtual uint64_t a(const uint8_t * data) const override { return read_l(data + 1); }
    virtual uint64_t b(const uint8_t * data) const override { return read_h(data + 1); }
    virtual uint64_t c(const uint8_t * data) const override { return read_2(data + 2); }
    virtual int length() const override { return 4; }
};

struct Getter_22t : public InstType::Getter {
    virtual uint64_t a(const uint8_t * data) const override { return read_l(data + 1); }
    virtual uint64_t b(const uint8_t * data) const override { return read_h(data + 1); }
    virtual uint64_t c(const uint8_t * data) const override { return (int16_t) read_2(data + 2); }
    virtual int length() const override { return 4; }
};

struct Getter_op00AAAAAAAA : public InstType::Getter {
    virtual uint64_t a(const uint8_t * data) const override { return read_4(data + 2); }
    virtual int length() const override { return 6; }
};

struct Getter_30t : public InstType::Getter {
    virtual uint64_t a(const uint8_t * data) const override { return (int32_t) read_4(data + 2); }
    virtual int length() const override { return 6; }
};

struct Getter_op00AAAABBBB : public InstType::Getter {
    virtual uint64_t a(const uint8_t * data) const override { return read_2(data + 2); }
    virtual uint64_t b(const uint8_t * data) const override { return read_2(data + 4); }
    virtual int length() const override { return 6; }
};

struct Getter_opAABBBBBBBB : public InstType::Getter {
    virtual uint64_t a(const uint8_t * data) const override { return data[1]; }
    virtual uint64_t b(const uint8_t * data) const override { return read_4(data + 2); }
    virtual int length() const override { return 6; }
};

struct Getter_opAABBBBCCCC : public InstType::Getter {
    virtual uint64_t a(const uint8_t * data) const override { return data[1]; }
    virtual uint64_t b(const uint8_t * data) const override { return read_2(data + 2); }
    virtual uint64_t c(const uint8_t * data) const override { return read_2(data + 4); }
    virtual int length() const override { return 6; }
};

struct Getter_opAGBBBBDCFE : public InstType::Getter {
    virtual uint64_t a(const uint8_t * data) const override { return read_h(data + 1); }
    virtual uint64_t b(const uint8_t * data) const override { return read_2(data + 2); }
    virtual uint64_t c(const uint8_t * data) const override { return read_l(data + 4); }
    virtual uint64_t d(const uint8_t * data) const override { return read_h(data + 4); }
    virtual uint64_t e(const uint8_t * data) const override { return read_l(data + 5); }
    virtual uint64_t f(const uint8_t * data) const override { return read_h(data + 5); }
    virtual uint64_t g(const uint8_t * data) const override { return read_l(data + 1); }
    virtual int length() const override { return 6; }
};

struct Getter_opAABBBBCCCCHHHH : public InstType::Getter {
    virtual uint64_t a(const uint8_t * data) const override { return data[1]; }
    virtual uint64_t b(const uint8_t * data) const override { return read_2(data + 2); }
    virtual uint64_t c(const uint8_t * data) const override { return read_2(data + 4); }
    virtual uint64_t h(const uint8_t * data) const override { return read_2(data + 6); }
    virtual int length() const override { return 8; }
};

struct Getter_opAGBBBBDCFEHHHH : public InstType::Getter {
    virtual uint64_t a(const uint8_t * data) const override { return read_h(data + 1); }
    virtual uint64_t b(const uint8_t * data) const override { return read_2(data + 2); }
    virtual uint64_t c(const uint8_t * data) const override { return read_l(data + 4); }
    virtual uint64_t d(const uint8_t * data) const override { return read_h(data + 4); }
    virtual uint64_t e(const uint8_t * data) const override { return read_l(data + 5); }
    virtual uint64_t f(const uint8_t * data) const override { return read_h(data + 5); }
    virtual uint64_t g(const uint8_t * data) const override { return read_l(data + 1); }
    virtual uint64_t h(const uint8_t * data) const override { return read_2(data + 6); }
    virtual int length() const override { return 8; }
};

struct Getter_opAABBBBBBBBBBBBBBBB : public InstType::Getter {
    virtual uint64_t a(const uint8_t * data) const override { return data[1]; }
    virtual uint64_t b(const uint8_t * data) const override { return read_8(data + 2); }
    virtual int length() const override { return 10; }
};

}

const InstType::Getter * InstType::getters[] = {
    [op00]                  = new Getter_op00(),
    [opAA]                  = new Getter_opAA(),
    [opBA]                  = new Getter_opBA(),
    [op00AAAA]              = new Getter_op00AAAA(),
    [opAABBBB]              = new Getter_opAABBBB(),
    [opAACCBB]              = new Getter_opAACCBB(),
    [opBACCCC]              = new Getter_opBACCCC(),
    [op00AAAAAAAA]          = new Getter_op00AAAAAAAA(),
    [op00AAAABBBB]          = new Getter_op00AAAABBBB(),
    [opAABBBBBBBB]          = new Getter_opAABBBBBBBB(),
    [opAABBBBCCCC]          = new Getter_opAABBBBCCCC(),
    [opAGBBBBDCFE]          = new Getter_opAGBBBBDCFE(),
    [opAABBBBCCCCHHHH]      = new Getter_opAABBBBCCCCHHHH(),
    [opAGBBBBDCFEHHHH]      = new Getter_opAGBBBBDCFEHHHH(),
    [opAABBBBBBBBBBBBBBBB]  = new Getter_opAABBBBBBBBBBBBBBBB(),
    [_10t]                  = new Getter_10t(),
    [_20t]                  = new Getter_20t(),
    [_21t]                  = new Getter_21t(),
    [_22t]                  = new Getter_22t(),
    [_30t]                  = new Getter_30t(),
};

const InstType inst_types[256] = {
    [0x00] = InstType("nop",                        op00                                                            ),
    [0x01] = InstType("move",                       opBA,                   "vA, vB"                                ),
    [0x02] = InstType("move/from16",                opAABBBB,               "vA, vB"                                ),
    [0x03] = InstType("move/16",                    op00AAAABBBB,           "vA, vB"                                ),
    [0x04] = InstType("move_wide",                  opBA,                   "vA, vB"                                ),
    [0x05] = InstType("move_wide/from16",           opAABBBB,               "vA, vB"                                ),
    [0x06] = InstType("move_wide/16",               op00AAAABBBB,           "vA, vB"                                ),
    [0x07] = InstType("move_object",                opBA,                   "vA, vB"                                ),
    [0x08] = InstType("move_object/from16",         opAABBBB,               "vA, vB"                                ),
    [0x09] = InstType("move_object/16",             op00AAAABBBB,           "vA, vB"                                ),
    [0x0a] = InstType("move_result",                opAA,                   "vA"                                    ),
    [0x0b] = InstType("move_result_wide",           opAA,                   "vA"                                    ),
    [0x0c] = InstType("move_result_object",         opAA,                   "vA"                                    ),
    [0x0d] = InstType("move_exception",             opAA,                   "vA"                                    ),
    [0x0e] = InstType("return_void",                op00                                                            ),
    [0x0f] = InstType("return",                     opAA,                   "vA"                                    ),
    [0x10] = InstType("return_wide",                opAA,                   "VA"                                    ),
    [0x11] = InstType("return_object",              opAA,                   "vA"                                    ),
    [0x12] = InstType("const/4",                    opBA,                   "vA, #+B"                               ),
    [0x13] = InstType("const/16",                   opAABBBB,               "vA, #+B"                               ),
    [0x14] = InstType("const",                      opAABBBBBBBB,           "vA, #+B"                               ),
    [0x15] = InstType("const/high16",               opAABBBB,               "vA, #+B0000"                           ),
    [0x16] = InstType("const_wide/16",              opAABBBB,               "vA, #+B"                               ),
    [0x17] = InstType("const_wide/32",              opAABBBBBBBB,           "vA, #+B"                               ),
    [0x18] = InstType("const_wide",                 opAABBBBBBBBBBBBBBBB,   "vA, #+B"                               ),
    [0x19] = InstType("const_wide/high16",          opAABBBB,               "vA, #+B000000000000"                   ),
    [0x1a] = InstType("const_string",               opAABBBB,               "vA, string@B"                          ),
    [0x1b] = InstType("const_string/jumbo",         opAABBBBBBBB,           "vA, string@B"                          ),
    [0x1c] = InstType("const_class",                opAABBBB,               "vA, type@B"                            ),
    [0x1d] = InstType("monitor_enter",              opAA,                   "vA"                                    ),
    [0x1e] = InstType("monitor_exit",               opAA,                   "vA"                                    ),
    [0x1f] = InstType("check_cast",                 opAABBBB,               "vA, type@B"                            ),
    [0x20] = InstType("instance_of",                opBACCCC,               "vA, vB, type@C"                        ),
    [0x21] = InstType("array_length",               opBA,                   "vA, vB"                                ),
    [0x22] = InstType("new_instance",               opAABBBB,               "vA, type@B"                            ),
    [0x23] = InstType("new_array",                  opBACCCC,               "vA, vB, type@C"                        ),
    [0x24] = InstType("filled_new_array",           opAGBBBBDCFE,           "{vC, vD, vE, vF, vG}, type@B"          ),
    [0x25] = InstType("filled_new_array/range",     opAABBBBCCCC,           "{vC .. vN}, type@B"                    ),
    [0x26] = InstType("fill_array_data",            opAABBBBBBBB,           "{vA, +B}"                              ),
    [0x27] = InstType("throw",                      opAA,                   "vA"                                    ),
    [0x28] = InstType("goto",                       _10t,                   "+A"                                    ),
    [0x29] = InstType("goto/16",                    _20t,                   "+A"                                    ),
    [0x2a] = InstType("goto/32",                    _30t,                   "+A"                                    ),
    [0x2b] = InstType("packed_switch",              opAABBBBBBBB,           "vA, +B"                                ),
    [0x2c] = InstType("sparse_switch",              opAABBBBBBBB,           "vA, +B"                                ),
    [0x2d] = InstType("cmpl_float",                 opAACCBB,               "vA, vB, vC"                            ),
    [0x2e] = InstType("cmpg_float",                 opAACCBB,               "vA, vB, vC"                            ),
    [0x2f] = InstType("cmpl_double",                opAACCBB,               "vA, vB, vC"                            ),
    [0x30] = InstType("cmpg_double",                opAACCBB,               "vA, vB, vC"                            ),
    [0x31] = InstType("cmp_long",                   opAACCBB,               "vA, vB, vC"                            ),
    [0x32] = InstType("if_eq",                      _22t,                   "vA, vB, +C"                            ),
    [0x33] = InstType("if_ne",                      _22t,                   "vA, vB, +C"                            ),
    [0x34] = InstType("if_lt",                      _22t,                   "vA, vB, +C"                            ),
    [0x35] = InstType("if_ge",                      _22t,                   "vA, vB, +C"                            ),
    [0x36] = InstType("if_gt",                      _22t,                   "vA, vB, +C"                            ),
    [0x37] = InstType("if_le",                      _22t,                   "vA, vB, +C"                            ),
    [0x38] = InstType("if_eqz",                     _21t,                   "vA, +B"                                ),
    [0x39] = InstType("if_nez",                     _21t,                   "vA, +B"                                ),
    [0x3a] = InstType("if_ltz",                     _21t,                   "vA, +B"                                ),
    [0x3b] = InstType("if_gez",                     _21t,                   "vA, +B"                                ),
    [0x3c] = InstType("if_gtz",                     _21t,                   "vA, +B"                                ),
    [0x3d] = InstType("if_lez",                     _21t,                   "vA, +B"                                ),
    [0x3e] = InstType("UNUSED",                     op00                                                            ),
    [0x3f] = InstType("UNUSED",                     op00                                                            ),
    [0x40] = InstType("UNUSED",                     op00                                                            ),
    [0x41] = InstType("UNUSED",                     op00                                                            ),
    [0x42] = InstType("UNUSED",                     op00                                                            ),
    [0x43] = InstType("UNUSED",                     op00                                                            ),
    [0x44] = InstType("aget",                       opAACCBB,               "vA, vB, vC"                            ),
    [0x45] = InstType("aget_wide",                  opAACCBB,               "vA, vB, vC"                            ),
    [0x46] = InstType("aget_object",                opAACCBB,               "vA, vB, vC"                            ),
    [0x47] = InstType("aget_boolean",               opAACCBB,               "vA, vB, vC"                            ),
    [0x48] = InstType("aget_byte",                  opAACCBB,               "vA, vB, vC"                            ),
    [0x49] = InstType("aget_char",                  opAACCBB,               "vA, vB, vC"                            ),
    [0x4a] = InstType("aget_short",                 opAACCBB,               "vA, vB, vC"                            ),
    [0x4b] = InstType("aput",                       opAACCBB,               "vA, vB, vC"                            ),
    [0x4c] = InstType("aput_wide",                  opAACCBB,               "vA, vB, vC"                            ),
    [0x4d] = InstType("aput_object",                opAACCBB,               "vA, vB, vC"                            ),
    [0x4e] = InstType("aput_boolean",               opAACCBB,               "vA, vB, vC"                            ),
    [0x4f] = InstType("aput_byte",                  opAACCBB,               "vA, vB, vC"                            ),
    [0x50] = InstType("aput_char",                  opAACCBB,               "vA, vB, vC"                            ),
    [0x51] = InstType("aput_short",                 opAACCBB,               "vA, vB, vC"                            ),
    [0x52] = InstType("iget",                       opBACCCC,               "vA, vB, field@C"                       ),
    [0x53] = InstType("iget_wide",                  opBACCCC,               "vA, vB, field@C"                       ),
    [0x54] = InstType("iget_object",                opBACCCC,               "vA, vB, field@C"                       ),
    [0x55] = InstType("iget_boolean",               opBACCCC,               "vA, vB, field@C"                       ),
    [0x56] = InstType("iget_byte",                  opBACCCC,               "vA, vB, field@C"                       ),
    [0x57] = InstType("iget_char",                  opBACCCC,               "vA, vB, field@C"                       ),
    [0x58] = InstType("iget_short",                 opBACCCC,               "vA, vB, field@C"                       ),
    [0x59] = InstType("iput",                       opBACCCC,               "vA, vB, field@C"                       ),
    [0x5a] = InstType("iput_wide",                  opBACCCC,               "vA, vB, field@C"                       ),
    [0x5b] = InstType("iput_object",                opBACCCC,               "vA, vB, field@C"                       ),
    [0x5c] = InstType("iput_boolean",               opBACCCC,               "vA, vB, field@C"                       ),
    [0x5d] = InstType("iput_byte",                  opBACCCC,               "vA, vB, field@C"                       ),
    [0x5e] = InstType("iput_char",                  opBACCCC,               "vA, vB, field@C"                       ),
    [0x5f] = InstType("iput_short",                 opBACCCC,               "vA, vB, field@C"                       ),
    [0x60] = InstType("sget",                       opAABBBB,               "vA, field@B"                           ),
    [0x61] = InstType("sget_wide",                  opAABBBB,               "vA, field@B"                           ),
    [0x62] = InstType("sget_object",                opAABBBB,               "vA, field@B"                           ),
    [0x63] = InstType("sget_boolean",               opAABBBB,               "vA, field@B"                           ),
    [0x64] = InstType("sget_byte",                  opAABBBB,               "vA, field@B"                           ),
    [0x65] = InstType("sget_char",                  opAABBBB,               "vA, field@B"                           ),
    [0x66] = InstType("sget_short",                 opAABBBB,               "vA, field@B"                           ),
    [0x67] = InstType("sput",                       opAABBBB,               "vA, field@B"                           ),
    [0x68] = InstType("sput_wide",                  opAABBBB,               "vA, field@B"                           ),
    [0x69] = InstType("sput_object",                opAABBBB,               "vA, field@B"                           ),
    [0x6a] = InstType("sput_boolean",               opAABBBB,               "vA, field@B"                           ),
    [0x6b] = InstType("sput_byte",                  opAABBBB,               "vA, field@B"                           ),
    [0x6c] = InstType("sput_char",                  opAABBBB,               "vA, field@B"                           ),
    [0x6d] = InstType("sput_short",                 opAABBBB,               "vA, field@B"                           ),
    [0x6e] = InstType("invoke_virtual",             opAGBBBBDCFE,           "{vC vD, vE, vF, vG}, meth@B"           ),
    [0x6f] = InstType("invoke_super",               opAGBBBBDCFE,           "{vC vD, vE, vF, vG}, meth@B"           ),
    [0x70] = InstType("invoke_direct",              opAGBBBBDCFE,           "{vC vD, vE, vF, vG}, meth@B"           ),
    [0x71] = InstType("invoke_static",              opAGBBBBDCFE,           "{vC vD, vE, vF, vG}, meth@B"           ),
    [0x72] = InstType("invoke_interface",           opAGBBBBDCFE,           "{vC vD, vE, vF, vG}, meth@B"           ),
    [0x73] = InstType("UNUSED",                     op00                                                            ),
    [0x74] = InstType("invoke_virtual/range",       opAABBBBCCCC,           "{vC .. vN}, meth@B"                    ),
    [0x75] = InstType("invoke_super/range",         opAABBBBCCCC,           "{vC .. vN}, meth@B"                    ),
    [0x76] = InstType("invoke_direct/range",        opAABBBBCCCC,           "{vC .. vN}, meth@B"                    ),
    [0x77] = InstType("invoke_static/range",        opAABBBBCCCC,           "{vC .. vN}, meth@B"                    ),
    [0x78] = InstType("invoke_interface/range",     opAABBBBCCCC,           "{vC .. vN}, meth@B"                    ),
    [0x79] = InstType("UNUSED",                     op00                                                            ),
    [0x7a] = InstType("UNUSED",                     op00                                                            ),
    [0x7b] = InstType("neg_int",                    opBA,                   "vA, vB"                                ),
    [0x7c] = InstType("not_int",                    opBA,                   "vA, vB"                                ),
    [0x7d] = InstType("neg_long",                   opBA,                   "vA, vB"                                ),
    [0x7e] = InstType("not_long",                   opBA,                   "vA, vB"                                ),
    [0x7f] = InstType("neg_float",                  opBA,                   "vA, vB"                                ),
    [0x80] = InstType("neg_double",                 opBA,                   "vA, vB"                                ),
    [0x81] = InstType("int_to_long",                opBA,                   "vA, vB"                                ),
    [0x82] = InstType("int_to_float",               opBA,                   "vA, vB"                                ),
    [0x83] = InstType("int_to_double",              opBA,                   "vA, vB"                                ),
    [0x84] = InstType("long_to_int",                opBA,                   "vA, vB"                                ),
    [0x85] = InstType("long_to_float",              opBA,                   "vA, vB"                                ),
    [0x86] = InstType("long_to_double",             opBA,                   "vA, vB"                                ),
    [0x87] = InstType("float_to_int",               opBA,                   "vA, vB"                                ),
    [0x88] = InstType("float_to_long",              opBA,                   "vA, vB"                                ),
    [0x89] = InstType("float_to_double",            opBA,                   "vA, vB"                                ),
    [0x8a] = InstType("double_to_int",              opBA,                   "vA, vB"                                ),
    [0x8b] = InstType("double_to_long",             opBA,                   "vA, vB"                                ),
    [0x8c] = InstType("double_to_float",            opBA,                   "vA, vB"                                ),
    [0x8d] = InstType("int_to_byte",                opBA,                   "vA, vB"                                ),
    [0x8e] = InstType("int_to_char",                opBA,                   "vA, vB"                                ),
    [0x8f] = InstType("int_to_short",               opBA,                   "vA, vB"                                ),
    [0x90] = InstType("add_int",                    opAACCBB,               "vA, vB, vC"                            ),
    [0x91] = InstType("sub_int",                    opAACCBB,               "vA, vB, vC"                            ),
    [0x92] = InstType("mul_int",                    opAACCBB,               "vA, vB, vC"                            ),
    [0x93] = InstType("div_int",                    opAACCBB,               "vA, vB, vC"                            ),
    [0x94] = InstType("rem_int",                    opAACCBB,               "vA, vB, vC"                            ),
    [0x95] = InstType("and_int",                    opAACCBB,               "vA, vB, vC"                            ),
    [0x96] = InstType("or_int",                     opAACCBB,               "vA, vB, vC"                            ),
    [0x97] = InstType("xor_int",                    opAACCBB,               "vA, vB, vC"                            ),
    [0x98] = InstType("shl_int",                    opAACCBB,               "vA, vB, vC"                            ),
    [0x99] = InstType("shr_int",                    opAACCBB,               "vA, vB, vC"                            ),
    [0x9a] = InstType("ushr_int",                   opAACCBB,               "vA, vB, vC"                            ),
    [0x9b] = InstType("add_long",                   opAACCBB,               "vA, vB, vC"                            ),
    [0x9c] = InstType("sub_long",                   opAACCBB,               "vA, vB, vC"                            ),
    [0x9d] = InstType("mul_long",                   opAACCBB,               "vA, vB, vC"                            ),
    [0x9e] = InstType("div_long",                   opAACCBB,               "vA, vB, vC"                            ),
    [0x9f] = InstType("rem_long",                   opAACCBB,               "vA, vB, vC"                            ),
    [0xa0] = InstType("and_long",                   opAACCBB,               "vA, vB, vC"                            ),
    [0xa1] = InstType("or_long",                    opAACCBB,               "vA, vB, vC"                            ),
    [0xa2] = InstType("xor_long",                   opAACCBB,               "vA, vB, vC"                            ),
    [0xa3] = InstType("shl_long",                   opAACCBB,               "vA, vB, vC"                            ),
    [0xa4] = InstType("shr_long",                   opAACCBB,               "vA, vB, vC"                            ),
    [0xa5] = InstType("ushr_long",                  opAACCBB,               "vA, vB, vC"                            ),
    [0xa6] = InstType("add_float",                  opAACCBB,               "vA, vB, vC"                            ),
    [0xa7] = InstType("sub_float",                  opAACCBB,               "vA, vB, vC"                            ),
    [0xa8] = InstType("mul_float",                  opAACCBB,               "vA, vB, vC"                            ),
    [0xa9] = InstType("div_float",                  opAACCBB,               "vA, vB, vC"                            ),
    [0xaa] = InstType("rem_float",                  opAACCBB,               "vA, vB, vC"                            ),
    [0xab] = InstType("add_double",                 opAACCBB,               "vA, vB, vC"                            ),
    [0xac] = InstType("sub_double",                 opAACCBB,               "vA, vB, vC"                            ),
    [0xad] = InstType("mul_double",                 opAACCBB,               "vA, vB, vC"                            ),
    [0xae] = InstType("div_double",                 opAACCBB,               "vA, vB, vC"                            ),
    [0xaf] = InstType("rem_double",                 opAACCBB,               "vA, vB, vC"                            ),
    [0xb0] = InstType("add_int/2addr",              opBA,                   "vA, vB"                                ),
    [0xb1] = InstType("sub_int/2addr",              opBA,                   "vA, vB"                                ),
    [0xb2] = InstType("mul_int/2addr",              opBA,                   "vA, vB"                                ),
    [0xb3] = InstType("div_int/2addr",              opBA,                   "vA, vB"                                ),
    [0xb4] = InstType("rem_int/2addr",              opBA,                   "vA, vB"                                ),
    [0xb5] = InstType("and_int/2addr",              opBA,                   "vA, vB"                                ),
    [0xb6] = InstType("or_int/2addr",               opBA,                   "vA, vB"                                ),
    [0xb7] = InstType("xor_int/2addr",              opBA,                   "vA, vB"                                ),
    [0xb8] = InstType("shl_int/2addr",              opBA,                   "vA, vB"                                ),
    [0xb9] = InstType("shr_int/2addr",              opBA,                   "vA, vB"                                ),
    [0xba] = InstType("ushr_int/2addr",             opBA,                   "vA, vB"                                ),
    [0xbb] = InstType("add_long/2addr",             opBA,                   "vA, vB"                                ),
    [0xbc] = InstType("sub_long/2addr",             opBA,                   "vA, vB"                                ),
    [0xbd] = InstType("mul_long/2addr",             opBA,                   "vA, vB"                                ),
    [0xbe] = InstType("div_long/2addr",             opBA,                   "vA, vB"                                ),
    [0xbf] = InstType("rem_long/2addr",             opBA,                   "vA, vB"                                ),
    [0xc0] = InstType("and_long/2addr",             opBA,                   "vA, vB"                                ),
    [0xc1] = InstType("or_long/2addr",              opBA,                   "vA, vB"                                ),
    [0xc2] = InstType("xor_long/2addr",             opBA,                   "vA, vB"                                ),
    [0xc3] = InstType("shl_long/2addr",             opBA,                   "vA, vB"                                ),
    [0xc4] = InstType("shr_long/2addr",             opBA,                   "vA, vB"                                ),
    [0xc5] = InstType("ushr_long/2addr",            opBA,                   "vA, vB"                                ),
    [0xc6] = InstType("add_float/2addr",            opBA,                   "vA, vB"                                ),
    [0xc7] = InstType("sub_float/2addr",            opBA,                   "vA, vB"                                ),
    [0xc8] = InstType("mul_float/2addr",            opBA,                   "vA, vB"                                ),
    [0xc9] = InstType("div_float/2addr",            opBA,                   "vA, vB"                                ),
    [0xca] = InstType("rem_float/2addr",            opBA,                   "vA, vB"                                ),
    [0xcb] = InstType("add_double/2addr",           opBA,                   "vA, vB"                                ),
    [0xcc] = InstType("sub_double/2addr",           opBA,                   "vA, vB"                                ),
    [0xcd] = InstType("mul_double/2addr",           opBA,                   "vA, vB"                                ),
    [0xce] = InstType("div_double/2addr",           opBA,                   "vA, vB"                                ),
    [0xcf] = InstType("rem_double/2addr",           opBA,                   "vA, vB"                                ),
    [0xd0] = InstType("add_int/lit16",              opBACCCC,               "vA, vB, #+C"                           ),
    [0xd1] = InstType("rsub_int",                   opBACCCC,               "vA, vB, #+C"                           ),
    [0xd2] = InstType("mul_int/lit16",              opBACCCC,               "vA, vB, #+C"                           ),
    [0xd3] = InstType("div_int/lit16",              opBACCCC,               "vA, vB, #+C"                           ),
    [0xd4] = InstType("rem_int/lit16",              opBACCCC,               "vA, vB, #+C"                           ),
    [0xd5] = InstType("and_int/lit16",              opBACCCC,               "vA, vB, #+C"                           ),
    [0xd6] = InstType("or_int/lit16",               opBACCCC,               "vA, vB, #+C"                           ),
    [0xd7] = InstType("xor_int/lit16",              opBACCCC,               "vA, vB, #+C"                           ),
    [0xd8] = InstType("add_int/lit8",               opAACCBB,               "vA, vB, #+C"                           ),
    [0xd9] = InstType("rsub_int/lit8",              opAACCBB,               "vA, vB, #+C"                           ),
    [0xda] = InstType("mul_int/lit8",               opAACCBB,               "vA, vB, #+C"                           ),
    [0xdb] = InstType("div_int/lit8",               opAACCBB,               "vA, vB, #+C"                           ),
    [0xdc] = InstType("rem_int/lit8",               opAACCBB,               "vA, vB, #+C"                           ),
    [0xdd] = InstType("and_int/lit8",               opAACCBB,               "vA, vB, #+C"                           ),
    [0xde] = InstType("or_int/lit8",                opAACCBB,               "vA, vB, #+C"                           ),
    [0xdf] = InstType("xor_int/lit8",               opAACCBB,               "vA, vB, #+C"                           ),
    [0xe0] = InstType("shl_int/lit8",               opAACCBB,               "vA, vB, #+C"                           ),
    [0xe1] = InstType("shr_int/lit8",               opAACCBB,               "vA, vB, #+C"                           ),
    [0xe2] = InstType("ushr_int/lit8",              opAACCBB,               "vA, vB, #+C"                           ),
    [0xe3] = InstType("UNUSED",                     op00                                                            ),
    [0xe4] = InstType("UNUSED",                     op00                                                            ),
    [0xe5] = InstType("UNUSED",                     op00                                                            ),
    [0xe6] = InstType("UNUSED",                     op00                                                            ),
    [0xe7] = InstType("UNUSED",                     op00                                                            ),
    [0xe8] = InstType("UNUSED",                     op00                                                            ),
    [0xe9] = InstType("UNUSED",                     op00                                                            ),
    [0xea] = InstType("UNUSED",                     op00                                                            ),
    [0xeb] = InstType("UNUSED",                     op00                                                            ),
    [0xec] = InstType("UNUSED",                     op00                                                            ),
    [0xed] = InstType("UNUSED",                     op00                                                            ),
    [0xee] = InstType("UNUSED",                     op00                                                            ),
    [0xef] = InstType("UNUSED",                     op00                                                            ),
    [0xf0] = InstType("UNUSED",                     op00                                                            ),
    [0xf1] = InstType("UNUSED",                     op00                                                            ),
    [0xf2] = InstType("UNUSED",                     op00                                                            ),
    [0xf3] = InstType("UNUSED",                     op00                                                            ),
    [0xf4] = InstType("UNUSED",                     op00                                                            ),
    [0xf5] = InstType("UNUSED",                     op00                                                            ),
    [0xf6] = InstType("UNUSED",                     op00                                                            ),
    [0xf7] = InstType("UNUSED",                     op00                                                            ),
    [0xf8] = InstType("UNUSED",                     op00                                                            ),
    [0xf9] = InstType("UNUSED",                     op00                                                            ),
    [0xfa] = InstType("invoke_polymorphic",         opAGBBBBDCFEHHHH,       "{vC, vD, vE, vF, vG}, meth@B, proto@H" ),
    [0xfb] = InstType("invoke_polymorphic/range",   opAABBBBCCCCHHHH,       "{vC .. vN}, meth@B, proto@H"           ),
    [0xfc] = InstType("invoke_custom",              opAGBBBBDCFE,           "{vC, vD, vE, vF, vG}, call_site@B"     ),
    [0xfd] = InstType("invoke_custom/range",        opAABBBBCCCC,           "{vC .. vN}, call_site@B"               ),
    [0xfe] = InstType("const_method_handle",        opAABBBB,               "vA, method_handle@B"                   ),
    [0xff] = InstType("const_method_type",          opAABBBB,               "vA, proto@B"                           ),
};
