# GPUOpen-radeon_gpu_analyzer - AMD Direct3D 11 GSA headers and chip identifiers

> **NOTE:**  
> This repository provides headers for consumption by other projects.

This is a `build2` package repository for
[`radeon_gpu_analyzer`](https://github.com/GPUOpen-Tools/radeon_gpu_analyzer),
the AMD Direct3D 11 GSA shader compile extension headers published with
Radeon GPU Analyzer.

This file contains setup instructions and other details that are more
appropriate for development rather than consumption. If you want to use
`libgpuopen-amddxgsa` in your `build2`-based project, then instead see the
accompanying [`PACKAGE-README.md`](libgpuopen-amddxgsa/PACKAGE-README.md) file.

The development setup for `GPUOpen-radeon_gpu_analyzer` uses the standard `bdep`-based workflow.
For example:

```
git clone https://github.com/build2-packaging/GPUOpen-radeon_gpu_analyzer.git
cd GPUOpen-radeon_gpu_analyzer

bdep init -C @gcc cc config.cxx=g++
bdep update
bdep test
```
