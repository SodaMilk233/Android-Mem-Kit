#ifndef MEMKIT_IL2CPP_H
#define MEMKIT_IL2CPP_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// ============================================================================
// IL2CPP API (XDL)
// ============================================================================

/**
 * Resolve an IL2CPP export symbol from .dynsym
 * @param symbol_name Symbol name (e.g., "il2cpp_domain_get")
 * @return Function pointer or NULL on failure
 */
void* memkit_il2cpp_resolve(const char* symbol_name);

/**
 * Resolve an IL2CPP symbol from .symtab section only (advanced)
 * Use this for stripped/internal symbols not in .dynsym
 * @param symbol_name Symbol name
 * @return Function pointer or NULL on failure
 */
void* memkit_il2cpp_resolve_symtab(const char* symbol_name);

/**
 * Get cached IL2CPP handle
 * @return Handle pointer or NULL
 */
void* memkit_il2cpp_get_handle(void);

/**
 * Get the Il2CppImage* for a named assembly.
 * Handles the full chain: domain_get_assemblies → find by name → assembly_get_image.
 * Falls back to image name matching if assembly_get_name is not exported.
 *
 * @param assembly_name Assembly name (e.g., "Assembly-CSharp")
 * @return Il2CppImage* pointer, or NULL on failure.
 */
void* memkit_il2cpp_get_image(const char* assembly_name);

/**
 * Safely call an IL2CPP runtime API with crash protection.
 * If the call crashes (SIGSEGV/SIGBUS), the function returns false instead
 * of killing the process.
 *
 * @param fn Function pointer to call (takes one arg, returns pointer)
 * @param arg Argument to pass to fn
 * @param out_result Output: result pointer (set only if true returned)
 * @return true if call succeeded, false if crashed or returned NULL.
 */
bool memkit_il2cpp_safe_call(void* (*fn)(void*), void* arg, void** out_result);

/**
 * Wait until the IL2CPP runtime is ready.
 * Polls il2cpp_domain_get() until it returns non-NULL or timeout.
 *
 * @param timeout_ms Maximum time to wait in milliseconds
 * @return Domain pointer, or NULL on timeout.
 */
void* memkit_il2cpp_wait_ready(int timeout_ms);

/**
 * Attach the current thread to the IL2CPP domain.
 *
 * @param domain IL2CPP domain pointer (use memkit_il2cpp_wait_ready or il2cpp_domain_get)
 * @return Thread pointer, or NULL on failure.
 */
void* memkit_il2cpp_attach_thread(void* domain);

/**
 * Detach the current thread from the IL2CPP domain.
 *
 * @param thread Thread pointer from memkit_il2cpp_attach_thread
 */
void memkit_il2cpp_detach_thread(void* thread);

// ============================================================================
// CONVENIENCE MACROS
// ============================================================================

/**
 * IL2CPP_CALL macro - Auto-caches resolved function pointer
 * Uses __builtin_expect for branch prediction optimization
 *
 * Usage: IL2CPP_CALL(return_type, "symbol_name", arg_types...)(arguments...)
 *
 * Example:
 *   void* domain = IL2CPP_CALL(void*, "il2cpp_domain_get")(void);
 *   IL2CPP_CALL(void, "il2cpp_thread_attach", void*)(domain);
 */
#define IL2CPP_CALL(ret_type, func_name, ...) ({ \
    static ret_type (*func_ptr)(__VA_ARGS__) = NULL; \
    if (__builtin_expect(!func_ptr, 0)) { \
        func_ptr = (ret_type (*)(__VA_ARGS__)) memkit_il2cpp_resolve(func_name); \
    } \
    func_ptr; \
})

#endif // MEMKIT_IL2CPP_H
