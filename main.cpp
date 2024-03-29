#include "src/capi.h"
#include "src/ustring.h"

#include <cstdio>

int t;

int main(int argc, char * argv[])
{
    const char * dex_file;
    if (argc == 1)
        dex_file = "classes.dex";
    else
        dex_file = argv[1];

    int dex_id = load_dex(dex_file);

    //puts(get_field_full_name(dex_id, 0));
    //puts(get_field_full_name(dex_id, 1));
    //puts(get_field_full_name(dex_id, 2));

    int class_cnt = get_class_count(dex_id);
    for (int class_id = 0; class_id < class_cnt; ++class_id) {
        //puts(get_class_name(dex_id, class_id));

        int method_cnt = get_methods_count(dex_id, class_id);
        for (int method_idx = 0; method_idx < method_cnt; ++method_idx) {
            //printf("    %s\n", get_class_method_full_name(dex_id, class_id, method_idx));

            auto strings = get_const_strings(dex_id, class_id, method_idx);
            for (int i = 0; i < strings->len; ++i)
                t = strings->data[i];
                //printf("        (%d)\n", strings->data[i]);
                //printf("        (%d) %s\n", strings->data[i], get_string(dex_id, strings->data[i]));

    //        auto invoked = get_invoked_methods(dex_id, class_id, method_idx);
    //        for (int i = 0; i < invoked->len; ++i)
    //            printf("        %s\n", get_method_full_name(dex_id, invoked->data[i]));
        }
    }

    //auto features = get_repackage_features(dex_id, 0);
    //for (int i = 0; i < features->len; ++i)
    //    printf("%d ", features->data[i]);
    //putchar('\n');

    return 0;
}
