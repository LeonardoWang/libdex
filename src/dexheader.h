#pragma once

/** @file
 *  Deal with dex file header.
 *  See Android documentation for details.
 *      https://source.android.com/devices/tech/dalvik/dex-format
 **/

#include "reader.h"

//const uint64_t dex_file_magic = 0x003833300a786564;
const uint64_t dex_file_magic = 0x003533300a786564;
const uint32_t dex_header_size = 0x70;
const uint32_t endian_constant = 0x12345678;

struct DexHeader {
    uint64_t magic;  // dex_file_magic
    uint32_t checksum;
    vector<uint8_t> signature;  // size: 20
    uint32_t file_size;
    uint32_t header_size;  // dex_header_size
    uint32_t endian_tag;  // endian_constant
    uint32_t link_size;
    uint32_t link_off;
    uint32_t map_off;
    uint32_t string_ids_size;
    uint32_t string_ids_off;
    uint32_t type_ids_size;
    uint32_t type_ids_off;
    uint32_t proto_ids_size;
    uint32_t proto_ids_off;
    uint32_t field_ids_size;
    uint32_t field_ids_off;
    uint32_t method_ids_size;
    uint32_t method_ids_off;
    uint32_t class_defs_size;
    uint32_t class_defs_off;
    uint32_t data_size;
    uint32_t data_off;
};

inline DexHeader * read_dex_header(Reader & r)
{
    auto hdr = new DexHeader();

    r.seek(0);

    hdr->magic           = r.u64();
    hdr->checksum        = r.u32();
    hdr->signature       = r.bytes(20);
    hdr->file_size       = r.u32();
    hdr->header_size     = r.u32();
    hdr->endian_tag      = r.u32();
    hdr->link_size       = r.u32();
    hdr->link_off        = r.u32();
    hdr->map_off         = r.u32();
    hdr->string_ids_size = r.u32();
    hdr->string_ids_off  = r.u32();
    hdr->type_ids_size   = r.u32();
    hdr->type_ids_off    = r.u32();
    hdr->proto_ids_size  = r.u32();
    hdr->proto_ids_off   = r.u32();
    hdr->field_ids_size  = r.u32();
    hdr->field_ids_off   = r.u32();
    hdr->method_ids_size = r.u32();
    hdr->method_ids_off  = r.u32();
    hdr->class_defs_size = r.u32();
    hdr->class_defs_off  = r.u32();
    hdr->data_size       = r.u32();
    hdr->data_off        = r.u32();

    return hdr;
}
