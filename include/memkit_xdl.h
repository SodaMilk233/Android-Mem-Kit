#ifndef MEMKIT_XDL_H
#define MEMKIT_XDL_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Forward declaration for memkit_xdl_open_from_phdr */
struct dl_phdr_info;

// ============================================================================
// XDL WRAPPER TYPES
// ============================================================================

/**
 * Library information for iteration callbacks
 */
typedef struct {
    const char* name;             // Library basename (e.g., "libil2cpp.so")
    const char* path;             // Full path (may be NULL if not available)
    uintptr_t base;               // Load base address
    size_t size;                  // Library size in bytes
} MemKitLibInfo;

/**
 * Callback type for library iteration
 * @param info Library information
 * @param user_data User context data
 * @return true to continue iteration, false to stop
 */
typedef bool (*memkit_lib_iter_cb_t)(const MemKitLibInfo* info, void* user_data);

/**
 * Symbol information from address resolution
 */
typedef struct {
    const char* lib_name;         // Library pathname
    uintptr_t lib_base;           // Library base address
    const char* sym_name;         // Nearest symbol name (may be NULL)
    uintptr_t sym_offset;         // Offset from symbol start
    size_t sym_size;              // Symbol size in bytes
} MemKitSymInfo;

/**
 * Opaque context for address resolution cache
 * Manages internal xdl_addr() cache to prevent memory leaks
 */
typedef struct memkit_addr_ctx memkit_addr_ctx_t;

// ============================================================================
// XDL FLAG CONSTANTS
// ============================================================================

/* memkit_xdl_open() flags */
#define XDL_DEFAULT           0x00
#define XDL_TRY_FORCE_LOAD    0x01
#define XDL_ALWAYS_FORCE_LOAD 0x02

/* memkit_xdl_addr_to_symbol4() flags */
#define XDL_NON_SYM           0x01

/* memkit_xdl_iterate() flags */
#define XDL_FULL_PATHNAME     0x01

/* xdl_dlinfo() selector */
#define XDL_DI_DLINFO         1

// ============================================================================
// XDL WRAPPER API - PHASE 1: CORE DISCOVERY
// ============================================================================

/**
 * Iterate all loaded shared libraries
 *
 * @param callback Function called for each library
 * @param user_data User context passed to callback
 * @param flags XDL_DEFAULT or XDL_FULL_PATHNAME
 * @return Number of libraries iterated, or -1 on error
 *
 * Example:
 *   typedef struct { const char* target; uintptr_t base; } ctx_t;
 *
 *   bool callback(const MemKitLibInfo* info, void* user_data) {
 *       ctx_t* ctx = (ctx_t*)user_data;
 *       if (strcmp(info->name, ctx->target) == 0) {
 *           ctx->base = info->base;
 *           return false; // Stop iteration
 *       }
 *       return true; // Continue
 *   }
 *
 *   ctx_t ctx = {.target = "libil2cpp.so"};
 *   memkit_xdl_iterate(callback, &ctx, XDL_DEFAULT);
 */
int memkit_xdl_iterate(memkit_lib_iter_cb_t callback, void* user_data, int flags);

/**
 * Open a handle to any loaded library
 *
 * @param name Library name (basename or full path)
 * @param flags XDL_DEFAULT, XDL_TRY_FORCE_LOAD, or XDL_ALWAYS_FORCE_LOAD
 * @return Handle on success, NULL on failure
 *
 * Example:
 *   void* handle = memkit_xdl_open("libc.so", XDL_DEFAULT);
 *   if (handle) {
 *       void* sym = memkit_xdl_sym(handle, "open", NULL);
 *       memkit_xdl_close(handle);
 *   }
 */
void* memkit_xdl_open(const char* name, int flags);

/**
 * Close a library handle
 *
 * @param handle Handle from memkit_xdl_open()
 * @return true if handle was closed, false if no action needed
 */
bool memkit_xdl_close(void* handle);

/**
 * Resolve a symbol from a library handle (.dynsym section)
 *
 * @param handle Library handle from memkit_xdl_open()
 * @param symbol Symbol name to resolve
 * @param out_size Optional output: symbol size
 * @return Symbol address on success, NULL on failure
 *
 * Example:
 *   void* handle = memkit_xdl_open("libc.so", XDL_DEFAULT);
 *   size_t size;
 *   void* open_sym = memkit_xdl_sym(handle, "open", &size);
 */
void* memkit_xdl_sym(void* handle, const char* symbol, size_t* out_size);

