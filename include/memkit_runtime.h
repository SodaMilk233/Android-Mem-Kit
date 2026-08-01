#ifndef MEMKIT_RUNTIME_H
#define MEMKIT_RUNTIME_H

#include "memkit_common.h"

// ============================================================================
// RUNTIME CONFIGURATION
// ============================================================================

/* Convenience mode check macros (evaluates to true/false at runtime) */
#define MEMKIT_IS_SHARED_MODE (SHADOWHOOK_MODE_SHARED == memkit_get_mode())
#define MEMKIT_IS_UNIQUE_MODE (SHADOWHOOK_MODE_UNIQUE == memkit_get_mode())
#define MEMKIT_IS_MULTI_MODE  (SHADOWHOOK_MODE_MULTI == memkit_get_mode())

int memkit_get_mode(void);
void memkit_set_debuggable(bool debuggable);
bool memkit_get_debuggable(void);
void memkit_set_recordable(bool recordable);
bool memkit_get_recordable(void);
void memkit_set_disable(bool disable);
bool memkit_get_disable(void);

#endif // MEMKIT_RUNTIME_H
