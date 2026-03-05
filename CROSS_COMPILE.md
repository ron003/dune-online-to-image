# aarch64 Cross-Compilation Setup

This project is configured to support cross-compilation for aarch64 (ARM64) targets on amd64 (x86-64) development machines.

## Built-in Tools

The Codespace now includes:
- `gcc-aarch64-linux-gnu` - C compiler for aarch64
- `g++-aarch64-linux-gnu` - C++ compiler for aarch64
- `aarch64-linux-gnu-*` - Full GNU toolchain for aarch64

## Quick Build

To build for aarch64, use the provided script:

```bash
./build-aarch64.sh
```

This will:
1. Create a `build-aarch64/` directory
2. Configure CMake with the aarch64 toolchain
3. Build all targets

## Manual Build

If you prefer to configure manually:

```bash
mkdir build-aarch64
cd build-aarch64
cmake -DCMAKE_TOOLCHAIN_FILE=../.devcontainer/cmake-aarch64-toolchain.cmake ..
cmake --build .
```

## Testing aarch64 Binaries

You can test aarch64 binaries on this amd64 system using QEMU emulation (if available), but the primary intent is to cross-compile for deployment to your Tegra IGX system.

## Alignment Considerations

When developing for aarch64, keep these in mind:

- **Memory alignment**: aarch64 is stricter about alignment than x86-64
- **Bit operations**: Safe to use - both architectures handle them identically
- **Pointer arithmetic**: Ensure any pointer casts respect alignment boundaries
- **Endianness**: Both are little-endian, so byte order is compatible

## Development Workflow

1. Develop and test on amd64 (native)
2. Periodically cross-compile with `./build-aarch64.sh`
3. Deploy and test on your Tegra IGX hardware
4. If issues arise, they're likely alignment-related rather than endianness