/**
 * Resolve a debug symbol from a library handle (.symtab section)
 * Use for stripped/internal symbols not in .dynsym
 *
 * @param handle Library handle from memkit_xdl_open()
 * @param symbol Symbol name to resolve
 * @param out_size Optional output: symbol size
 * @return Symbol address on success, NULL on failure
 */
void* memkit_xdl_dsym(void* handle, const char* symbol, size_t* out_size);

/**
 * Get detailed information about a loaded library
 *
 * @param handle Library handle from memkit_xdl_open()
 * @param out Output: library information
 * @return true on success, false on failure
 *
 * Example:
 *   MemKitLibInfo info;
 *   if (memkit_xdl_get_lib_info(handle, &info)) {
 *       // info.base, info.size available
 *   }
 */
bool memkit_xdl_get_lib_info(void* handle, MemKitLibInfo* out);

// ============================================================================
// XDL WRAPPER API - PHASE 2: DEBUG INTROSPECTION
// ============================================================================

/**
 * Create address resolution context
 * Must be destroyed with memkit_xdl_addr_ctx_destroy() to prevent memory leaks
 *
 * @return Context pointer, or NULL on failure
 *
 * Example:
 *   memkit_addr_ctx_t* ctx = memkit_xdl_addr_ctx_create();
 *   // ... use context for multiple lookups ...
 *   memkit_xdl_addr_ctx_destroy(ctx);
 */
memkit_addr_ctx_t* memkit_xdl_addr_ctx_create(void);

/**
 * Destroy address resolution context and free cache
 *
 * @param ctx Context pointer from memkit_xdl_addr_ctx_create()
 */
void memkit_xdl_addr_ctx_destroy(memkit_addr_ctx_t* ctx);

/**
 * Resolve address to symbol information
 *
 * @param addr Target address to resolve
 * @param out Output: symbol information
 * @param ctx Context from memkit_xdl_addr_ctx_create()
 * @return true on success, false on failure
 *
 * Example:
 *   memkit_addr_ctx_t* ctx = memkit_xdl_addr_ctx_create();
 *   MemKitSymInfo info;
 *   if (memkit_xdl_addr_to_symbol((void*)0x12345678, &info, ctx)) {
 *       // info.sym_name contains nearest symbol
 *   }
 *   memkit_xdl_addr_ctx_destroy(ctx);
 */
bool memkit_xdl_addr_to_symbol(void* addr, MemKitSymInfo* out, memkit_addr_ctx_t* ctx);

/**
 * Resolve address to symbol with flags (advanced)
 *
 * @param addr Target address
 * @param out Output: symbol information
 * @param ctx Context pointer (may be NULL for single lookup)
 * @param flags XDL_DEFAULT or XDL_NON_SYM (skip symbol lookup)
 * @return true on success, false on failure
 *
 * Use XDL_NON_SYM for fast address-to-library lookup without symbol resolution.
 */
bool memkit_xdl_addr_to_symbol4(void* addr, MemKitSymInfo* out, memkit_addr_ctx_t* ctx, int flags);

// ============================================================================
// XDL WRAPPER API - PHASE 3: ADVANCED FEATURES
// ============================================================================

/**
 * Create a handle from dl_phdr_info (advanced use case)
 * Typically used during library iteration callbacks
 *
 * @param info Pointer to dl_phdr_info structure
 * @return Handle on success, NULL on failure
 *
 * Example:
 *   bool callback(const MemKitLibInfo* info, void* user_data) {
 *       // Create handle without knowing library name
 *       void* handle = memkit_xdl_open_from_phdr(info->phdr_info);
 *   }
 */
void* memkit_xdl_open_from_phdr(struct dl_phdr_info* info);

// ============================================================================
// CONVENIENCE MACROS
// ============================================================================

/**
 * XDL_RESOLVE - Resolve symbol from library (one-shot, auto-closes handle)
 *
 * Usage: void* sym = XDL_RESOLVE("libc.so", "open");
 *
 * Note: This is a convenience macro for quick one-off lookups.
 * For multiple lookups, use memkit_xdl_open() + memkit_xdl_sym() directly.
 */
#define XDL_RESOLVE(lib_name, symbol) \
    memkit_xdl_sym(memkit_xdl_open(lib_name, XDL_DEFAULT), symbol, NULL)

/**
 * XDL_RESOLVE_SIZE - Resolve symbol with size output
 *
 * Usage:
 *   size_t size;
 *   void* sym = XDL_RESOLVE_SIZE("libc.so", "open", &size);
 */
#define XDL_RESOLVE_SIZE(lib_name, symbol, out_size) \
    memkit_xdl_sym(memkit_xdl_open(lib_name, XDL_DEFAULT), symbol, out_size)

#endif // MEMKIT_XDL_H
