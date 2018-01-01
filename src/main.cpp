#include "capi.h"
#include "ustring.h"

#include <cstdio>

int main(int , char * argv[])
{
    String path = argv[0];
    int p = path.rfind('/');
    path = path.slice(0, p + 1) + "instruments.txt";

    load_inst_conf(path.cstr());
    int dex_id = load_dex("classes.dex");

    int class_cnt = get_class_count(dex_id);
    for (int class_id = 0; class_id < class_cnt; ++class_id) {
        puts(get_class_name(dex_id, class_id));

        int method_cnt = get_methods_count(dex_id, class_id);
        for (int method_idx = 0; method_idx < method_cnt; ++method_idx) {
            printf("    %s\n", get_class_method_full_name(dex_id, class_id, method_idx));

            auto invoked = get_invoked_methods(dex_id, class_id, method_idx);
            for (int i = 0; i < invoked->len; ++i)
                printf("        %s\n", get_method_full_name(dex_id, invoked->data[i]));
        }
    }

    return 0;
}
