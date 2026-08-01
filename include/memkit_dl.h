#ifndef MEMKIT_DL_H
#define MEMKIT_DL_H

#include "memkit_common.h"

// ============================================================================
// DL HELPERS: Library loading and symbol resolution
// ============================================================================

/**
 * Open a library handle for symbol resolution (ShadowHook's internal loader)
 */
void *memkit_dlopen(const char *lib_name);

/**
 * Close a library handle
 */
void memkit_dlclose(void *handle);

/**
 * Resolve a symbol from a library handle (tries .dynsym then .symtab)
 */
void *memkit_dlsym(void *handle, const char *sym_name);

/**
 * Resolve a symbol from .dynsym section only (faster)
 */
void *memkit_dlsym_dynsym(void *handle, const char *sym_name);

/**
 * Resolve a symbol from .symtab section only (debug/stripped symbols, slower)
 */
void *memkit_dlsym_symtab(void *handle, const char *sym_name);

// ============================================================================
// DL INIT/FINI CALLBACKS
// ============================================================================

typedef shadowhook_dl_info_t MemKitDlInfo;
typedef void (*MemKitDlInitCallback)(struct dl_phdr_info *info, size_t size, void *data);
typedef void (*MemKitDlFiniCallback)(struct dl_phdr_info *info, size_t size, void *data);

int memkit_register_dl_init_callback(MemKitDlInitCallback pre, MemKitDlInitCallback post, void *data);
int memkit_unregister_dl_init_callback(MemKitDlInitCallback pre, MemKitDlInitCallback post, void *data);
int memkit_register_dl_fini_callback(MemKitDlFiniCallback pre, MemKitDlFiniCallback post, void *data);
int memkit_unregister_dl_fini_callback(MemKitDlFiniCallback pre, MemKitDlFiniCallback post, void *data);

#endif // MEMKIT_DL_H
