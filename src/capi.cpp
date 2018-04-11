#include "capi.h"

#include "dex.h"
#include "inst.h"

#include <iostream>

namespace {
vector<Dex*> dex_list;

IntArray * update_buffer(IntArray * & buffer, const vector<int> & vec)
{
    delete [] (int32_t *) buffer;
    buffer = (IntArray *) new int32_t [ vec.size() + 1 ];
    buffer->len = vec.size();
    std::copy(vec.cbegin(), vec.cend(), buffer->data);
    return buffer;
}

int insert_dex(Dex * dex)
{
    for (unsigned i = 0; i < dex_list.size(); ++i) {
        if (dex_list[i] == nullptr) {
            dex_list[i] = dex;
            return i;
        }
    }

    dex_list.push_back(dex);
    return dex_list.size() - 1;
}
}

int32_t load_dex(const char * dex_file_name)
{
    try {
        int fd = Reader::open_file(dex_file_name);
        Reader r = Reader::from_fd(fd);
        auto dex = new Dex(r, fd);
        return insert_dex(dex);
    } catch (const std::exception & e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}

void release_dex(int32_t dex_id)
{
    delete dex_list[dex_id];
    dex_list[dex_id] = nullptr;
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
        if (inst.is_invoke() && inst.invoke_target() < (int) dex_list[dex_id]->methods.size())
            ret.push_back(inst.invoke_target());

    return update_buffer(buffer, ret);
}


const IntArray * get_invoked_methods_libradar(int32_t dex_id, int32_t class_id, int32_t method_idx)
{
    static IntArray * buffer = nullptr;

    const auto & method = dex_list[dex_id]->classes[class_id].methods(method_idx);
    if (! method.valid()) return nullptr;

    vector<int> ret;
    for (const auto & inst : method)
        if (inst.is_libradar_invoke() && inst.invoke_target() < (int) dex_list[dex_id]->methods.size())
            ret.push_back(inst.invoke_target());

    return update_buffer(buffer, ret);
}

vector<int> repackage_features(const Dex * dex, bool ordered);

const IntArray * get_repackage_features(int32_t dex_id, int32_t ordered)
{
    static IntArray * buffer = nullptr;

    auto ret = repackage_features(dex_list[dex_id], ordered != 0);
    return update_buffer(buffer, ret);
}

vector<int> class_repackage_features(const Class & class_, bool ordered);

const IntArray * get_class_repackage_features(int32_t dex_id, int32_t class_id, int32_t ordered)
{
    static IntArray * buffer = nullptr;

    const auto & c = dex_list[dex_id]->classes[class_id];
    auto ret = class_repackage_features(c, ordered != 0);
    return update_buffer(buffer, ret);
}
