#ifndef HOOK_H
#define HOOK_H

enum HOOK_TYPE { HOOK_BY_SUBHOOK, HOOK_BY_FUNCHOOK };

void *install_hook(void *oldf, void *newf, HOOK_TYPE hook_type);

#endif