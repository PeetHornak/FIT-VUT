
#ifndef QUEUE_EXPORT_H
#define QUEUE_EXPORT_H

#ifdef QUEUE_STATIC_DEFINE
#  define QUEUE_EXPORT
#  define QUEUE_NO_EXPORT
#else
#  ifndef QUEUE_EXPORT
#    ifdef queue_EXPORTS
        /* We are building this library */
#      define QUEUE_EXPORT 
#    else
        /* We are using this library */
#      define QUEUE_EXPORT 
#    endif
#  endif

#  ifndef QUEUE_NO_EXPORT
#    define QUEUE_NO_EXPORT 
#  endif
#endif

#ifndef QUEUE_DEPRECATED
#  define QUEUE_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef QUEUE_DEPRECATED_EXPORT
#  define QUEUE_DEPRECATED_EXPORT QUEUE_EXPORT QUEUE_DEPRECATED
#endif

#ifndef QUEUE_DEPRECATED_NO_EXPORT
#  define QUEUE_DEPRECATED_NO_EXPORT QUEUE_NO_EXPORT QUEUE_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef QUEUE_NO_DEPRECATED
#    define QUEUE_NO_DEPRECATED
#  endif
#endif

#endif /* QUEUE_EXPORT_H */
