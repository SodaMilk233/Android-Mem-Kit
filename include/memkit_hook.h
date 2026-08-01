#ifndef MEMKIT_HOOK_H
#define MEMKIT_HOOK_H

#include "memkit_common.h"

// ============================================================================
// HOOKING API (ShadowHook)
// ============================================================================

/**
 * Initialize ShadowHook (call once at startup)
 * @param mode SHADOWHOOK_MODE_UNIQUE, SHARED, or MULTI
 * @param debuggable Enable debug logging
 * @return 0 on success, negative value on failure
 */
int memkit_hook_init(int mode, bool debuggable);

/**
 * Hook a function at target address
 * FIXED: Returns stub handle for later unhooking
 * FIXED: Uses out parameter for original function
 *
 * @param target_addr Address of target function
 * @param replace_func Pointer to replacement function (proxy)
 * @param out_orig_func Output: pointer to original function
 * @return Stub handle (for unhook) or NULL on failure
 */
void* memkit_hook(uintptr_t target_addr, void* replace_func, void** out_orig_func);

/**
 * Unhook a function using stub handle
 * @param stub Handle returned by memkit_hook
 */
void memkit_unhook(void* stub);

/**
 * Hook by symbol name (convenience wrapper)
 * @param lib_name Library name (e.g., "libil2cpp.so")
 * @param symbol_name Symbol to hook (e.g., "il2cpp_thread_attach")
 * @param replace_func Pointer to replacement function
 * @param out_orig_func Output: pointer to original function
 * @return Stub handle or NULL on failure
 */
void* memkit_hook_by_symbol(const char* lib_name, const char* symbol_name, void* replace_func, void** out_orig_func);

/**
 * Hook by symbol address (already resolved pointer)
 * @param sym_addr Pointer to target function symbol
 * @param new_addr Replacement function pointer
 * @param orig_addr Output: original function pointer
 * @return Stub handle or NULL on failure
 */
void *memkit_hook_sym_addr(void *sym_addr, void *new_addr, void **orig_addr);

// ============================================================================
// HOOKING: MODE CONSTANTS
// ============================================================================

// Mode constants are provided by ShadowHook (SHADOWHOOK_MODE_SHARED, etc.)
// Use MEMKIT_IS_SHARED_MODE, MEMKIT_IS_UNIQUE_MODE, MEMKIT_IS_MULTI_MODE macros above.

// ============================================================================
// HOOKING: PROXY & STACK MANAGEMENT (Macros)
// ============================================================================

/* Call the previous function in the proxy chain (MULTI mode only).
 * @param func       Your proxy function pointer
 * @param func_sig   Function signature type, e.g., int(*)(int, const char*)
 * @param ...        Arguments to forward
 * @note Only works in MULTI mode. In SHARED mode, use SHADOWHOOK_CALL_PREV directly. */
#define MEMKIT_CALL_PREV(func, func_sig, ...) \
    ((func_sig)memkit_get_prev_func((void *)(func)))(__VA_ARGS__)

/* Pop the current stack frame after a proxy call returns.
 * Must be called at the end of every proxy function. */
#define MEMKIT_POP_STACK() \
    memkit_pop_stack(__builtin_return_address(0))

/* Allow reentrant calls to this proxy from the same thread. */
#define MEMKIT_ALLOW_REENTRANT() \
    memkit_allow_reentrant(__builtin_return_address(0))

/* Disallow reentrant calls to this proxy from the same thread. */
#define MEMKIT_DISALLOW_REENTRANT() \
    memkit_disallow_reentrant(__builtin_return_address(0))

/* Get the return address of the current proxy caller. */
#define MEMKIT_RETURN_ADDRESS() \
    memkit_get_return_address()

// ============================================================================
// HOOKING: PROXY & STACK MANAGEMENT (Functions)
// ============================================================================

void *memkit_get_prev_func(void *func);
void memkit_pop_stack(const void *return_address);
void memkit_allow_reentrant(const void *return_address);
void memkit_disallow_reentrant(const void *return_address);
void *memkit_get_return_address(void);

// ============================================================================
// HOOKING: FLAGS (V2 API)
// ============================================================================

#define MK_HOOK_DEFAULT                 0
#define MK_HOOK_WITH_SHARED_MODE        1
#define MK_HOOK_WITH_UNIQUE_MODE        2
#define MK_HOOK_WITH_MULTI_MODE         4
#define MK_HOOK_RECORD                  8

void *memkit_hook_v2(const char *lib_name, const char *sym_name, void *new_addr, void **orig_addr, uint32_t flags);
void *memkit_hook_by_symbol_v2(const char *lib_name, const char *sym_name, void *new_addr, void **orig_addr, uint32_t flags);

/**
 * Hook by function address with flags (variadic for RECORD mode)
 * When MK_HOOK_RECORD is set, pass record_lib_name and record_sym_name after flags.
 */
void *memkit_hook_func_addr_2(void *func_addr, void *new_addr, void **orig_addr, uint32_t flags, ...);

/**
 * Hook by symbol address with flags (variadic for RECORD mode)
 * When MK_HOOK_RECORD is set, pass record_lib_name and record_sym_name after flags.
 */
void *memkit_hook_sym_addr_2(void *sym_addr, void *new_addr, void **orig_addr, uint32_t flags, ...);

/**
 * Hook by symbol name with flags and completion callback
 */
void *memkit_hook_sym_name_callback_2(const char *lib_name, const char *sym_name, void *new_addr, void **orig_addr, uint32_t flags, MemKitHooked hooked, void *hooked_arg);

// ============================================================================
// HOOKING: CALLBACK VARIANTS
// ============================================================================

/* Hook with completion callback */
void *memkit_hook_with_callback(const char *lib_name, const char *sym_name, void *new_addr, void **orig_addr, MemKitHooked hooked, void *hooked_arg);

/* Hook by symbol name with completion callback (alias) */
void *memkit_hook_by_symbol_callback(const char *lib_name, const char *sym_name, void *new_addr, void **orig_addr, MemKitHooked hooked, void *hooked_arg);

#endif // MEMKIT_HOOK_H
