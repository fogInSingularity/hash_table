#include "faster_strcmp.h"
#include <immintrin.h>
#include "opt.h"

HOT int FasterStrcmp(const char word_key1[kWordMaxLen], 
                     const char word_key2[kWordMaxLen]) {
    __m256i load1 = _mm256_loadu_si256((const __m256i*)word_key1);
    __m256i load2 = _mm256_loadu_si256((const __m256i*)word_key2);
    __m256i xor_vec = _mm256_xor_si256(load1, load2);
                                       
    return !_mm256_testz_si256(xor_vec, xor_vec);
}