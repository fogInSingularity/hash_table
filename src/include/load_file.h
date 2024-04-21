#ifndef LOAD_FILE_H_
#define LOAD_FILE_H_

#include "my_typedefs.h"
#include "bin_file.h"

FatPointer LoadFile(const int argc, 
                    const char** argv, 
                    BinData* file_data, 
                    int* mapped_fd);

#endif // LOAD_FILE_H_