#include "capi.h"
#include "ustring.h"

#include <cstdio>

int main(int argc, char * argv[])
{
    const char * dex_file;
    if (argc == 1)
        dex_file = "classes.dex";
    else
        dex_file = argv[1];

    int dex_id = load_dex(dex_file);

    //int class_cnt = get_class_count(dex_id);
    //for (int class_id = 0; class_id < class_cnt; ++class_id) {
    //    puts(get_class_name(dex_id, class_id));

    //    int method_cnt = get_methods_count(dex_id, class_id);
    //    for (int method_idx = 0; method_idx < method_cnt; ++method_idx) {
    //        printf("    %s\n", get_class_method_full_name(dex_id, class_id, method_idx));

    //        auto invoked = get_invoked_methods(dex_id, class_id, method_idx);
    //        for (int i = 0; i < invoked->len; ++i)
    //            printf("        %s\n", get_method_full_name(dex_id, invoked->data[i]));
    //    }
    //}

    auto features = get_repackage_features(dex_id, 0);
    for (int i = 0; i < features->len; ++i)
        printf("%d ", features->data[i]);
    putchar('\n');

    return 0;
}
