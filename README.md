## 编译

    $ make

只支持 Linux 系统，Windows 需要修改 `Reader::open_file()` 中的 `mmap` 部分，Mac 需要处理端序问题。

## 分发

需要将 `libdex.so`、`instruments.txt`、`dex.py`、`lx.py` 放在同一目录下（或者将 `libdex.so` 放进 `LD_LIBRARY_PATH`，将 `lx.py` 放进 `PYTHONPATH`）。

## 测试

将格式正确的 `classes.dex` 文件置于程序目录，运行

    $ python3 dex.py

或

    $ LD_LIBRARY_PATH=. ./test

理论上将会列出每一个类中定义的方法和每一个方法中调用的方法。

## 使用

参考 `dex.py` 中的 `test()` 函数。
