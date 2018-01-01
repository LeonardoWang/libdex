#pragma once

/** @file
 *  Structures defined in dex files.
 **/

#include "reader.h"

/** Signature of method **/
struct Proto {
    String shorty;
    String ret_type;
    vector<String> params;
};

/** Definition of field **/
struct Field {
    String class_;
    String type;
    String name;
};

/** Declaration of method **/
struct Method {
    String class_;
    int proto_id;
    String name;

    String full_name() const { return class_ + "->" + name; }
};

/**
 *  Definition of field in class
 *  Fields with identical name may have different access flags in base and derived classes.
 **/
struct EncodedField {
    int field_id;
    uint32_t access_flags;
};

/**
 *  Definition of method in class
 *  Methods with identical name may contain different instructions in base and derived classes.
 **/
struct EncodedMethod {
    int method_id;
    uint32_t access_flags;

    uint16_t reg_size;
    uint16_t ins_size;
    uint16_t outs_size;
    uint16_t tries_size;
    uint32_t debug_info_off;
    uint32_t inst_size;
    const uint8_t * insts;
    // uint16_t padding = 0;
    // vector<Try> tries;
    // EncodedCatchHandlerList handlers;

    void load_detail(Reader r);
    void clear();
};

/** Definition of class **/
struct Class {
    String name;
    uint32_t access_flags;
    String super;
    vector<String> interfaces;
    String src_file;
    uint32_t annotations_off;

    vector<EncodedField> static_fields;
    vector<EncodedField> instance_fields;

    vector<EncodedMethod> direct_methods;
    vector<EncodedMethod> virtual_methods;

    uint32_t static_values_off;

    EncodedMethod methods(int idx)
    {
        if (idx < (int) direct_methods.size())
            return direct_methods[idx];
        else
            return virtual_methods[idx - direct_methods.size()];
    }
};
