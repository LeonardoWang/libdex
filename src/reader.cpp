#include "reader.h"

uint16_t Reader::u16()
{
    uint16_t a = data[off];
    uint16_t b = data[off + 1];
    off += 2;
    return (b << 8) + a;
}

uint32_t Reader::u32()
{
    uint32_t a = data[off];
    uint32_t b = data[off + 1];
    uint32_t c = data[off + 2];
    uint32_t d = data[off + 3];
    off += 4;
    return (d << 24) + (c << 16) + (b << 8) + a;
}

uint64_t Reader::u64()
{
    uint64_t a = data[off];
    uint64_t b = data[off + 1];
    uint64_t c = data[off + 2];
    uint64_t d = data[off + 3];
    uint64_t e = data[off + 4];
    uint64_t f = data[off + 5];
    uint64_t g = data[off + 6];
    uint64_t h = data[off + 7];
    off += 8;
    return (h << 56) + (g << 48) + (f << 40) + (e << 32) + (d << 24) + (c << 16) + (b << 8) + a;
}

uint32_t Reader::uleb128()
{
    uint32_t ret = 0;
    int shift = 0;

    while ((data[off] & 0x80) == 0x80) {
        ret += (data[off] & 0x7f) << shift;
        shift += 7;
        off += 1;
    }

    ret += (data[off] & 0x7f) << shift;
    off += 1;
    return ret;
}

String Reader::str()
{
    int utf16_len = uleb128();  // decoded UTF-16 string length, not used
    if (utf16_len == 0) return String();  // FIXME: not sure why this can be 0

    int len = strnlen((const char *) data + off, utf16_len * 4);
    if (len >= utf16_len * 4) {
        printf("%d %d %08x\n", utf16_len, len, off);
        throw String::EncodingError("incorrect utf-16 length", data + off, len);
    }

    String ret(data + off, len, String::MUTF8);
    off += len;
    return ret;
}

vector<String> Reader::type_list(const vector<String> & types)
{
    uint32_t size = u32();
    vector<String> ret(size);
    for (unsigned i = 0; i < size; ++i)
        ret[i] = types[u16()];
    return ret;
}

vector<uint8_t> Reader::bytes(int n)
{
    vector<uint8_t> ret(n);
    for (int i = 0; i < n; ++i)
        ret[i] = data[off + i];
    off += n;
    return ret;
}

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int Reader::open_file(const char * file_name)  // FIXME: POSIX only
{
    return open(file_name, O_RDONLY);
}

void Reader::close_file(int fd)
{
    if (fd > 0) close(fd);
}

Reader Reader::from_fd(int fd)
{
    Reader ret(nullptr, -1);
    if (fd == -1) return ret;
    struct stat st;
    fstat(fd, & st);
    if (st.st_size == 0) return ret;
    auto bytes = mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (bytes == nullptr) return ret;

    ret.data = (const uint8_t *) bytes;
    ret.off = 0;
    return ret;
}
