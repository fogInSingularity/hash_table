#ifndef MMAP_WRAPER_H_
#define MMAP_WRAPER_H_

#if defined(_WIN32)
#error "Unable to use mmap"
#else
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

#endif // MMAP_WRAPER_H_