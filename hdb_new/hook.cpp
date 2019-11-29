#include "hook.h"
#include "subhook.h"
#include "funchook.h"
#include "assert.h"

void *install_hook_by_subhook(void *oldf, void *newf);
void *install_hook_by_funchook(void *oldf, void *newf);

void *install_hook(void *oldf, void *newf, HOOK_TYPE hook_type) {
    switch (hook_type) {
        case HOOK_BY_SUBHOOK: {
            return install_hook_by_subhook(oldf, newf);
        } break;
        case HOOK_BY_FUNCHOOK: {
            return install_hook_by_funchook(oldf, newf);
        } break;
        default: { assert(false); return nullptr; }
    }
}

void *install_hook_by_subhook(void *oldf, void *newf) {
    auto hk = new subhook::Hook((void*)oldf, (void*)newf, subhook::HookFlag64BitOffset);
    if (!hk->Install()) {
        assert(false);
        return nullptr;
    }
    if (hk->GetTrampoline() == nullptr) {
        assert(false);
        return nullptr;
    }
    return hk->GetTrampoline();
}

void *install_hook_by_funchook(void *oldf, void *newf) {
    funchook_t *funchook = funchook_create();
    int rv = funchook_prepare(funchook, (void**)&oldf, newf);
    if (rv != 0) {
        assert(false);
        return nullptr;
    }
    rv = funchook_install(funchook, 0);
    if (rv != 0) {
        assert(false);
        return nullptr;
    }
    return oldf;
}
