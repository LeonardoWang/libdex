#include "dex.h"
#include "dexheader.h"

#include <cstdio>

const uint32_t no_index = 0xffffffff;

void EncodedMethod::load_detail(Reader r)
{
    reg_size = r.u16();
    ins_size = r.u16();
    outs_size = r.u16();
    tries_size = r.u16();
    debug_info_off = r.u32();
    inst_size = r.u32();
    insts = r.ptr();

    if (! r.in_range(inst_size)) {
        insts = nullptr;
        inst_size = 0;
        fputs("method size out of range\n", stderr);
    }
}

void EncodedMethod::clear()
{
    reg_size = 0;
    ins_size = 0;
    outs_size = 0;
    tries_size = 0;
    debug_info_off = 0;
    inst_size = 0;
    insts = nullptr;
}

void Dex::init()
{
    /* header */
    hdr = read_dex_header(r);

    if (hdr->header_size != dex_header_size) throw FormatError("Incorrect header size");
    if (hdr->endian_tag != endian_constant) throw FormatError("Incorrect endian");
    if (! r.is_valid()) throw FormatError("Incomplete header");
    // TODO: check magic and checksum


    /* string ids */
    r.seek(hdr->string_ids_off);
    strings.resize(hdr->string_ids_size);
    for (unsigned i = 0; i < hdr->string_ids_size; ++i) {
        uint32_t str_data_off = r.u32();
        strings[i] = r.at(str_data_off).str();
    }

    /* type ids */
    r.seek(hdr->type_ids_off);
    types.resize(hdr->type_ids_size);
    for (unsigned i = 0; i < hdr->type_ids_size; ++i)
        types[i] = strings[r.u32()];

    /* proto ids */
    r.seek(hdr->proto_ids_off);
    protos.resize(hdr->proto_ids_size);
    for (unsigned i = 0; i < hdr->proto_ids_size; ++i) {
        protos[i].shorty = strings[r.u32()];
        protos[i].ret_type = types[r.u32()];

        uint32_t params_off = r.u32();
        if (params_off != 0)
            protos[i].params = r.at(params_off).type_list(types);
    }

    /* field ids */
    r.seek(hdr->field_ids_off);
    fields.resize(hdr->field_ids_size);
    for (unsigned i = 0; i < hdr->field_ids_size; ++i) {
        fields[i].class_ = types[r.u16()];
        fields[i].type = types[r.u16()];
        fields[i].name = strings[r.u32()];
    }

    /* method ids */
    r.seek(hdr->method_ids_off);
    methods.resize(hdr->method_ids_size);
    for (unsigned i = 0; i < hdr->method_ids_size; ++i) {
        methods[i].class_ = types[r.u16()];
        methods[i].proto_id = r.u16();
        methods[i].name = strings[r.u32()];
    }

    /* class defs */
    r.seek(hdr->class_defs_off);
    classes.resize(hdr->class_defs_size);
    for (unsigned i = 0; i < hdr->class_defs_size; ++i) {
        classes[i].name = types[r.u32()];
        classes[i].access_flags = r.u32();

        uint32_t super_idx = r.u32();
        if (super_idx != no_index)
            classes[i].super = types[super_idx];

        uint32_t inter_off = r.u32();
        if (inter_off != 0)
            classes[i].interfaces = r.at(inter_off).type_list(types);

        uint32_t src_file_idx = r.u32();
        if (src_file_idx != no_index)
            classes[i].src_file = strings[src_file_idx];

        classes[i].annotations_off = r.u32();

        uint32_t class_data_off = r.u32();
        if (class_data_off != 0) {
            Reader data_r = r.at(class_data_off);
            uint32_t static_fields_size = data_r.uleb128();
            uint32_t instance_fields_size = data_r.uleb128();
            uint32_t direct_methods_size = data_r.uleb128();
            uint32_t virtual_methods_size = data_r.uleb128();

            classes[i].static_fields.resize(static_fields_size);
            for (unsigned j = 0; j < static_fields_size; ++j) {
                classes[i].static_fields[j].field_id = data_r.uleb128();
                classes[i].static_fields[j].access_flags = data_r.uleb128();
            }

            classes[i].instance_fields.resize(instance_fields_size);
            for (unsigned j = 0; j < instance_fields_size; ++j) {
                classes[i].instance_fields[j].field_id = data_r.uleb128();
                classes[i].instance_fields[j].access_flags = data_r.uleb128();
            }

            classes[i].direct_methods.resize(direct_methods_size);
            int last_method_idx = 0;
            for (unsigned j = 0; j < direct_methods_size; ++j) {
                last_method_idx += data_r.uleb128();
                classes[i].direct_methods[j].method_id = last_method_idx;
                classes[i].direct_methods[j].access_flags = data_r.uleb128();
                uint32_t code_off = data_r.uleb128();
                if (code_off != 0)
                    classes[i].direct_methods[j].load_detail(r.at(code_off));
                else
                    classes[i].direct_methods[j].clear();
            }

            classes[i].virtual_methods.resize(virtual_methods_size);
            last_method_idx = 0;
            for (unsigned j = 0; j < virtual_methods_size; ++j) {
                last_method_idx += data_r.uleb128();
                classes[i].virtual_methods[j].method_id = last_method_idx;
                classes[i].virtual_methods[j].access_flags = data_r.uleb128();
                uint32_t code_off = data_r.uleb128();
                if (code_off != 0)
                    classes[i].virtual_methods[j].load_detail(r.at(code_off));
                else
                    classes[i].virtual_methods[j].clear();
            }
        }

        classes[i].static_values_off = r.u32();
    }

    // call site ids
    // method handles
    // data
    // link data
}

Dex::~Dex()
{
    delete hdr;
    Reader::close_file(fd);
}
