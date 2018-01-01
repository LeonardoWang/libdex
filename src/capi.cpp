#include "capi.h"

#include "dex.h"
#include "inst.h"
#include "instformat.h"

const char * hello() { return "hello, world\n"; }

namespace {
vector<Dex*> dex_list;
}

void load_inst_conf(const char * path)
{
    InstFormat::load(path);
}

int32_t load_dex(const char * dex_file_name)
{
    Reader r = Reader::open_file(dex_file_name);
    auto dex = new Dex(r);
    dex_list.push_back(dex);
    return dex_list.size() - 1;
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
    auto insts = Inst::load_method(method);

    vector<int> ret;
    for (const auto & inst : insts)
        if (inst.is_invoke())
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
    auto insts = Inst::load_method(method);

    vector<int> ret;
    for (const auto & inst : insts)
        if (inst.is_libradar_invoke())
            ret.push_back(inst.invoke_target());

    delete [] (int32_t *) buffer;
    buffer = (IntArray *) new int32_t [ ret.size() + 1 ];
    buffer->len = ret.size();
    std::copy(ret.cbegin(), ret.cend(), buffer->data);
    return buffer;
}
