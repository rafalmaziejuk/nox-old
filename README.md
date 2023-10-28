# NOX - Graphics Rendering Library
NOX is a cross-platform and graphics API agnostic library. The API is designed to be similar to 
a modern graphics APIs such as Vulkan or Direct3D 12. It is written in C++17 and uses CMake build 
system.

## Platform Support

| Platform | CI | OpenGL | Vulkan | D3D12 |
|----------|:--:|:----------:|:------:|:-----:|
| Windows | [![Windows](https://github.com/rafalmaziejuk/nox/actions/workflows/ci.yml/badge.svg?branch=master)](https://github.com/rafalmaziejuk/nox/actions/workflows/ci.yml) | In Development | Planned | Planned |
| Linux | N/A | Planned | Planned | N/A |

## Building
```
git clone https://github.com/rafalmaziejuk/nox
```

### Windows
```
mkdir build && cd build
cmake ..
```

**Microsoft Visual Studio**  
Enter `scripts/` directory, create `custom_config.cmd` or edit existing 
`config.cmd` file and run `generate_nox_visual.bat` script.  
Visual Studio solution generation script can also be used as follows:
```
generate_nox_visual.bat <BUILD_TYPE> <VISUAL_STUDIO_VERSION>
generate_nox_visual.bat shared 2022
```

## Dependencies
* [glad](https://github.com/Dav1dde/glad) : Loader-Generator for OpenGL API.
