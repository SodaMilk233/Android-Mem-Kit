# Android-Mem-Kit

**A Lightweight Native Instrumentation Library for Android Security Research**

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Platform: Android 5.0+](https://img.shields.io/badge/Platform-Android%205.0+-blue.svg)]()
[![NDK: r25b+](https://img.shields.io/badge/NDK-r25b+-green.svg)]()

Android-Mem-Kit is a minimal-overhead, pure C library for Android native instrumentation. It provides memory patching, function hooking, and symbol resolution capabilities for **security research, debugging, and educational purposes**.

---

## Features

| Feature | Implementation | Description |
| :--- | :--- | :--- |
| **Function Hooking** | [ShadowHook](https://github.com/bytedance/android-inline-hook) | Inline hook with intercept, proxy chaining, and records |
| **Symbol Resolution** | [XDL](https://github.com/hexhacking/xdl) | Bypasses Android 7+ linker restrictions |
| **JIT Code Generation** | [SLJIT](https://github.com/zherczeg/sljit) | Platform-independent runtime code generation |

### Why Pure C?

- **Small Binary Size**: <100KB overhead
- **Simple NDK Integration**: No FFI bridge or complex build setup
- **Direct JNI/NDK Access**: Native C integration with Android frameworks
- **Modern Tooling**: Leverages battle-tested libraries (ShadowHook, XDL, SLJIT)

---

## Documentation

| Document | Content |
|----------|---------|
| **[docs/GENERAL.md](docs/GENERAL.md)** | Quick start: prerequisites, setup, build, and basic usage |
| **[docs/USAGE.md](docs/USAGE.md)** | Complete API reference: memory, hooking, intercept, records, JIT, IL2CPP, XDL, DL callbacks |
| **[docs/RECIPES.md](docs/RECIPES.md)** | Common patterns: integrity checks, SSL pinning bypass, JIT code generation |

---

## Known Issues

### ShadowHook Error 12

When using ShadowHook on building shared library, you may encounter **error code 12** (`MK_ERRNO_INIT_LINKER`) during `memkit_hook_init()`. This is caused by missing libshadowhook_nothing.so.

**Root Cause:** ShadowHook requires `libshadowhook_nothing.so` to be present alongside `libshadowhook.so` for proper shared library compatibility. When building from local sources (USE_LOCAL_DEPS=ON), this library is now automatically built as part of the CMake build process.

### Subproject Builds

When building memkit as a subproject via `add_subdirectory()` in your parent project's `CMakeLists.txt`:

```cmake
enable_language(ASM)  # Required for shadowhook assembly (sh_glue.S). Needs CMake 3.9+.
add_subdirectory(path/to/Android-Mem-Kit)
target_link_libraries(your_lib)
```

Set `-DMEMKIT_BUILD_SHARED=OFF` to build a static library. The default is `OFF` for subprojects and `ON` for standalone builds.

### Build Output

The build process produces two files in `build/<ABI>/lib/`:
- **libmemkit.so** - Main memkit library (~190KB for arm64-v8a with JIT)
- **libshadowhook_nothing.so** - Required companion library for shared library compatibility (~1.5KB)

Both files must be packaged together in your `lib/<ABI>/` directory.

---

## Credits

This project utilizes excellent open-source libraries:

- **[ShadowHook](https://github.com/bytedance/android-inline-hook)** by ByteDance - Inline hooking for Android
- **[XDL](https://github.com/hexhacking/xdl)** by HexHacking - Dynamic linker bypass
- **[SLJIT](https://github.com/zherczeg/sljit)** - Platform-independent JIT code generation

---

## License

MIT License - See [LICENSE](LICENSE) file for details.

---

## Contributing

Contributions are welcome! Please read [CONTRIBUTING.md](CONTRIBUTING.md) first.

---

## Support

- **Issues**: [GitHub Issues](https://github.com/HanSoBored/Android-Mem-Kit/issues)
- **Discussions**: [GitHub Discussions](https://github.com/HanSoBored/Android-Mem-Kit/discussions)

---

*Built for the security research community. Use responsibly.*
