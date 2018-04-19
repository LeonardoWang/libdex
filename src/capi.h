#pragma once

/** @file
 *
 *  C-style APIs (for Python)
 *
 *  In general, any object in C-style API is identified by one or more IDs or indices.
 *  All IDs are sequential and starts from 0, except that DEX IDs may be reused after releasing.
 *  Any return value less than 0 indicates that an error occurred.
 *  ID of dex files are returned by their loading function.
 *  ID of classes are local to dex file.
 *  Methods has two representation form:
 *      the "declaration" of method is binded to dex file;
 *      the "definition" of method is binded to class.
 *  Definition of a method does not contain its name and signature, but offers its declaration ID.
 *
 *  Strings and arrays returned by C-style APIs are stored in internal buffers.
 *  As a result the caller should not modify or free them,
 *  and the data is available until next invocation.
 *
 *  NOTE: remember to release DEX objects.
 **/

#include <cstdint>

struct IntArray {
    int32_t len;
    int32_t data[];
};

extern "C" {

/**
 *  Load a dex file; return its ID.
 **/
int32_t load_dex(const char * dex_file_name);

/**
 *  Release a dex file
 **/
void release_dex(int32_t dex_id);

/**
 *  Get number of classes defined in a dex file.
 *  Their IDs are 0, 1, 2, ... , (return_value - 1).
 **/
int32_t get_class_count(int32_t dex_id);

/**
 *  Get the name of a class. (e.g. `Ljava/lang/String;`)
 **/
const char * get_class_name(int32_t dex_id, int32_t class_id);

/**
 *  Get number of methods defined in a class, containing direct and virtual functions.
 *  Their indices are 0, 1, 2, ... , (return_value - 1).
 **/
int32_t get_methods_count(int32_t dex_id, int32_t class_id);

/**
 *  Get the name of a method from its dex-wise ID. (e.g. `Ljava/lang/String;-><init>`)
 **/
const char * get_method_full_name(int32_t dex_id, int32_t method_id);

/**
 *  Get the name of a method from its class-wise index. (e.g. `Ljava/lang/String;-><init>`)
 **/
const char * get_class_method_full_name(int32_t dex_id, int32_t class_id, int32_t method_idx);

/**
 *  Get all invoked methods in a method; return an array of their dex-wise ID.
 *  The result may contain duplicate methods if they occured multiple times in the byte code.
 *  The order of return value is guaranteed to match the byte code.
 **/
const IntArray * get_invoked_methods(int32_t dex_id, int32_t class_id, int32_t method_idx);


/**
 *  Similar to `get_invoked_methods`, but ignore ranged and polymorphic invocations.
 **/
const IntArray * get_invoked_methods_libradar(int32_t dex_id, int32_t class_id, int32_t method_idx);

/**
 *  Get a tree of basic block features.
 *  The OPs will preserve their original order in a basic block if parameter `ordered` is `false`;
 *  otherwise they'll be sorted based on numeric value.
 *  The tree itself is always sorted (i.e. modifying class/method names will not alter the tree).
 *  NOPs and pseudo instructions are ignored.
 *  Function calls are considered as simple instructions and have no effect on basic block splitting.
 *
 *  Example tree:
 *  (dex)
 *   +- (class1)
 *   |   +- (method1)
 *   |   |   +- (block1)
 *   |   |   |  +- [ op1, op1, op2, op1, op4 ]
 *   |   |   +- (block2)
 *   |   |      +- [ op5 ]
 *   |   +- (method2) (empty)
 *   +- (class2)
 *       +- (method1)
 *           +- (block1)
 *              +- [ op3, op2 ]
 *
 *  Encoded array of above tree:
 *      -1 , -2 , -3 , op1 , op1 , op2, op1, op4 , -3 , op5 , -2 , -1, -2, -3 , op3, op2
 **/
const IntArray * get_repackage_features(int32_t dex_id, int32_t ordered);

/**
 *  Get a tree of basic block features in a class.
 *  The format is indentical to `get_repackage_features`.
 **/
const IntArray * get_class_repackage_features(int32_t dex_id, int32_t class_id, int32_t ordered);

}
