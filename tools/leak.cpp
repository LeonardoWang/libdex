#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>

#include "inst.h"
#include "dex.h"
#include "inst_enum.h"

using namespace std;

Dex * dex = nullptr;

const int RegNum = 5;

bool bug[30] = { };

bool use_webkit = false;
bool remove_js_interface = false;
bool use_fragment = false;
bool safe_fragment = false;
bool safe_password = false;

string to_string(String const& s) {
    if (s.empty()) return string();
    const char *cstr = s.c_str();
    string str = cstr;
    delete [] cstr;
    return str;
}

string get_str(int idx)
{
    if (0 <= idx && idx < (int) dex->strings.size())
        return to_string(dex->strings[idx]);
    else
        return string();
}

void analyze(const EncodedMethod & method, const Class &class_)
{
    if (to_string(class_.name) == "Landroid.preference.PreferenceActivity;")
        use_fragment = true;

    bool const_assigning = false;

    int reg[RegNum] = { };

    bool may_throw = false;
    bool use_intent = false;
    bool safe_intent = false;
    bool uri_intent = false;
    bool safe_uri = false;

    for (const Inst & i : method) {
        string m;

        if (i.is_invoke() && i.invoke_target() < dex->methods.size()) {
            may_throw = true;
            m = to_string(dex->methods[i.invoke_target()].full_name());

            if (m == "Landroid/content/Context;->getSharedPreferences") {
                if (const_assigning && (reg[2] == 1 || reg[2] == 2 || reg[2] == 3)) {
                    bug[1] = true;
                }

            } else if (m == "Landroid/webkit/SslErrorHandler;->proceed") {
                bug[2] = true;

            } else if (m == "Landroid/webkit/WebView;->getSettings") {
                use_webkit = true;

            } else if (m == "Landroid/webkit/WebView;->removeJavascriptInterface") {
                if (const_assigning) {
                    string s = get_str(reg[1]);
                    if (s == "searchBoxJavaBridge_" || s == "accessibility" || s == "accessibilityTraversal")
                        remove_js_interface = true;
                }

            } else if (m == "Landroid/content/Context;->openFileOutput") {
                if (const_assigning && (reg[2] == 1 || reg[2] == 2 || reg[2] == 3))
                    bug[5] = true;

            } else if (m == "Ljava/security/SecureRandom;-><init>" || m == "Ljava/security/SecureRandom;->setSeed") {
                if (const_assigning)
                    bug[6] =true;

            } else if (m == "Ljavax/crypto/Cipher;->getInstance") {
                if (const_assigning) {
                    string s = get_str(reg[1]);
                    if (s == "AES" || s == "DES" || s == "AES/ECB" || s == "DES/ECB" || s == "DESede/ECB")
                        bug[8] = true;
                }

            } else if (m == "Ljavax/crypto/spec/IvParameterSpec;-><init>") {
                if (const_assigning)
                    bug[9] = true;

            } else if (m == "Lorg/apache/http/conn/ssl;->SSLSocketFactory") {

            } else if (m == "Landroid/content/Intent;-><init>") {
                use_intent = true;

                const auto & p = dex->protos[dex->methods[i.invoke_target()].proto_id].params;
                if (p.size() == 2 && to_string(p[1]) == "Ljava/lang/Class;")
                    safe_intent = true;

            } else if (m == "Landroid/content/Intent;->setAction") {
                use_intent = true;

            } else if (m == "Landroid/content/Intent;->setComponent" || m == "Landroid/content/Intent;->setClass" || m == "Landroid/content/Intent;->setClassName" || m == "Landroid/content/Intent;->setPackage") {
                safe_intent = true;

            } else if (m == "Landroid/preference/PreferenceActivity;->isValidFragment") {
                safe_fragment = true;

            } else if (m == "Landroid/content/Intent;->parseUri") {
                uri_intent = true;

            } else if (m == "Landroid/content/Intent;->addCategory") {
                safe_uri = true;

            } else if (m == "Landroid/webkit/WebSettings;->setSavePassword") {
                if (const_assigning && reg[1] == 1)
                    safe_password = true;

            } else if (m == "Ljavax/crypto/spec/SecretKeySpec-><init>") {
                if (const_assigning)
                    bug[22] = true;
            }
        }

        if (i.op() == Throw)
            may_throw = true;

        if (i.is_const()) {
            const_assigning = true;
            if (i.get_a() < RegNum) reg[i.get_a()] = i.get_b();

        } else if (const_assigning) {
            const_assigning = false;

            if (i.is_invoke() && m == "Ljava/lang/String;->getBytes")
                const_assigning = true;
            if (i.op() == MoveResult || i.op() == MoveResultWide || i.op() == MoveResultObject)
                const_assigning = true;

            if (! const_assigning)
                for (int i = 0; i < RegNum; ++i)
                    reg[i] = -1;
        }
    }

    if (to_string(dex->methods[method.method_id].name) == "checkServerTrusted" && ! may_throw)
        bug[7] = true;

    if (use_intent && ! safe_intent) {
        bug[12] = true;
    }

    if (uri_intent && ! safe_uri)
        bug[15] = true;

    //cerr << "done\n";
}

int main(int argc, char * argv[])
{
    const char * dex_file = argv[1];
    //dex_file = "classes.dex";

    Reader r = Reader::open_file(dex_file);
    dex = new Dex(r);

    for (const auto & class_ : dex->classes) {
        for (const auto & method : class_.direct_methods)
            if (method.insts != nullptr)
                analyze(method, class_);
        for (const auto & method : class_.virtual_methods)
            if (method.insts != nullptr)
                analyze(method, class_);
    }

    if (use_webkit && ! remove_js_interface)
        bug[3] = true;

    if (use_fragment && ! safe_fragment)
        bug[14] = true;

    if (use_webkit && ! safe_password)
        bug[18] = true;

    int mid[] = { 1, 2, 3, 5, 7, 11, 13, 15, 20 };
    int low[] = { 6, 8, 9, 12, 16, 17, 18, 19, 21, 22, 23 };

    double score = 100;

    vector<int> ans;
    for (int i : mid) if (bug[i]) { ans.push_back(i); score -= 7; }
    for (int i : low) if (bug[i]) { ans.push_back(i); score -= 3; }
    if (score < 0) score = 0;

    cout << "{\"score\":" << score << ",\"bugs\":[";
    for (int i = 0; i < ((int) ans.size()) - 1; ++i)
        cout << ans[i] << ',';
    if (ans.size() >= 1)
        cout << ans.back();
    cout << "]}\n";

    return 0;
}
