#include "analyze.h"

#include "dexstruct.h"

#include <set>
#include <cstdio>

using std::set;

vector<BasicBlock> split_basic_blocks(const EncodedMethod & method)
{
    auto i_begin = * method.begin();
    auto i_end = * method.end();

    set<Inst> split_points;

    for (auto inst : method) {
        if (inst >= i_end) {
            fputs("instruction exceeds end of method\n", stderr);
            break;
        }

        if (inst.is_return()) {
            split_points.insert(inst.next());

        } else if (inst.is_throw()) {
            split_points.insert(inst.next());

        } else if (inst.is_goto()) {
            split_points.insert(inst.next());
            split_points.insert(inst.goto_target());

        } else if (inst.is_switch()) {
            split_points.insert(inst.next());
            auto targets = inst.switch_targets(i_begin, i_end);
            split_points.insert(targets.begin(), targets.end());

        } else if (inst.is_branch()) {
            split_points.insert(inst.next());
            split_points.insert(inst.branch_target());
        }
    }

    vector<BasicBlock> ret;
    Inst prev = i_begin;
    for (auto inst : split_points) {
        if (inst != prev && i_begin <= inst && inst <= i_end) {
            ret.emplace_back(prev, inst);
            prev = inst;
        }
    }
    return ret;
}
