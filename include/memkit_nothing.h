#ifndef MEMKIT_NOTHING_H
#define MEMKIT_NOTHING_H

// ============================================================================
// NOTHING LIBRARY PATH — PUBLIC API
//
// Manages the path to libshadowhook_nothing.so, which ShadowHook dlopen()s
// during initialization. When the user doesn't provide a path, memkit
// automatically extracts the embedded blob to a temp file.
// ============================================================================

/**
 * Set the path to libshadowhook_nothing.so
 *
 * Must be called BEFORE memkit_hook_init(). When not called,
 * memkit automatically extracts the embedded library to a temp
 * directory and loads it from there.
 *
 * @param path Absolute path to libshadowhook_nothing.so on device.
 *             Pass NULL to reset to auto-extract behavior.
 */
void memkit_set_nothing_path(const char *path);

/**
 * Get the currently configured nothing library path.
 * Returns NULL if not set and no auto-extract has happened.
 * Caller must free the returned string.
 *
 * NOTE: After memkit_hook_init() completes successfully, the auto-extracted
 * temp file is consumed (unlinked and freed). If you need the path, call
 * memkit_get_nothing_path() BEFORE memkit_hook_init().
 */
char *memkit_get_nothing_path(void);

#endif // MEMKIT_NOTHING_H
