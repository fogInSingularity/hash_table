#ifndef FASTER_STRNCMP_H_
#define FASTER_STRNCMP_H_

#include "llist_config.h"
#include "opt.h"

HOT int FasterStrcmp(const char word_key1[kWordMaxLen], 
                     const char word_key2[kWordMaxLen]);

#endif // FASTER_STRNCMP_H_