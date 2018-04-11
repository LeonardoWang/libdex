from ctypes import *
from os import path

so_path = path.join(path.abspath(path.dirname(__file__)), 'libdex.so')
libdex = cdll.LoadLibrary(so_path)

# These functions are defined in `capi.h/cpp`

libdex.hello.argtypes = [ ]
libdex.hello.restype = c_char_p

libdex.load_dex.argstypes = [ c_char_p ]
libdex.load_dex.restype = c_int32

libdex.get_class_count.argstypes = [ c_int32 ]
libdex.get_class_count.restype = c_int32

libdex.get_class_name.argstypes = [ c_int32, c_int32 ]
libdex.get_class_name.restype = c_char_p

libdex.get_methods_count.argstypes = [ c_int32, c_int32 ]
libdex.get_methods_count.restype = c_int32

libdex.get_method_full_name.argstypes = [ c_int32, c_int32 ]
libdex.get_method_full_name.restype = c_char_p

libdex.get_class_method_full_name.argstypes = [ c_int32, c_int32, c_int32 ]
libdex.get_class_method_full_name.restype = c_char_p

libdex.get_invoked_methods.argstypes = [ c_int32, c_int32, c_int32 ]
libdex.get_invoked_methods.restype = POINTER(c_int32)

libdex.get_invoked_methods_libradar.argstypes = [ c_int32, c_int32, c_int32 ]
libdex.get_invoked_methods_libradar.restype = POINTER(c_int32)

libdex.get_repackage_features.argstypes = [ c_int32, c_int32 ]
libdex.get_repackage_features.restype = POINTER(c_int32)

libdex.get_class_repackage_features.argstypes = [ c_int32, c_int32, c_int32 ]
libdex.get_class_repackage_features.restype = POINTER(c_int32)

def decode_int_array(ptr):
    ret = [ ]
    for i in range(ptr[0]):
        ret.append(ptr[i + 1])
    return ret


def decode_features(arr, level):
    if len(arr) == 0 or arr[0] >= 0: return arr
    assert arr[0] == level - 1, (arr[0], level)

    ret = [ ]
    last = 0
    for i in range(1, len(arr)):
        if arr[i] == level - 1:
            ret.append(decode_features(arr[last + 1 : i], level - 1))
            last = i
    ret.append(decode_features(arr[last + 1 : ], level - 1))
    return ret


class Dex:
    def __init__(self, dex_file_name):
        if type(dex_file_name) is str:
            dex_file_name = dex_file_name.encode('utf8')
        if type(dex_file_name) is not bytes:
            raise RuntimeError('dex_file_name has wrong type %s' % type(dex_file_name))

        self.id = libdex.load_dex(dex_file_name)
        assert self.id >= 0

        class_cnt = libdex.get_class_count(self.id)
        self.classes = [ DexClass(self, i) for i in range(class_cnt) ]

    def get_repackage_features(self, ordered = False):
        ptr = libdex.get_repackage_features(self.id, 1 if ordered else 0)
        arr = decode_int_array(ptr)
        return decode_features(arr, 0)


class DexClass:
    def __init__(self, dex, id_):
        self.dex = dex
        self.id = id_
        self._name = None
        self._methods = None

    def name(self):
        if self._name is None:
            name_bytes = libdex.get_class_name(self.dex.id, self.id)
            self._name = name_bytes.decode('utf8')
        return self._name

    def methods(self):
        if self._methods is None:
            method_cnt = libdex.get_methods_count(self.dex.id, self.id)
            self._methods = [ DexMethod(self, i) for i in range(method_cnt) ]
        return self._methods

    def get_repackage_features(self, ordered = False):
        ptr = libdex.get_class_repackage_features(self.dex.id, self.id, 1 if ordered else 0)
        arr = decode_int_array(ptr)
        ret = decode_features(arr, 0)
        assert len(ret) == 1
        return ret[0]


class DexMethod:
    def __init__(self, class_, idx):
        self.dex = class_.dex
        self.class_ = class_
        self.idx = idx
        self._name = None

    def name(self):
        if self._name is None:
            name_bytes = libdex.get_class_method_full_name(self.dex.id, self.class_.id, self.idx)
            self._name = name_bytes.decode('utf8')
        return self._name

    def get_invoked_methods(self):
        ptr = libdex.get_invoked_methods(self.dex.id, self.class_.id, self.idx)
        method_ids = decode_int_array(ptr)

        ret = [ ]
        for method_id in method_ids:
            b = libdex.get_method_full_name(self.dex.id, method_id)
            ret.append(b.decode('utf8'))
        
        return ret

    def get_invoked_methods_libradar(self):
        ptr = libdex.get_invoked_methods_libradar(self.dex.id, self.class_.id, self.idx)
        method_ids = decode_int_array(ptr)

        ret = [ ]
        for method_id in method_ids:
            b = libdex.get_method_full_name(self.dex.id, method_id)
            ret.append(b.decode('utf8'))

        return ret



def test(file_name):
    dex = Dex(file_name)
    for class_ in dex.classes:
        print(class_.name())
        #for method in class_.methods():
        #    print('    ' + method.name())
        #    for im in method.get_invoked_methods():
        #        print('        ' + im)
        f = class_.get_repackage_features()
        print(f)

import sys

if __name__ == '__main__':
    if len(sys.argv) == 1:
        test('classes.dex')
    else:
        test(sys.argv[1])
