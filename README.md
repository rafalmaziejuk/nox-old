# NOX - Cross-Platform Graphics Rendering Library

<p align="center">
    <a href="https://github.com/rafalmaziejuk/nox/blob/master/LICENSE"><img alt="License" src="https://img.shields.io/badge/License-Apache_2.0-blue.svg" /></a>
    <a href="https://github.com/rafalmaziejuk/nox/actions/workflows/ci.yml"><img src="https://github.com/rafalmaziejuk/nox/actions/workflows/ci.yml/badge.svg?branch=master"/></a>
</p>

NOX is a C++17 library crafted for cross-platform graphics rendering, providing a unified API for modern graphics APIs such as Vulkan or D3D12, as well as OpenGL. Tailored for Windows and Linux development, NOX simplifies the intricacies of low-level graphics programming, enabling you to focus on creating immersive visual experiences.

## Platform Support

| Platform | OpenGL | Vulkan | D3D12 |
|----------|:----------:|:------:|:-----:|
| Windows | In Development | Planned | Planned |
| Linux | In Development | Planned | N/A |



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
