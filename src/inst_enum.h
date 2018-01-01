#pragma once

enum OpCode {
    Nop                     = 0x00,
    Move                    = 0x01,
    Move_from16             = 0x02,
    Move_16                 = 0x03,
    MoveWide                = 0x04,
    MoveWide_from16         = 0x05,
    MoveWide_16             = 0x06,
    MoveObject              = 0x07,
    MoveObject_from16       = 0x08,
    MoveObject_16           = 0x09,
    MoveResult              = 0x0a,
    MoveResultWide          = 0x0b,
    MoveResultObject        = 0x0c,
    MoveException           = 0x0d,
    ReturnVoid              = 0x0e,
    Return                  = 0x0f,
    ReturnWide              = 0x10,
    ReturnObject            = 0x11,
    Const_4                 = 0x12,
    Const_16                = 0x13,
    Const                   = 0x14,
    Const_high16            = 0x15,
    ConstWide_16            = 0x16,
    ConstWide_32            = 0x17,
    ConstWide               = 0x18,
    ConstWide_high16        = 0x19,
    ConstString             = 0x1a,
    ConstString_jumbo       = 0x1b,
    ConstClass              = 0x1c,
    MonitorEnter            = 0x1d,
    MonitorExit             = 0x1e,
    CheckCast               = 0x1f,
    InstanceOf              = 0x20,
    ArrayLength             = 0x21,
    NewInstance             = 0x22,
    NewArray                = 0x23,
    FilledNewArray          = 0x24,
    FilledNewArray_range    = 0x25,
    FillArrayData           = 0x26,
    Throw                   = 0x27,
    Goto                    = 0x28,
    Goto_16                 = 0x29,
    Goto_32                 = 0x2a,
    PackedSwitch            = 0x2b,
    SparseSwitch            = 0x2c,
    CmplFloat               = 0x2d,
    CmpgFloat               = 0x2e,
    CmplDouble              = 0x2f,
    CmpgDouble              = 0x30,
    CmpLong                 = 0x31,
    IfEq                    = 0x32,
    IfNe                    = 0x33,
    IfLt                    = 0x34,
    IfGe                    = 0x35,
    IfGt                    = 0x36,
    IfLe                    = 0x37,
    IfEqz                   = 0x38,
    IfNez                   = 0x39,
    IfLtz                   = 0x3a,
    IfGez                   = 0x3b,
    IfGtz                   = 0x3c,
    IfLez                   = 0x3d,
//  UNUSED                  = 0x3e,
//  UNUSED                  = 0x3f,
//  UNUSED                  = 0x40,
//  UNUSED                  = 0x41,
//  UNUSED                  = 0x42,
//  UNUSED                  = 0x43,
    Aget                    = 0x44,
    AgetWide                = 0x45,
    AgetObject              = 0x46,
    AgetBoolean             = 0x47,
    AgetByte                = 0x48,
    AgetChar                = 0x49,
    AgetShort               = 0x4a,
    Aput                    = 0x4b,
    AputWide                = 0x4c,
    AputObject              = 0x4d,
    AputBoolean             = 0x4e,
    AputByte                = 0x4f,
    AputChar                = 0x50,
    AputShort               = 0x51,
    Iget                    = 0x52,
    IgetWide                = 0x53,
    IgetObject              = 0x54,
    IgetBoolean             = 0x55,
    IgetByte                = 0x56,
    IgetChar                = 0x57,
    IgetShort               = 0x58,
    Iput                    = 0x59,
    IputWide                = 0x5a,
    IputObject              = 0x5b,
    IputBoolean             = 0x5c,
    IputByte                = 0x5d,
    IputChar                = 0x5e,
    IputShort               = 0x5f,
    Sget                    = 0x60,
    SgetWide                = 0x61,
    SgetObject              = 0x62,
    SgetBoolean             = 0x63,
    SgetByte                = 0x64,
    SgetChar                = 0x65,
    SgetShort               = 0x66,
    Sput                    = 0x67,
    SputWide                = 0x68,
    SputObject              = 0x69,
    SputBoolean             = 0x6a,
    SputByte                = 0x6b,
    SputChar                = 0x6c,
    SputShort               = 0x6d,
    InvokeVirtual           = 0x6e,
    InvokeSuper             = 0x6f,
    InvokeDirect            = 0x70,
    InvokeStatic            = 0x71,
    InvokeInterface         = 0x72,
//  UNUSED                  = 0x73,
    InvokeVirtual_range     = 0x74,
    InvokeSuper_range       = 0x75,
    InvokeDirect_range      = 0x76,
    InvokeStatic_range      = 0x77,
    InvokeInterface_range   = 0x78,
//  UNUSED                  = 0x79,
//  UNUSED                  = 0x7a,
    NegInt                  = 0x7b,
    NotInt                  = 0x7c,
    NegLong                 = 0x7d,
    NotLong                 = 0x7e,
    NegFloat                = 0x7f,
    NegDouble               = 0x80,
    IntToLong               = 0x81,
    IntToFloat              = 0x82,
    IntToDouble             = 0x83,
    LongToInt               = 0x84,
    LongToFloat             = 0x85,
    LongToDouble            = 0x86,
    FloatToInt              = 0x87,
    FloatToLong             = 0x88,
    FloatToDouble           = 0x89,
    DoubleToInt             = 0x8a,
    DoubleToLong            = 0x8b,
    DoubleToFloat           = 0x8c,
    IntToByte               = 0x8d,
    IntToChar               = 0x8e,
    IntToShort              = 0x8f,
    AddInt                  = 0x90,
    SubInt                  = 0x91,
    MulInt                  = 0x92,
    DivInt                  = 0x93,
    RemInt                  = 0x94,
    AndInt                  = 0x95,
    OrInt                   = 0x96,
    XorInt                  = 0x97,
    ShlInt                  = 0x98,
    ShrInt                  = 0x99,
    UshrInt                 = 0x9a,
    AddLong                 = 0x9b,
    SubLong                 = 0x9c,
    MulLong                 = 0x9d,
    DivLong                 = 0x9e,
    RemLong                 = 0x9f,
    AndLong                 = 0xa0,
    OrLong                  = 0xa1,
    XorLong                 = 0xa2,
    ShlLong                 = 0xa3,
    ShrLong                 = 0xa4,
    UshrLong                = 0xa5,
    AddFloat                = 0xa6,
    SubFloat                = 0xa7,
    MulFloat                = 0xa8,
    DivFloat                = 0xa9,
    RemFloat                = 0xaa,
    AddDouble               = 0xab,
    SubDouble               = 0xac,
    MulDouble               = 0xad,
    DivDouble               = 0xae,
    RemDouble               = 0xaf,
    AddInt_2addr            = 0xb0,
    SubInt_2addr            = 0xb1,
    MulInt_2addr            = 0xb2,
    DivInt_2addr            = 0xb3,
    RemInt_2addr            = 0xb4,
    AndInt_2addr            = 0xb5,
    OrInt_2addr             = 0xb6,
    XorInt_2addr            = 0xb7,
    ShlInt_2addr            = 0xb8,
    ShrInt_2addr            = 0xb9,
    UshrInt_2addr           = 0xba,
    AddLong_2addr           = 0xbb,
    SubLong_2addr           = 0xbc,
    MulLong_2addr           = 0xbd,
    DivLong_2addr           = 0xbe,
    RemLong_2addr           = 0xbf,
    AndLong_2addr           = 0xc0,
    OrLong_2addr            = 0xc1,
    XorLong_2addr           = 0xc2,
    ShlLong_2addr           = 0xc3,
    ShrLong_2addr           = 0xc4,
    UshrLong_2addr          = 0xc5,
    AddFloat_2addr          = 0xc6,
    SubFloat_2addr          = 0xc7,
    MulFloat_2addr          = 0xc8,
    DivFloat_2addr          = 0xc9,
    RemFloat_2addr          = 0xca,
    AddDouble_2addr         = 0xcb,
    SubDouble_2addr         = 0xcc,
    MulDouble_2addr         = 0xcd,
    DivDouble_2addr         = 0xce,
    RemDouble_2addr         = 0xcf,
    AddInt_lit16            = 0xd0,
    RsubInt                 = 0xd1,
    MulInt_lit16            = 0xd2,
    DivInt_lit16            = 0xd3,
    RemInt_lit16            = 0xd4,
    AndInt_lit16            = 0xd5,
    OrInt_lit16             = 0xd6,
    XorInt_lit16            = 0xd7,
    AddInt_lit8             = 0xd8,
    RsubInt_lit8            = 0xd9,
    MulInt_lit8             = 0xda,
    DivInt_lit8             = 0xdb,
    RemInt_lit8             = 0xdc,
    AndInt_lit8             = 0xdd,
    OrInt_lit8              = 0xde,
    XorInt_lit8             = 0xdf,
    ShlInt_lit8             = 0xe0,
    ShrInt_lit8             = 0xe1,
    UshrInt_lit8            = 0xe2,
//  UNUSED                  = 0xe3,
//  UNUSED                  = 0xe4,
//  UNUSED                  = 0xe5,
//  UNUSED                  = 0xe6,
//  UNUSED                  = 0xe7,
//  UNUSED                  = 0xe8,
//  UNUSED                  = 0xe9,
//  UNUSED                  = 0xea,
//  UNUSED                  = 0xeb,
//  UNUSED                  = 0xec,
//  UNUSED                  = 0xed,
//  UNUSED                  = 0xee,
//  UNUSED                  = 0xef,
//  UNUSED                  = 0xf0,
//  UNUSED                  = 0xf1,
//  UNUSED                  = 0xf2,
//  UNUSED                  = 0xf3,
//  UNUSED                  = 0xf4,
//  UNUSED                  = 0xf5,
//  UNUSED                  = 0xf6,
//  UNUSED                  = 0xf7,
//  UNUSED                  = 0xf8,
//  UNUSED                  = 0xf9,
    InvokePolymorphic       = 0xfa,
    InvokePolymorphic_range = 0xfb,
    InvokeCustom            = 0xfc,
    InvokeCustom_range      = 0xfd,
//  UNUSED                  = 0xfe,
//  UNUSED                  = 0xff,
};
