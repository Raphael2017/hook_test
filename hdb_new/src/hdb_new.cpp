#include <dlfcn.h>
#include "interface.h"
#include "talk_with_hdb.h"

PrepareFun          prepare_old         = nullptr;
ExecuteItabFun      execute_itab_old    = nullptr;
GetItabReaderFun    get_itab_reader_old = nullptr;
BindParameterFun    bind_parameter_old  = nullptr;
ExecuteFun          execute_old         = nullptr;
GetResultSetFun     get_result_set_old  = nullptr;

#define SQLDBC_SQLDBC_PreparedStatement_prepare_char_const_SQLDBC_StringEncodingType_Encoding   "_ZN6SQLDBC24SQLDBC_PreparedStatement7prepareEPKcN25SQLDBC_StringEncodingType8EncodingE"
#define SQLDBC_SQLDBC_PreparedStatement_executeItab_void_bool                                   "_ZN6SQLDBC24SQLDBC_PreparedStatement11executeItabEPvb"
#define SQLDBC_SQLDBC_PreparedStatement_getItabReader                                           "_ZN6SQLDBC24SQLDBC_PreparedStatement13getItabReaderEv"
#define SQLDBC_SQLDBC_PreparedStatement_bindParameter_unsigned_int_SQLDBC_HostType_void_long_long_long_long_bool "_ZN6SQLDBC24SQLDBC_PreparedStatement13bindParameterEj15SQLDBC_HostTypePvPxxb"
#define SQLDBC_SQLDBC_PreparedStatement_execute                                                 "_ZN6SQLDBC24SQLDBC_PreparedStatement7executeEv"
#define SQLDBC_SQLDBC_Statement_getResultSet                                                    "_ZN6SQLDBC16SQLDBC_Statement12getResultSetEv"
#define ThRqSetCurrentRequest                                                                   (0x540b71)


extern "C" {

    /*
typedef
void (*ThRqSetCurrentRequestFun)(struct REQUEST_BUF *, unsigned int);
ThRqSetCurrentRequestFun thrq_set_current_request_old = nullptr;
void thrq_set_current_request_new(struct REQUEST_BUF *req, unsigned int d) {
    return thrq_set_current_request_old(req, d);
} */

__attribute__((constructor))
void loadMsg() {
    freopen("/home/he4adm/github/log_hook.txt","w",stdout);


    IHook *hook_by_sub = create_hook(IHook::HOOK_BY_SUBHOOK);
    IHook *hook_by_func = create_hook(IHook::HOOK_BY_FUNCHOOK);


    prepare_old = (PrepareFun)hook_by_sub->Install(dlsym(RTLD_DEFAULT, SQLDBC_SQLDBC_PreparedStatement_prepare_char_const_SQLDBC_StringEncodingType_Encoding),
                               (void*)prepare_new);
    if (prepare_old == nullptr) {
        /* todo log */
        return;
    }

    execute_itab_old = (ExecuteItabFun)hook_by_func->Install(dlsym(RTLD_DEFAULT, SQLDBC_SQLDBC_PreparedStatement_executeItab_void_bool),
                                                    (void*)execute_itab_new);
    if (execute_itab_old == nullptr) {
        /* todo log */
        return;
    }

    get_itab_reader_old = (GetItabReaderFun)hook_by_func->Install(dlsym(RTLD_DEFAULT, SQLDBC_SQLDBC_PreparedStatement_getItabReader),
                                                         (void*)get_itab_reader_new);
    if (get_itab_reader_old == nullptr) {
        /* todo log */
        return;
    }

    bind_parameter_old = (BindParameterFun)hook_by_func->Install(dlsym(RTLD_DEFAULT, SQLDBC_SQLDBC_PreparedStatement_bindParameter_unsigned_int_SQLDBC_HostType_void_long_long_long_long_bool),
                                                        (void*)bind_parameter_new);
    if (bind_parameter_old == nullptr) {
        /* todo log */
        return;
    }

    execute_old = (ExecuteFun)hook_by_func->Install(dlsym(RTLD_DEFAULT, SQLDBC_SQLDBC_PreparedStatement_execute),
                                           (void*)execute_new);
    if (execute_old == nullptr) {
        /* todo log */
        return;
    }

    get_result_set_old = (GetResultSetFun)hook_by_func->Install(dlsym(RTLD_DEFAULT, SQLDBC_SQLDBC_Statement_getResultSet),
                                                        (void*)get_result_set_new);
    if (get_result_set_old == nullptr) {
        /* todo log */
        return;
    }

    /*
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
    */
    /* todo log */
}

}
