# General Guide

## Quick Start

### 1. Prerequisites

```bash
# NDK r25b or newer required
export ANDROID_NDK_HOME=/path/to/your/android-ndk

# CMake 3.14+ required (used by Makefile to generate embed headers)
# Install via: sudo apt install cmake  (or brew install cmake)
```

### 2. Clone & Setup

```bash
git clone https://github.com/HanSoBored/Android-Mem-Kit.git
cd Android-Mem-Kit
git submodule update --init --recursive
```

### 3. Build

```bash
# Default build (arm64-v8a)
make

# Build with tests
make test

# Custom ABI
make ANDROID_ABI=armeabi-v7a

# Clean build
make clean && make

# Custom build with CMake (for subproject use)
cmake -B build -DMEMKIT_BUILD_SHARED=OFF -DCMAKE_TOOLCHAIN_FILE=...
cmake --build build

# Build with examples (CMake)
cmake -B build -DBUILD_EXAMPLES=ON -DCMAKE_TOOLCHAIN_FILE=...
cmake --build build
```

### 4. Basic Usage

```c
#include "memkit.h"
#include <android/log.h>

#define LOG_TAG "MyResearch"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

static int (*orig_SSL_read)(void* ssl, void* buf, int num) = NULL;

static int my_SSL_read(void* ssl, void* buf, int num) {
    LOGI("SSL_read: buf=%p, size=%d", buf, num);
    return orig_SSL_read(ssl, buf, num);
}

__attribute__((constructor))
void init() {
    memkit_hook_init(SHADOWHOOK_MODE_UNIQUE, false);

    void* stub = memkit_hook_by_symbol(
        "libssl.so", "SSL_read",
        (void*)my_SSL_read,
        (void**)&orig_SSL_read
    );

    if (stub) LOGI("SSL_read hooked!");
}
```
