
#ifndef VECTOR_EXPORT_H
#define VECTOR_EXPORT_H

#ifdef VECTOR_STATIC_DEFINE
#  define VECTOR_EXPORT
#  define VECTOR_NO_EXPORT
#else
#  ifndef VECTOR_EXPORT
#    ifdef vector_EXPORTS
        /* We are building this library */
#      define VECTOR_EXPORT 
#    else
        /* We are using this library */
#      define VECTOR_EXPORT 
#    endif
#  endif

#  ifndef VECTOR_NO_EXPORT
#    define VECTOR_NO_EXPORT 
#  endif
#endif

#ifndef VECTOR_DEPRECATED
#  define VECTOR_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef VECTOR_DEPRECATED_EXPORT
#  define VECTOR_DEPRECATED_EXPORT VECTOR_EXPORT VECTOR_DEPRECATED
#endif

#ifndef VECTOR_DEPRECATED_NO_EXPORT
#  define VECTOR_DEPRECATED_NO_EXPORT VECTOR_NO_EXPORT VECTOR_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VECTOR_NO_DEPRECATED
#    define VECTOR_NO_DEPRECATED
#  endif
#endif

#endif /* VECTOR_EXPORT_H */
