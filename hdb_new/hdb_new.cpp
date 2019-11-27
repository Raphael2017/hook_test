#include "stdio.h"
#include <dlfcn.h>
#include "SQLDBC.h"
#include "subhook.h"
#include <string>

#define LOCAL_OUT_PUT       "/home/qwerty/hook_log.txt"
#define REMOTE_OUT_PUT      "/home/he4adm/hook_log.txt"
#define TERMINAL_OUT_PUT    ""

#define SQLDBC_SQLDBC_PreparedStatement_prepare_char_const_SQLDBC_StringEncodingType_Encoding   "_ZN6SQLDBC24SQLDBC_PreparedStatement7prepareEPKcN25SQLDBC_StringEncodingType8EncodingE"
#define SQLDBC_SQLDBC_PreparedStatement_executeItab_void_bool                                   "_ZN6SQLDBC24SQLDBC_PreparedStatement11executeItabEPvb"
#define SQLDBC_SQLDBC_PreparedStatement_getItabReader                                           "_ZN6SQLDBC24SQLDBC_PreparedStatement13getItabReaderEv"
#define SQLDBC_SQLDBC_PreparedStatement_bindParameter_unsigned_int_SQLDBC_HostType_void_long_long_long_long_bool "_ZN6SQLDBC24SQLDBC_PreparedStatement13bindParameterEj15SQLDBC_HostTypePvPxxb"

extern "C" {
////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////
typedef
SQLDBC_Retcode (*ExecuteItab)(SQLDBC::SQLDBC_PreparedStatement *,
                              void*, bool);

ExecuteItab execute_itab_old = nullptr;
SQLDBC_Retcode execute_itab_new(SQLDBC::SQLDBC_PreparedStatement *self,
                                void *p, bool b) {
    printf("Call execute_itab_new\n");
    return execute_itab_old(self, p, b);
}
////////////////////////////////////////////////////////////////////
typedef
void* (*GetItabReaderFun)(SQLDBC::SQLDBC_PreparedStatement *);

GetItabReaderFun get_itab_reader_old = nullptr;
void* get_itab_reader_new(SQLDBC::SQLDBC_PreparedStatement *self) {
    printf("Call get_itab_reader_new\n");
    return get_itab_reader_old(self);
}
////////////////////////////////////////////////////////////////////
typedef
SQLDBC_Retcode (*BindParameterFun) (SQLDBC::SQLDBC_PreparedStatement *,
                              const SQLDBC_UInt2     Index,
                              const SQLDBC_HostType  Type,
                              void                  *paramAddr,
                              SQLDBC_Length         *LengthIndicator,
                              const SQLDBC_Length    Size,
                              const SQLDBC_Bool      Terminate);
BindParameterFun bind_parameter_old = nullptr;
SQLDBC_Retcode bind_parameter_new(SQLDBC::SQLDBC_PreparedStatement *self,
                                    const SQLDBC_UInt4     Index,
                                    const SQLDBC_HostType  Type,
                                    void                  *paramAddr,
                                    SQLDBC_Length         *LengthIndicator,
                                    const SQLDBC_Length    Size,
                                    const SQLDBC_Bool      Terminate) {
    printf("Call bind_parameter_new index: %d\n", Index);
    return bind_parameter_old(self, Index, Type, paramAddr, LengthIndicator, Size, Terminate);
}


void init_log(const std::string& file) {
    if (file.length() > 0) freopen(file.c_str(), "w", stdout);
}

__attribute__((constructor))
void loadMsg() {
    init_log(TERMINAL_OUT_PUT);
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

    execute_itab_old = (ExecuteItab)dlsym(RTLD_DEFAULT, SQLDBC_SQLDBC_PreparedStatement_executeItab_void_bool);
    if (execute_itab_old == nullptr) {
        printf("Hook fail with execute_itab_old = NIL\n");
        return;
    }
    auto hk1 = new subhook::Hook((void*)execute_itab_old, (void*)execute_itab_new, subhook::HookFlag64BitOffset);
    if (!hk1->Install()) {
        printf("Hook fail with hk->Install fail\n");
        return;
    }
    if (hk1->GetTrampoline() == nullptr) {
        printf("Hook fail with hk->GetTrampoline() = NIL\n");
        return;
    }
    execute_itab_old = (ExecuteItab)hk1->GetTrampoline();

    get_itab_reader_old = (GetItabReaderFun)dlsym(RTLD_DEFAULT, SQLDBC_SQLDBC_PreparedStatement_getItabReader);
    if (get_itab_reader_old == nullptr) {
        printf("Hook fail with get_itab_reader_old = NIL\n");
        return;
    }
    auto hk2 = new subhook::Hook((void*)get_itab_reader_old, (void*)get_itab_reader_new, subhook::HookFlag64BitOffset);
    if (!hk2->Install()) {
        printf("Hook fail with hk->Install fail\n");
        return;
    }
    if (hk2->GetTrampoline() == nullptr) {
        printf("Hook fail with hk->GetTrampoline() = NIL\n");
        return;
    }
    get_itab_reader_old = (GetItabReaderFun)hk2->GetTrampoline();

    bind_parameter_old = (BindParameterFun)dlsym(RTLD_DEFAULT, SQLDBC_SQLDBC_PreparedStatement_bindParameter_unsigned_int_SQLDBC_HostType_void_long_long_long_long_bool);
    if (bind_parameter_old == nullptr) {
        printf("Hook fail with bind_parameter_old = NIL\n");
        return;
    }
    auto hk3 = new subhook::Hook((void*)bind_parameter_old, (void*)bind_parameter_new, subhook::HookFlag64BitOffset);
    if (!hk3->Install()) {
        printf("Hook fail with hk->Install fail\n");
        return;
    }
    if (hk3->GetTrampoline() == nullptr) {
        printf("Hook fail with hk->GetTrampoline() = NIL\n");
        return;
    }
    bind_parameter_old = (BindParameterFun)hk3->GetTrampoline();


    printf("Hook success\n");
}



}