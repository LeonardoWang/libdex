import subprocess

def run(args):
    proc = subprocess.run(args, stdout = subprocess.PIPE, check = True)
    return proc.stdout.decode()


branch_ops = set([ 'bl', 'blx', 'cbnz', 'cbz' ])
jump_ops = set([ 'b', 'bxj' ])
return_ops = set([ 'bkpt', 'bx', 'eret', 'hlt' ])
cond_suffixs = set([ 'eq', 'ne', 'cs', 'cc', 'mi', 'pl', 'vs', 'vc', 'hi', 'ls', 'ge', 'lt', 'gt', 'le', 'al' ])
hex_digits = set([ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' ])

class Inst:
    def __init__(self, line = None):
        self.addr = None
        self.op = None
        self.args = None

        self.is_entry = False
        self.accessed = False

        self.base_op = None
        self.cond = False

        if line is None: return

        t = line.split('\t', 3)
        addr, data, self.op, self.args = line.split('\t', 3)
        self.addr = int(addr.strip()[:-1], 16)

        self.base_op = self.op.split('.', 1)[0]
        if len(self.op) > 2 and self.op[-2:] in cond_suffixs:
            self.base_op = self.base_op[:-2]
            self.cond = True


    def is_branch(self):
        if self.base_op in branch_ops: return True
        if not self.cond: return False
        return self.base_op in jump_ops or self.base_op in return_ops

    def is_jump(self):
        return not self.cond and self.base_op in jump_ops

    def is_return(self):
        return not self.cond and self.base_op in return_ops

    def target_addr(self):
        ret = self.args.split(' ', 1)[0]
        try:
            return int(ret, 16)
        except ValueError:
            return -1



EndInst = Inst()
EndInst.is_entry = True
EndInst.accessed = True


class Func:
    def __init__(self, text):
        lines = text.split('\n')

        assert lines[0][9] == '<' and lines[0][-2] == '>'
        self.orig_name = lines[0][10:-2]

        if '\t...' in lines:
            lines = lines[ : lines.index('\t...') ]
        self.insts = [ Inst(line) for line in lines[1:] ]
        self.insts.append(EndInst)

        self.insts[0].is_entry = True
        self.run(0)

        blocks = [ ]
        cur_block = [ ]
        for inst in self.insts:
            if inst.is_entry or not inst.accessed:
                if cur_block:
                    blocks.append(cur_block)
                    cur_block = [ ]
            if inst.accessed:
                cur_block.append(inst.op)

        self.blocks = sorted(blocks)


    def run(self, idx):
        while not self.insts[idx].accessed:
            inst = self.insts[idx]
            inst.accessed = True

            if inst.is_branch() or inst.is_jump():
                target_idx = self.addr_to_idx(inst.target_addr())
                if target_idx is not None:
                    self.insts[target_idx].is_entry = True
                    self.run(target_idx)

            if inst.is_jump() or inst.is_return():
                return

            if inst.is_branch():
                self.insts[idx + 1].is_entry = True
            idx += 1


    def addr_to_idx(self, addr):
        if addr < 0: return None
        begin = self.insts[0].addr
        end = begin + 4 * len(self.insts)
        if addr < begin or addr >= end: return None
        return (addr - begin) // 4


def repackage_features(path):
    out = run(['arm-none-eabi-objdump', '-d', '-j', '.text', path])
    funcs = [ f.strip() for f in out.split('\n\n') ]
    assert funcs[1] == 'Disassembly of section .text:'

    return [ Func(f).blocks for f in funcs[3:] ]


import sys
import json

if __name__ == '__main__':
    result = repackage_features(sys.argv[1])
    for f in result:
        print(f)
