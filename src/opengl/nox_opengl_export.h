
#ifndef NOX_OPENGL_EXPORT_H
#define NOX_OPENGL_EXPORT_H

#ifdef NOX_OPENGL_STATIC_DEFINE
#  define NOX_OPENGL_EXPORT
#  define NOX_OPENGL_NO_EXPORT
#else
#  ifndef NOX_OPENGL_EXPORT
#    ifdef nox_opengl_EXPORTS
        /* We are building this library */
#      define NOX_OPENGL_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define NOX_OPENGL_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef NOX_OPENGL_NO_EXPORT
#    define NOX_OPENGL_NO_EXPORT 
#  endif
#endif

#ifndef NOX_OPENGL_DEPRECATED
#  define NOX_OPENGL_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef NOX_OPENGL_DEPRECATED_EXPORT
#  define NOX_OPENGL_DEPRECATED_EXPORT NOX_OPENGL_EXPORT NOX_OPENGL_DEPRECATED
#endif

#ifndef NOX_OPENGL_DEPRECATED_NO_EXPORT
#  define NOX_OPENGL_DEPRECATED_NO_EXPORT NOX_OPENGL_NO_EXPORT NOX_OPENGL_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef NOX_OPENGL_NO_DEPRECATED
#    define NOX_OPENGL_NO_DEPRECATED
#  endif
#endif

#endif /* NOX_OPENGL_EXPORT_H */
