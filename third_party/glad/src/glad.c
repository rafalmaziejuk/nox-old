#if defined(NOX_GLAD_WIN32)
#define GLAD_WGL_IMPLEMENTATION
#include <glad/wgl.h>
#endif

#if defined(NOX_GLAD_UNIX)
#define GLAD_GLX_IMPLEMENTATION
#include <glad/glx.h>
#endif

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
