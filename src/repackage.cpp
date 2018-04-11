#include "capi.h"
#include "analyze.h"
#include "dex.h"

#include <algorithm>
#include <cstdio>

namespace {

using BlockFeature = vector<uint8_t>;
using MethodFeature = vector<BlockFeature>;
using ClassFeature = vector<MethodFeature>;
using DexFeature = vector<ClassFeature>;

//const Dex * _dex = nullptr;

BlockFeature block_features(BasicBlock block, bool ordered)
{
    BlockFeature ret;
    for (Inst i : block)
        if (i.op() != 0)
            ret.push_back(i.op());
    if (ordered) std::sort(ret.begin(), ret.end());
    return ret;
}

MethodFeature method_features(const EncodedMethod & method, bool ordered)
{
    //printf("  decoding method %s [%04x]\n", _dex->methods[method.method_id].name.cstr(), method.inst_size);
    //for (Inst inst : method) {
    //    for (int i = 0; i < inst.length(); ++i)
    //        printf("%02x ", inst.bytes[i]);
    //    putchar('\n');
    //}

    MethodFeature ret;
    for (auto block : split_basic_blocks(method))
        ret.push_back(block_features(block, ordered));
    std::sort(ret.begin(), ret.end());
    return ret;
}

ClassFeature class_features(const Class & class_, bool ordered)
{
    //printf("decoding class %s\n", class_.name.cstr());
    ClassFeature ret;
    for (const auto & method : class_.direct_methods)
        ret.push_back(method_features(method, ordered));
    for (const auto & method : class_.virtual_methods)
        ret.push_back(method_features(method, ordered));

    auto mid = ret.begin() + class_.direct_methods.size();
    std::sort(ret.begin(), mid);
    std::sort(mid, ret.end());
    return ret;
}

DexFeature dex_features(const Dex * dex, bool ordered)
{
    DexFeature ret;
    for (const auto & class_ : dex->classes)
        ret.push_back(class_features(class_, ordered));
    std::sort(ret.begin(), ret.end());
    return ret;
}

vector<int> encode(const ClassFeature & cf)
{
    vector<int> ret;
    ret.push_back(-1);
    for (const auto & mf : cf) {
        ret.push_back(-2);
        for (const auto & bf : mf) {
            ret.push_back(-3);
            ret.insert(ret.end(), bf.cbegin(), bf.cend());
        }
    }
    return ret;
}

vector<int> encode(const DexFeature & df)
{
    vector<int> ret;
    for (const auto & cf : df) {
        ret.push_back(-1);
        for (const auto & mf : cf) {
            ret.push_back(-2);
            for (const auto & bf : mf) {
                ret.push_back(-3);
                ret.insert(ret.end(), bf.cbegin(), bf.cend());
            }
        }
    }
    return ret;
}

}

vector<int> class_repackage_features(const Class & class_, bool ordered)
{
    auto features = class_features(class_, ordered);
    return encode(features);
}

vector<int> repackage_features(const Dex * dex, bool ordered)
{
    //_dex = dex;
    auto features = dex_features(dex, ordered);
    return encode(features);
}
