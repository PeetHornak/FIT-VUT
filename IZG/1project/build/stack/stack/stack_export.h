
#ifndef STACK_EXPORT_H
#define STACK_EXPORT_H

#ifdef STACK_STATIC_DEFINE
#  define STACK_EXPORT
#  define STACK_NO_EXPORT
#else
#  ifndef STACK_EXPORT
#    ifdef stack_EXPORTS
        /* We are building this library */
#      define STACK_EXPORT 
#    else
        /* We are using this library */
#      define STACK_EXPORT 
#    endif
#  endif

#  ifndef STACK_NO_EXPORT
#    define STACK_NO_EXPORT 
#  endif
#endif

#ifndef STACK_DEPRECATED
#  define STACK_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef STACK_DEPRECATED_EXPORT
#  define STACK_DEPRECATED_EXPORT STACK_EXPORT STACK_DEPRECATED
#endif

#ifndef STACK_DEPRECATED_NO_EXPORT
#  define STACK_DEPRECATED_NO_EXPORT STACK_NO_EXPORT STACK_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef STACK_NO_DEPRECATED
#    define STACK_NO_DEPRECATED
#  endif
#endif

#endif /* STACK_EXPORT_H */
