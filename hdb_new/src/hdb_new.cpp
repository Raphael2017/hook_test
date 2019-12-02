#include "stdio.h"
#include <dlfcn.h>
#include "SQLDBC.h"
#include <string>
#include "hook.h"
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define LOCAL_OUT_PUT       "/home/qwerty/hook_log.txt"
#define REMOTE_OUT_PUT      "/home/he4adm/hook_log.txt"
#define TERMINAL_OUT_PUT    ""

#define SQLDBC_SQLDBC_PreparedStatement_prepare_char_const_SQLDBC_StringEncodingType_Encoding   "_ZN6SQLDBC24SQLDBC_PreparedStatement7prepareEPKcN25SQLDBC_StringEncodingType8EncodingE"
#define SQLDBC_SQLDBC_PreparedStatement_executeItab_void_bool                                   "_ZN6SQLDBC24SQLDBC_PreparedStatement11executeItabEPvb"
#define SQLDBC_SQLDBC_PreparedStatement_getItabReader                                           "_ZN6SQLDBC24SQLDBC_PreparedStatement13getItabReaderEv"
#define SQLDBC_SQLDBC_PreparedStatement_bindParameter_unsigned_int_SQLDBC_HostType_void_long_long_long_long_bool "_ZN6SQLDBC24SQLDBC_PreparedStatement13bindParameterEj15SQLDBC_HostTypePvPxxb"
#define SQLDBC_SQLDBC_PreparedStatement_execute                                                 "_ZN6SQLDBC24SQLDBC_PreparedStatement7executeEv"
#define SQLDBC_SQLDBC_Statement_getResultSet                                                    "_ZN6SQLDBC16SQLDBC_Statement12getResultSetEv"
#define ThRqSetCurrentRequest                                                                   (0x540b71)

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
SQLDBC_Retcode (*ExecuteItabFun)(SQLDBC::SQLDBC_PreparedStatement *,
                              void*, bool);

ExecuteItabFun execute_itab_old = nullptr;
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
///////////////////////////////////////////////////////////////////
typedef
SQLDBC_Retcode (*ExecuteFun)(SQLDBC::SQLDBC_PreparedStatement *);
ExecuteFun execute_old = nullptr;
SQLDBC_Retcode execute_new(SQLDBC::SQLDBC_PreparedStatement *self) {
    printf("Call execute_new\n");
    return execute_old(self);
}
/////////////////////////////////////////////////////////////////////////////

typedef
SQLDBC::SQLDBC_ResultSet *(*GetResultSetFun)(SQLDBC::SQLDBC_PreparedStatement *);
GetResultSetFun get_result_set_old = nullptr;
SQLDBC::SQLDBC_ResultSet *get_result_set_new(SQLDBC::SQLDBC_PreparedStatement *self) {
    printf("Call get_result_set_new\n");
    return get_result_set_old(self);
}
/////////////////////////////////////////////////////////////////////////////
typedef
void (*ThRqSetCurrentRequestFun)(struct REQUEST_BUF *, unsigned int);
ThRqSetCurrentRequestFun thrq_set_current_request_old = nullptr;
void thrq_set_current_request_new(struct REQUEST_BUF *req, unsigned int d) {
    printf("Call thrq_set_current_request_new\n");
    return thrq_set_current_request_old(req, d);
}


void init_log(const std::string& file) {
    if (file.length() > 0) freopen(file.c_str(), "w", stdout);
}

__attribute__((constructor))
void loadMsg() {
    init_log(REMOTE_OUT_PUT);
    printf("So file inject success\n");
    printf("Hook begin\n");

    prepare_old = (PrepareFun)install_hook(dlsym(RTLD_DEFAULT, SQLDBC_SQLDBC_PreparedStatement_prepare_char_const_SQLDBC_StringEncodingType_Encoding),
                               (void*)prepare_new, HOOK_BY_SUBHOOK);
    if (prepare_old == nullptr) {
        printf("Hook fail with prepare = NIL\n");
        return;
    }

    execute_itab_old = (ExecuteItabFun)install_hook(dlsym(RTLD_DEFAULT, SQLDBC_SQLDBC_PreparedStatement_executeItab_void_bool),
                                                    (void*)execute_itab_new, HOOK_BY_FUNCHOOK);
    if (execute_itab_old == nullptr) {
        printf("Hook fail with execute_itab = NIL\n");
        return;
    }

    get_itab_reader_old = (GetItabReaderFun)install_hook(dlsym(RTLD_DEFAULT, SQLDBC_SQLDBC_PreparedStatement_getItabReader),
                                                         (void*)get_itab_reader_new, HOOK_BY_FUNCHOOK);
    if (get_itab_reader_old == nullptr) {
        printf("Hook fail with get_itab_reader = NIL\n");
        return;
    }

    bind_parameter_old = (BindParameterFun)install_hook(dlsym(RTLD_DEFAULT, SQLDBC_SQLDBC_PreparedStatement_bindParameter_unsigned_int_SQLDBC_HostType_void_long_long_long_long_bool),
                                                        (void*)bind_parameter_new, HOOK_BY_FUNCHOOK);
    if (bind_parameter_old == nullptr) {
        printf("Hook fail with bind_parameter = NIL\n");
        return;
    }

    execute_old = (ExecuteFun)install_hook(dlsym(RTLD_DEFAULT, SQLDBC_SQLDBC_PreparedStatement_execute),
                                           (void*)execute_new, HOOK_BY_FUNCHOOK);
    if (execute_old == nullptr) {
        printf("Hook fail with execute = NIL\n");
        return;
    }

    get_result_set_old = (GetResultSetFun)install_hook(dlsym(RTLD_DEFAULT, SQLDBC_SQLDBC_Statement_getResultSet),
                                                        (void*)get_result_set_new, HOOK_BY_FUNCHOOK);
    if (get_result_set_old == nullptr) {
        printf("Hook fail with get_result_set = NIL\n");
        return;
    }

    void *tmp = nullptr;
    if ((tmp = dlopen(nullptr, RTLD_NOW)) != nullptr) {
        printf("disp+work: %p\n", tmp);
        thrq_set_current_request_old = (ThRqSetCurrentRequestFun)install_hook((unsigned char*)tmp+ThRqSetCurrentRequest,
                                                                              (void*)thrq_set_current_request_new, HOOK_BY_FUNCHOOK);
    }
    else {
        printf("Hook fail with dlopen(disp+work, RTLD_NOW) = NIL\n");
        return;
    }

    printf("Hook success\n");
}



}