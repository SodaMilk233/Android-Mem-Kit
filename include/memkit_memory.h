#ifndef MEMKIT_MEMORY_H
#define MEMKIT_MEMORY_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// ============================================================================
// MEMORY PATCHING API
// ============================================================================

typedef struct {
    uintptr_t address;
    size_t size;
    uint8_t* orig_bytes;
    uint8_t* patch_bytes;
} MemPatch;

/**
 * Get the lowest base address of a loaded library
 * @param lib_name Name of the library (e.g., "libil2cpp.so")
 * @return Base address or 0 if not found
 */
uintptr_t memkit_get_lib_base(const char* lib_name);

/**
 * Get library base via xdl_iterate_phdr (linker internals)
 * Finds libraries even when loaded in-place from APK ZIP files.
 * Uses deterministic matching: exact basename match first, then ".so" suffix fallback.
 * Unlike the old strstr behavior, this does NOT match "libil2cpp_hook.so" when
 * searching for "libil2cpp" — iteration order is deterministic.
 * @param lib_name   Library name or pattern (e.g., "libMyGame.so")
 * @return           Base address, or 0 if not found
 */
uintptr_t memkit_get_lib_base_from_xdl(const char* lib_name);

/**
 * Find library base by parsing APK ZIP files in process maps.
 * Handles games that load .so files directly from split APKs
 * without extraction (Android 12+ behavior).
 * @param lib_entry  Library entry path within APK (e.g., "lib/arm64-v8a/libMyGame.so")
 * @param out_base   Output: base address if found
 * @return           true if found, false otherwise
 */
bool memkit_get_lib_base_in_apk(const char* lib_entry, uintptr_t* out_base);

/**
 * Enhanced library base discovery — tries all methods:
 *   1. /proc/self/maps (fast, normal .so loading)
 *   2. xdl_iterate_phdr (linker internals, APK-loaded libs)
 *      Uses dlsym(RTLD_DEFAULT) — gracefully skipped if
 *      memkit_get_lib_base_from_xdl is not exported or stripped
 *      from the symbol table, or if xdl_wrapper.c is not linked.
 *   3. APK ZIP parsing (last resort, split APK in-place mapping)
 *
 * @param lib_name   Library name pattern (e.g., "libMyGame.so")
 * @return           Base address, or 0 if not found on all methods
 */
uintptr_t memkit_get_lib_base_v2(const char* lib_name);

/**
 * Create a memory patch from hex string
 * Automatically backs up original bytes safely (handles XOM)
 * @param address Target absolute address
 * @param hex_string Hex string with spaces (e.g., "00 00 A0 E3")
 * @return MemPatch pointer or NULL on failure
 */
MemPatch* memkit_patch_create(uintptr_t address, const char* hex_string);

/**
 * Apply the memory patch
 * @param patch MemPatch pointer
 * @return true on success, false on failure
 */
bool memkit_patch_apply(MemPatch* patch);

/**
 * Restore original bytes
 * @param patch MemPatch pointer
 * @return true on success, false on failure
 */
bool memkit_patch_restore(MemPatch* patch);

/**
 * Free MemPatch resources
 * @param patch MemPatch pointer
 */
void memkit_patch_free(MemPatch* patch);

#endif // MEMKIT_MEMORY_H
