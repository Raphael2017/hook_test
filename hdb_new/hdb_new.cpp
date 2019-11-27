#include "stdio.h"
#include <dlfcn.h>
#include "SQLDBC.h"
#include "subhook.h"
#include <string>

#define LOCAL_OUT_PUT       "/home/qwerty/hook_log.txt"
#define REMOTE_OUT_PUT      "/home/he4adm/hook_log.txt"
#define TERMINAL_OUT_PUT    ""

#define SQLDBC_SQLDBC_PreparedStatement_prepare_char_const_SQLDBC_StringEncodingType_Encoding "_ZN6SQLDBC24SQLDBC_PreparedStatement7prepareEPKcN25SQLDBC_StringEncodingType8EncodingE"

extern "C" {

typedef
SQLDBC_Retcode (*PrepareFun)(SQLDBC::SQLDBC_PreparedStatement *self,
                             const char *sql,
                             const SQLDBC_StringEncoding encoding);

PrepareFun prepare_old = nullptr;
SQLDBC_Retcode prepare_new(SQLDBC::SQLDBC_PreparedStatement *self,
                           const char *sql,
                           const SQLDBC_StringEncoding encoding) {
    printf("Call prepare_new sql script: %s\n", sql);
    return prepare_old(self, sql, encoding);
}

void init_log(const std::string& file) {
    if (file.length() > 0) freopen(file.c_str(), "w", stdout);
}

__attribute__((constructor))
void loadMsg() {
    init_log(LOCAL_OUT_PUT);
    printf("So file inject success\n");
    printf("Hook begin\n");
    prepare_old = (PrepareFun)dlsym(RTLD_DEFAULT, SQLDBC_SQLDBC_PreparedStatement_prepare_char_const_SQLDBC_StringEncodingType_Encoding);
    if (prepare_old == nullptr) {
        printf("Hook fail with prepare_old = NIL\n");
        return;
    }
    auto hk = new subhook::Hook((void*)prepare_old, (void*)prepare_new, subhook::HookFlag64BitOffset);
    if (!hk->Install()) {
        printf("Hook fail with hk->Install fail\n");
        return;
    }
    if (hk->GetTrampoline() == nullptr) {
        printf("Hook fail with hk->GetTrampoline() = NIL\n");
        return;
    }
    prepare_old = (PrepareFun)hk->GetTrampoline();
    printf("Hook success\n");
}

}