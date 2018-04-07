#include "capi.h"

#include "dex.h"
#include "inst.h"

#include <iostream>

const char * hello() { return "hello, world\n"; }

namespace {
vector<Dex*> dex_list;
}

int32_t load_dex(const char * dex_file_name)
{
    try {
        Reader r = Reader::open_file(dex_file_name);
        auto dex = new Dex(r);
        dex_list.push_back(dex);
        return dex_list.size() - 1;
    } catch (const std::exception & e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}

int32_t get_class_count(int32_t dex_id)
{
    return dex_list[dex_id]->classes.size();
}

const char * get_class_name(int32_t dex_id, int32_t class_id)
{
    static const char * buffer = nullptr;

    delete buffer;
    buffer = dex_list[dex_id]->classes[class_id].name.cstr();
    return buffer;
}

int32_t get_methods_count(int32_t dex_id, int32_t class_id)
{
    const auto & c = dex_list[dex_id]->classes[class_id];
    return c.direct_methods.size() + c.virtual_methods.size();
}

const char * get_method_full_name(int32_t dex_id, int32_t method_id)
{
    static const char * buffer = nullptr;

    if (! dex_list[dex_id]->has_method(method_id)) return nullptr;
    delete buffer;
    buffer = dex_list[dex_id]->methods[method_id].full_name().cstr();
    return buffer;
}

const char * get_class_method_full_name(int32_t dex_id, int32_t class_id, int32_t method_idx)
{
    static const char * buffer = nullptr;

    delete buffer;
    auto dex = dex_list[dex_id];
    int method_id = dex->classes[class_id].methods(method_idx).method_id;
    buffer = dex->methods[method_id].full_name().cstr();
    return buffer;
}

const IntArray * get_invoked_methods(int32_t dex_id, int32_t class_id, int32_t method_idx)
{
    static IntArray * buffer = nullptr;

    const auto & method = dex_list[dex_id]->classes[class_id].methods(method_idx);
    if (! method.valid()) return nullptr;

    vector<int> ret;
    for (const auto & inst : method)
        if (inst.is_invoke() && inst.invoke_target() < dex_list[dex_id]->methods.size())
            ret.push_back(inst.invoke_target());

    delete [] (int32_t *) buffer;
    buffer = (IntArray *) new int32_t [ ret.size() + 1 ];
    buffer->len = ret.size();
    std::copy(ret.cbegin(), ret.cend(), buffer->data);
    return buffer;
}


const IntArray * get_invoked_methods_libradar(int32_t dex_id, int32_t class_id, int32_t method_idx)
{
    static IntArray * buffer = nullptr;

    const auto & method = dex_list[dex_id]->classes[class_id].methods(method_idx);
    if (! method.valid()) return nullptr;

    vector<int> ret;
    for (const auto & inst : method)
        if (inst.is_libradar_invoke() && inst.invoke_target() < dex_list[dex_id]->methods.size())
            ret.push_back(inst.invoke_target());

    delete [] (int32_t *) buffer;
    buffer = (IntArray *) new int32_t [ ret.size() + 1 ];
    buffer->len = ret.size();
    std::copy(ret.cbegin(), ret.cend(), buffer->data);
    return buffer;
}

vector<int> repackage_features(const Dex * dex, bool ordered);

const IntArray * get_repackage_features(int32_t dex_id, int32_t ordered)
{
    static IntArray * buffer = nullptr;

    auto ret = repackage_features(dex_list[dex_id], ordered != 0);
    delete [] (int32_t *) buffer;
    buffer = (IntArray *) new int32_t [ ret.size() + 1 ];
    buffer->len = ret.size();
    std::copy(ret.cbegin(), ret.cend(), buffer->data);
    return buffer;
}
