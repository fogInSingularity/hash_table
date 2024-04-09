CXX = gcc

EXE = hash_table

SOURCES = src/source/* src/lib_code/source/*
INCLUDE = -Isrc/include/ -Isrc/lib_code/include/

WARNINGS = -Wall -Wextra -Waggressive-loop-optimizations          \
-Wmissing-declarations -Wcast-align -Wcast-qual                               \
-Wchar-subscripts -Wconversion                                                \
-Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security              \
-Wformat-signedness -Wformat=2 -Winline -Wlogical-op                          \
-Wopenmp-simd -Wpacked -Wpointer-arith -Winit-self                            \
-Wredundant-decls -Wshadow -Wsign-conversion                                  \
-Wstrict-overflow=2 -Wsuggest-attribute=noreturn                              \
-Wsuggest-final-methods -Wsuggest-final-types                                 \
-Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code         \
-Wunused -Wvariadic-macros                                                    \
-Wno-missing-field-initializers -Wno-narrowing                                \
-Wno-varargs -Wstack-usage=8192 -Wstack-protector 

FLAGS_GCC = -std=c2x -fstack-protector-strong -fcheck-new -fstrict-overflow $(WARNINGS)
FLAGS_CLANG = -std=c2x -fstack-protector-strong -fcheck-new -fstrict-overflow -Wall -Wextra

ASAN_FLAGS = -fsanitize=address,bool,bounds,enum,float-cast-overflow,$\
float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,$\
object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,$\
undefined,unreachable,vla-bound,vptr

O_LEVEL = -O2
MARCH = -march=znver1 -mavx2
LIBRARY = -lm

PROFILE = -fprofile-use
VEC_FLAGS_GCC = -fopt-info-vec-optimized -fopt-info-vec-missed
VEC_FLAGS_CLANG = -Rpass=loop-vectorize -Rpass-analysis=loop-vectorize -Rpass-missed=loop-vectorize -fsave-optimization-record
# -Rpass=.* -Rpass-analysis=.* -Rpass-missed=.* -fsave-optimization-record

DEBUG_FLAGS = $(FLAGS_GCC) $(_ASAN_FLAGS) $(O_LEVEL) $(MARCH) $(LIBRARY) -g -ggdb -D_FORTIFY_SOURCE=2
RELEASE_FLAGS = $(FLAGS_GCC) $(O_LEVEL) $(MARCH) $(LIBRARY) -s -flto -DNDEBUG -fno-omit-frame-pointer 

all:
	@$(CXX) $(INCLUDE) $(SOURCES) $(DEBUG_FLAGS) -o $(EXE) 

release:
	@$(CXX) $(INCLUDE) $(SOURCES) $(RELEASE_FLAGS) -o $(EXE) 

clang:
	@clang $(INCLUDE) $(FLAGS_CLANG) $(SOURCES) $(O_LEVEL) $(LIBRARY) $(MARCH) -s -flto -DNDEBUG -fno-omit-frame-pointer  -o $(EXE)

asm:
	@$(CXX) $(INCLUDE) $(SOURCES) $(RELEASE_FLAGS) -s -S

analyze:
	@clang-tidy $(SOURCES) -checks=performance-*

bench_build:
	@clang $(INCLUDE) $(FLAGS_CLANG) $(SOURCES) $(O_LEVEL) $(LIBRARY) $(MARCH) -flto -DNDEBUG -fno-omit-frame-pointer -o $(EXE)

graph:
	@python graph.py measures/alw_zero measures/alw_fchr measures/len measures/ch_sum measures/norm  measures/ror measures/rol measures/based

perf_rec:
	@sudo perf record --call-graph dwarf ./hash_table bible.txt 2>log

hotspot:
	@sudo hotspot perf.data