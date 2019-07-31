
#ifndef ERRORCODES_EXPORT_H
#define ERRORCODES_EXPORT_H

#ifdef ERRORCODES_STATIC_DEFINE
#  define ERRORCODES_EXPORT
#  define ERRORCODES_NO_EXPORT
#else
#  ifndef ERRORCODES_EXPORT
#    ifdef errorCodes_EXPORTS
        /* We are building this library */
#      define ERRORCODES_EXPORT 
#    else
        /* We are using this library */
#      define ERRORCODES_EXPORT 
#    endif
#  endif

#  ifndef ERRORCODES_NO_EXPORT
#    define ERRORCODES_NO_EXPORT 
#  endif
#endif

#ifndef ERRORCODES_DEPRECATED
#  define ERRORCODES_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef ERRORCODES_DEPRECATED_EXPORT
#  define ERRORCODES_DEPRECATED_EXPORT ERRORCODES_EXPORT ERRORCODES_DEPRECATED
#endif

#ifndef ERRORCODES_DEPRECATED_NO_EXPORT
#  define ERRORCODES_DEPRECATED_NO_EXPORT ERRORCODES_NO_EXPORT ERRORCODES_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef ERRORCODES_NO_DEPRECATED
#    define ERRORCODES_NO_DEPRECATED
#  endif
#endif

#endif /* ERRORCODES_EXPORT_H */
