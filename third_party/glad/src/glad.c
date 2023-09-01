#if defined(NOX_GLAD_WIN32)
#define GLAD_WGL_IMPLEMENTATION
#include <glad/wgl.h>
#endif

#if defined(NOX_GLAD_UNIX)
// placeholder
#endif

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
