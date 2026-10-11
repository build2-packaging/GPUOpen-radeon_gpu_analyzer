# libgpuopen-amddxgsa - AMD Direct3D 11 GSA headers and chip identifiers

> **NOTE:**  
> This package provides the AMD Direct3D 11 GSA shader compile extension
> headers published with
> [Radeon GPU Analyzer](https://github.com/GPUOpen-Tools/radeon_gpu_analyzer):
> `GSA/AmdDxGsaCompile.h` and `AsicReg/devices.h`. The extension is the
> Direct3D 11 driver ABI that compiles DXASM bytecode or AMD IL text for a
> target AMD GPU and returns an ELF object with the AMD IL and GPU ISA
> binaries, their disassembly, and register and memory usage statistics.
> `devices.h` provides the chip family and revision identifiers for that
> target.

This is a `build2` package for those headers as distributed with
[Radeon GPU Analyzer](https://github.com/GPUOpen-Tools/radeon_gpu_analyzer)
(RGA).


## Usage

To start using `libgpuopen-amddxgsa` in your project, add the following `depends`
value to your `manifest`, adjusting the version constraint as appropriate:

```
depends: libgpuopen-amddxgsa ^2.15.0
```

Then import the library in your `buildfile`:

```
import libs = libgpuopen-amddxgsa%lib{gpuopen-amddxgsa}
```


## Importable targets

This package provides the following importable targets:

```
lib{gpuopen-amddxgsa}
```

The shader compile extension header and the AMD GPU chip identifiers, both
included with their upstream subdirectory prefix
(`#include <GSA/AmdDxGsaCompile.h>` and `#include <AsicReg/devices.h>`).

This package is headers only. The `AmdDxGsaCompileShader()` and
`AmdDxGsaFreeCompiledShader()` functions declared in `AmdDxGsaCompile.h` are
exported by the AMD Direct3D 11 driver DLL (`atidxx64.dll` or
`atidxx32.dll`), which is not included, so they cannot be linked against.
Instead, load the driver DLL at runtime with `LoadLibrary()` and resolve the
functions with `GetProcAddress()` using the `PfnAmdDxGsaCompileShader` and
`PfnAmdDxGsaFreeCompiledShader` function pointer types declared in the header.

As in upstream, `AmdDxGsaCompile.h` includes `<windows.h>` and so is only
usable on Windows. `devices.h` defines the chip family (`FAMILY_*`) and
revision values for the `chipFamily` and `chipRevision` members of
`AmdDxGsaCompileShaderInput` and is usable on all platforms. It can only be
included once per translation unit since not all of its declarations are
covered by include guards.


## Configuration variables

This package provides no configuration variables.
