#ifndef MMAP_WRAPER_H_
#define MMAP_WRAPER_H_

#if defined(__linux__)
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#else
#error "Unable to use mmap"
#endif // __linux__

#endif // MMAP_WRAPER_H_