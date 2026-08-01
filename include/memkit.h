#ifndef MEMKIT_H
#define MEMKIT_H

/* ============================================================================
 * Android-Mem-Kit — Umbrella Public Header
 *
 * Including "memkit.h" exposes the entire public API. For smaller compile
 * footprints, include only the sub-headers you need instead:
 *
 *   memkit_common.h      Shared types, error codes, callbacks
 *   memkit_memory.h      Memory patching & library base discovery
 *   memkit_hook.h        Function hooking (ShadowHook), V2 flags, proxy/stack
 *   memkit_intercept.h   Intercept API (CPU context inspection/modification)
 *   memkit_records.h     Hook/intercept operation records (CSV)
 *   memkit_il2cpp.h      IL2CPP symbol resolution & runtime helpers
 *   memkit_xdl.h         XDL wrapper (library discovery, symbol resolution)
 *   memkit_dl.h          DL helpers & dlopen/dlclose callbacks
 *   memkit_runtime.h     Runtime configuration (mode, debug, record, disable)
 *   memkit_nothing.h     libshadowhook_nothing.so path management
 *   memkit_jit.h         JIT compiler API (SLJIT wrappers)
 * ========================================================================== */

#include "memkit_common.h"
#include "memkit_memory.h"
#include "memkit_hook.h"
#include "memkit_intercept.h"
#include "memkit_records.h"
#include "memkit_il2cpp.h"
#include "memkit_xdl.h"
#include "memkit_dl.h"
#include "memkit_runtime.h"
#include "memkit_nothing.h"

/* JIT API — includes sljitLir.h (see memkit_jit.h for full documentation) */
#include "memkit_jit.h"

#endif // MEMKIT_H
