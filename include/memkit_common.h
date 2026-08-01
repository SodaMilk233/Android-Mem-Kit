#ifndef MEMKIT_COMMON_H
#define MEMKIT_COMMON_H

/* Feature-test macro for fseeko64/ftello64/off64_t on older NDK / non-Android POSIX */
#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE 1
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <link.h>

/* Forward declaration for shadowhook.h compatibility on non-glibc platforms */
struct dl_phdr_info;

#include "../deps/shadowhook/shadowhook/src/main/cpp/include/shadowhook.h"

// ============================================================================
// HOOKING: ERROR HANDLING
// ============================================================================

/* Error codes (mirrors ShadowHook's 46 error codes) */
#define MK_ERRNO_OK                     0
#define MK_ERRNO_PENDING                1
#define MK_ERRNO_UNINIT                 2
#define MK_ERRNO_INVALID_ARG            3
#define MK_ERRNO_OOM                    4
#define MK_ERRNO_MPROT                  5
#define MK_ERRNO_WRITE_CRASH            6
#define MK_ERRNO_INIT_ERRNO             7
#define MK_ERRNO_INIT_SIGSEGV           8
#define MK_ERRNO_INIT_SIGBUS            9
#define MK_ERRNO_INTERCEPT_DUP          10
#define MK_ERRNO_INIT_SAFE              11
#define MK_ERRNO_INIT_LINKER            12
#define MK_ERRNO_INIT_HUB               13
#define MK_ERRNO_HUB_CREAT              14
#define MK_ERRNO_MONITOR_DLOPEN         15
#define MK_ERRNO_HOOK_UNIQUE_DUP        16
#define MK_ERRNO_HOOK_DLOPEN_CRASH      17
#define MK_ERRNO_HOOK_DLSYM             18
#define MK_ERRNO_HOOK_DLSYM_CRASH       19
#define MK_ERRNO_HOOK_DUP               20
#define MK_ERRNO_HOOK_DLADDR_CRASH      21
#define MK_ERRNO_HOOK_DLINFO            22
#define MK_ERRNO_HOOK_SYMSZ             23
#define MK_ERRNO_HOOK_ENTER             24
#define MK_ERRNO_HOOK_REWRITE_CRASH     25
#define MK_ERRNO_HOOK_REWRITE_FAILED    26
#define MK_ERRNO_UNHOOK_NOTFOUND        27
#define MK_ERRNO_UNHOOK_CMP_CRASH       28
#define MK_ERRNO_UNHOOK_TRAMPO_MISMATCH 29
#define MK_ERRNO_UNHOOK_EXIT_MISMATCH   30
#define MK_ERRNO_UNHOOK_EXIT_CRASH      31
#define MK_ERRNO_UNHOOK_ON_ERROR        32
#define MK_ERRNO_UNHOOK_ON_UNFINISHED   33
#define MK_ERRNO_ELF_ARCH_MISMATCH      34
#define MK_ERRNO_LINKER_ARCH_MISMATCH   35
#define MK_ERRNO_DUP                    36
#define MK_ERRNO_NOT_FOUND              37
#define MK_ERRNO_NOT_SUPPORT            38
#define MK_ERRNO_INIT_TASK              39
#define MK_ERRNO_HOOK_ISLAND_EXIT       40
#define MK_ERRNO_HOOK_ISLAND_ENTER      41
#define MK_ERRNO_HOOK_ISLAND_REWRITE    42
#define MK_ERRNO_MODE_CONFLICT          43
#define MK_ERRNO_HOOK_MULTI_DUP         44
#define MK_ERRNO_DISABLED               45

/* Get last error code from ShadowHook */
int memkit_errno(void);

/* Get human-readable error message */
const char *memkit_strerror(int errno_code);

/* Get ShadowHook version string */
const char *memkit_version(void);

/* Get error code from last shadowhook_init() call */
int memkit_init_errno(void);

// ============================================================================
// HOOKING: CPU CONTEXT & INTERCEPT TYPES
// ============================================================================

/* CPU context passed to interceptor — direct passthrough to ShadowHook */
typedef shadowhook_cpu_context_t MemKitCpuContext;

/* NEON/VFP vector register — direct passthrough to ShadowHook */
typedef shadowhook_vreg_t MemKitVReg;

/* Interceptor function type — receives CPU context on each call to target */
typedef void (*MemKitInterceptor)(
    MemKitCpuContext *cpu_context,
    void *data
);

// ============================================================================
// HOOKING: CALLBACK TYPES
// ============================================================================

/* Callback invoked when a hook operation completes (success or failure) */
typedef void (*MemKitHooked)(
    int error_number,
    const char *lib_name,
    const char *sym_name,
    void *sym_addr,
    void *new_addr,
    void *orig_addr,
    void *arg
);

/* Callback invoked when an intercept operation completes */
typedef void (*MemKitIntercepted)(
    int error_number,
    const char *lib_name,
    const char *sym_name,
    void *sym_addr,
    void *pre,
    void *data,
    void *arg
);

#endif // MEMKIT_COMMON_H
