#include "analyze.h"

#include "dexstruct.h"

#include <set>

using std::set;

vector<BasicBlock> split_basic_blocks(const EncodedMethod & method)
{
    set<Inst> split_points;
    split_points.insert(* method.begin());

    for (auto inst : method) {
        if (inst.is_return()) {
            split_points.insert(inst.next());

        } else if (inst.is_throw()) {
            split_points.insert(inst.next());

        } else if (inst.is_goto()) {
            split_points.insert(inst.next());
            split_points.insert(inst.goto_target());

        } else if (inst.is_switch()) {
            split_points.insert(inst.next());
            auto targets = inst.switch_targets();
            split_points.insert(targets.begin(), targets.end());

        } else if (inst.is_branch()) {
            split_points.insert(inst.next());
            split_points.insert(inst.branch_target());
        }
    }

    vector<BasicBlock> ret;
    Inst prev = * split_points.begin();
    for (auto inst : split_points) {
        if (inst != prev) {
            ret.emplace_back(prev, inst);
            prev = inst;
        }
    }
    return ret;
}
