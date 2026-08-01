#ifndef MEMKIT_INTERCEPT_H
#define MEMKIT_INTERCEPT_H

#include "memkit_common.h"

// ============================================================================
// INTERCEPT API
// ============================================================================

/* Intercept flags */
#define MK_INTERCEPT_DEFAULT                0
#define MK_INTERCEPT_WITH_FPSIMD_READ_ONLY  1
#define MK_INTERCEPT_WITH_FPSIMD_WRITE_ONLY 2
#define MK_INTERCEPT_WITH_FPSIMD_READ_WRITE 3
#define MK_INTERCEPT_RECORD                 4

/* Intercept by function address */
void *memkit_intercept(void *func_addr, MemKitInterceptor pre, void *data, uint32_t flags, ...);

/* Intercept by symbol address */
void *memkit_intercept_by_sym_addr(void *sym_addr, MemKitInterceptor pre, void *data, uint32_t flags, ...);

/* Intercept by library name and symbol name */
void *memkit_intercept_by_symbol(const char *lib_name, const char *sym_name, MemKitInterceptor pre, void *data, uint32_t flags);

/* Remove an interceptor */
int memkit_unintercept(void *stub);

/* Intercept at a specific instruction address */
void *memkit_intercept_at_instr(void *instr_addr, MemKitInterceptor pre, void *data, uint32_t flags, ...);

/* Intercept with completion callback */
void *memkit_intercept_with_callback(const char *lib_name, const char *sym_name, MemKitInterceptor pre, void *data, uint32_t flags, MemKitIntercepted intercepted, void *arg);

#endif // MEMKIT_INTERCEPT_H
