#ifndef PARSE_FILE_H_
#define PARSE_FILE_H_

#include "bin_file.h"
#include "my_typedefs.h"
#include "my_assert.h"

FatPointer ParseFile(const int argc, const char** argv, BinData* ret_bin_data);

#endif  // PARSE_FILE_H_