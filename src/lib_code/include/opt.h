#ifndef OPT_H_
#define OPT_H_

#define OPT_ON

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
    #elif defined (__GNUC__)
        #define LIKELY 
        #define UNLIKELY 
        #define HOT __attribute__((hot))
    #else
        #define LIKELY 
        #define UNLIKELY 
        #define HOT
    #endif
#else
    #define LIKELY 
    #define UNLIKELY 
    #define HOT
#endif // OPT_ON

#endif // OPT_H_