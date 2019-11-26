#include "stdio.h"
#define SUBHOOK_X86_64
#include "subhook.h"
#include <dlfcn.h>
#include "SQLDBC.h"

subhook::Hook *hk = nullptr;



typedef SQLDBC_Retcode (*ConnectFun)( SQLDBC::SQLDBC_Connection *self,
        const char *servernode,
        const char *serverdb,
        const char *username,
        const char *password);

typedef SQLDBC::SQLDBC_PreparedStatement *(*CreatePrepareStatementFun)(SQLDBC::SQLDBC_Connection *self);

SQLDBC_Retcode connect_new( SQLDBC::SQLDBC_Connection *self,
                              const char *servernode,
                              const char *serverdb,
                              const char *username,
                              const char *password) {
    printf("self: %p, servernode: %s, serverdb: %s, username: %s, password: %s\n", self, servernode, serverdb, username, password);
    printf("GetTrampoline: %p\n", hk->GetTrampoline());
    return ((ConnectFun)hk->GetTrampoline())(self, servernode, serverdb, username, password);
}

SQLDBC::SQLDBC_PreparedStatement *create_prepare_statement_new(SQLDBC::SQLDBC_Connection *self) {
    printf("self: %p\n", self);
    printf("GetTrampoline: %p\n", hk->GetTrampoline());
    return ((CreatePrepareStatementFun)hk->GetTrampoline())(self);
}

extern "C" {

void *p_test = nullptr;
SQLDBC_Retcode _ZN6SQLDBC17SQLDBC_Connection7connectEPKcS2_S2_S2_( SQLDBC::SQLDBC_Connection *self,
                            const char *servernode,
                            const char *serverdb,
                            const char *username,
                            const char *password) {
    printf("self: %p, servernode: %s, serverdb: %s, username: %s, password: %s\n", self, servernode, serverdb, username, password);
    //printf("GetTrampoline: %p\n", hk->GetTrampoline());

    void *old = dlsym(RTLD_NEXT, "_ZN6SQLDBC17SQLDBC_Connection7connectEPKcS2_S2_S2_");
    void *newp = dlsym(RTLD_DEFAULT, "_ZN6SQLDBC17SQLDBC_Connection7connectEPKcS2_S2_S2_");
    printf("old: %p, new: %p\n", old, newp);
    //old = nullptr;
    return ((ConnectFun)old)(self, servernode, serverdb, username, password);
}


__attribute__((constructor))
void loadMsg() {
    //void *h = dlopen("/home/qwerty/github/hook_test/hdb/libSQLDBCHDB.so", RTLD_LAZY);

    void *h = dlopen("/home/he4adm/libSQLDBCHDB.so", RTLD_LAZY);
    return;

    if (h == NULL)
        return;
    //void *p = dlsym(h, "_ZN6SQLDBC17SQLDBC_Connection23createPreparedStatementEv");
    void *p = dlsym(h, "_ZN6SQLDBC17SQLDBC_Connection7connectEPKcS2_S2_S2_");
    p_test = p;
    printf("old:%p, new: %p\n", p_test, _ZN6SQLDBC17SQLDBC_Connection7connectEPKcS2_S2_S2_);
    /*
    if (p != NULL) {
        hk = new subhook::Hook(p, (void*)&connect_new, subhook::HookFlag64BitOffset);
        printf("GetTrampoline: %p\n", hk->GetTrampoline());
        if (hk->Install()) {

        } else {
            printf("hook install fail\n");
        }
    }*/
}

}