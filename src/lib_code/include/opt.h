#ifndef OPT_H_
#define OPT_H_

#ifndef DEBUG
#define OPT_ON
#endif // DEBUG

#if defined (OPT_ON)
    // check for c2x clang
    #if defined (__clang__)
        #if __STDC_VERSION__ > 201710L
            #define LIKELY [[clang::likely]]
            #define UNLIKELY [[clang::unlikely]]
        #else
            #define LIKELY
            #define UNLIKELY
        #endif
        #define HOT __attribute__((hot))
        #define NOINLINE __attribute__((noinline))
        #define INLINE __attribute__((always_inline))
        #define ALIGN(n) __attribute__((aligned(n)))
    #elif defined (__GNUC__)
        #define LIKELY
        #define UNLIKELY
        #define HOT __attribute__((hot))
        #define INLINE __attribute__((always_inline))
        #define NOINLINE __attribute__((noinline))
        #define ALIGN(n) __attribute__((aligned(n)))
    #else
        #define LIKELY
        #define UNLIKELY
        #define HOT
        #define INLINE
        #define NOINLINE
        #define ALIGN(n)
    #endif
#else
    #define LIKELY
    #define UNLIKELY
    #define HOT
    #define INLINE
    #define NOINLINE
    #define ALIGN(n)
#endif // OPT_ON

// #undef NOINLINE
// #define NOINLINE

#endif // OPT_H_
