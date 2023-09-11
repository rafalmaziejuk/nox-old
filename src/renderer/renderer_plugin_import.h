#pragma once

#include "plugins/plugin_interface.h"

#define NOX_PLUGIN_LOAD_OPENGL() false

#if defined(NOX_BUILD_RENDERER_OPENGL)
NOX_PLUGIN_IMPORT(OpenGL);
#endif

#if defined(NOX_BUILD_RENDERER_OPENGL)
#undef NOX_PLUGIN_LOAD_OPENGL
#define NOX_PLUGIN_LOAD_OPENGL() NOX_PLUGIN_LOAD(OpenGL)
#endif
