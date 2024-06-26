default_flags = """
# ------------------------------------------
#
# THIS MAKE FILE GENERATED BY make_make.py
#
# ------------------------------------------
EXE = hash_table
EXE_ARGS = bible parsed
EXE_LOG = 2>log

ASM = nasm

WARNINGS = -Wall -Wextra

INCLUDE = -Isrc/include/ -Isrc/lib_code/include/

FLAGS = -std=c2x -fstack-protector-strong -fcheck-new -fstrict-overflow -masm=intel $(WARNINGS)

ASAN_FLAGS = -fsanitize=address,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

O_LEVEL = -O2
MARCH = -march=znver1 -mavx2
LIBRARY = -lm # for clang

DEBUG_FLAGS = $(FLAGS) $(ASAN_FLAGS) -Og -g3 $(MARCH) -DDEBUG -ggdb -D_FORTIFY_SOURCE=2
RELEASE_FLAGS = $(FLAGS) $(O_LEVEL) $(MARCH) -DNDEBUG -fomit-frame-pointer -g3 -flto

LINK_FLAGS_DEBUG = $(LIBRARY) $(ASAN_FLAGS) -DDEBUG -ggdb -g3
LINK_FLAGS_RELEASE = $(LIBRARY) -DNDEBUG -g3 -flto

ASM_FLAGS = -Werror -g -f elf64

# profiling:
# PROFILE = -fprofile-use
# VEC_FLAGS_GCC = -fopt-info-vec-optimized -fopt-info-vec-missed
# VEC_FLAGS_CLANG = -Rpass=loop-vectorize -Rpass-analysis=loop-vectorize -Rpass-missed=loop-vectorize -fsave-optimization-record
# -Rpass=.* -Rpass-analysis=.* -Rpass-missed=.* -fsave-optimization-record

# find . -type f -name "*.<file_ext>"
# find . -type f -name "*.<file_ext>" -printf "./build/%f\\n" | sort
"""

c_files = "./src/source/main.c ./src/source/hash_table.c ./src/source/load_file.c ./src/source/hash.c ./src/lib_code/source/utils.c ./src/lib_code/source/my_assert.c ./src/lib_code/source/bin_file.c ./src/lib_code/source/darray.c ./src/lib_code/source/llist.c ./src/lib_code/source/recalloc.c ./src/lib_code/source/faster_strcmp.c ./src/lib_code/source/file_wraper.c"
c_objects = "./src/build/main.o ./src/build/hash_table.o ./src/build/load_file.o ./src/build/hash.o ./src/lib_code/build/utils.o ./src/lib_code/build/my_assert.o ./src/lib_code/build/bin_file.o ./src/lib_code/build/darray.o ./src/lib_code/build/llist.o ./src/lib_code/build/recalloc.o ./src/lib_code/build/faster_strcmp.o ./src/lib_code/build/file_wraper.o"

asm_sources = ""
asm_objects = ""

# all target: ------------------------------------------------------------------
clean_dirs = "rm src/build/* src/lib_code/build/*"
all_target = """
all: {}
\t@$(CC) {} -o $(EXE) $(LINK_FLAGS_DEBUG)\n""".format(asm_objects + ' ' + c_objects, asm_objects + ' ' + c_objects)
release_target = """
all: {}
\t@$(CC) {} -o $(EXE) $(LINK_FLAGS_RELEASE)
""".format(asm_objects + ' ' + c_objects, asm_objects + ' ' + c_objects)

all_target += '\t@' + clean_dirs + '\n\n'
release_target += '\t@' + clean_dirs + '\n\n'
clean_target = """
clean:
\t"""
clean_target += clean_dirs
# ------------------------------------------------------------------------------

# objects for debug: -----------------------------------------------------------
asm_obj_debug = ''
c_obj_debug = ''
c_sources_splited = c_files.split()
c_objects_splited = c_objects.split()
asm_sources_splited = asm_sources.split()
asm_objects_splited = asm_objects.split()

c_ns_files = len(c_sources_splited)
c_no_files = len(c_objects_splited)
asm_ns_files = len(asm_sources_splited)
asm_no_files = len(asm_objects_splited)

if (asm_no_files != asm_ns_files) or (c_no_files != c_ns_files):
    print("fuck")
    exit(0)

for i in range(asm_no_files):
    asm_obj_debug += "{}: {}\n\t@$(ASM) $^ $(ASM_FLAGS) -o $@\n\n".format(asm_objects_splited[i], asm_sources_splited[i])

for i in range(c_no_files):
    c_obj_debug += "{}: {}\n\t@$(CC) -c $^ $(DEBUG_FLAGS) $(INCLUDE) -o $@\n\n".format(c_objects_splited[i], c_sources_splited[i])

# objects for release: ---------------------------------------------------------

asm_obj_release = ''
c_obj_release = ''
c_sources_splited = c_files.split()
c_objects_splited = c_objects.split()
asm_sources_splited = asm_sources.split()
asm_objects_splited = asm_objects.split()

c_ns_files = len(c_sources_splited)
c_no_files = len(c_objects_splited)
asm_ns_files = len(asm_sources_splited)
asm_no_files = len(asm_objects_splited)

if (asm_no_files != asm_ns_files) or (c_no_files != c_ns_files):
    print("fuck")
    exit(0)

for i in range(asm_no_files):
    asm_obj_release += "{}: {}\n\t@$(ASM) $^ $(ASM_FLAGS) -o $@\n\n".format(asm_objects_splited[i], asm_sources_splited[i])

for i in range(c_no_files):
    # c_obj_release += "{}: {}\n\t@$(CC) -c $^ $(DEBUG_FLAGS) $(INCLUDE) -o $@\n\n".format(c_objects_splited[i], c_sources_splited[i])
    c_obj_release += "{}: {}\n\t@$(CC) -c $^ $(RELEASE_FLAGS) $(INCLUDE) -o $@\n\n".format(c_objects_splited[i], c_sources_splited[i])


# aditional utils: -------------------------------------------------------------

utils = """
EXE = hash_table
EXE_ARGS = bible parsed
EXE_LOG = 2>log

#analyze:
#	@clang-tidy $(SOURCES) -checks=performance-*

graph:
	@python graph.py measures/alw_zero measures/alw_fchr measures/len measures/norm measures/ch_sum measures/crc measures/rol measures/mur

perf_rec:
	@sudo perf record --call-graph dwarf ./$(EXE) $(EXE_ARGS) $(EXE_LOG)

hotspot:
	@sudo hotspot perf.data

val:
	@valgrind --tool=callgrind --dump-instr=yes ./$(EXE) $(EXE_ARGS)

kch:
	@kcachegrind
"""

# ------------------------------------------------------------------------------

sep = "# ------------------------------------------------------------------------------\n"

make_file_name = "makefile"
make_file_debug_name = "debug_make"
make_file_release_name = "release_make"

make_file = open(make_file_name, "w")
make_file_debug = open(make_file_debug_name, "w")
make_file_release = open(make_file_release_name, "w")

make_file.write("""
# ------------------------------------------
#
# THIS MAKE FILE GENERATED BY make_make.py
#
# ------------------------------------------

debug: all
all:
\t@echo 'Compiler used: $(CC)'
\t@make -f debug_make CC=$(CC)

release:
\t@echo 'Compiler used: $(CC)'
\t@make -f release_make CC=$(CC)
""" + clean_target + '\n' + utils)
# make_file.write(default_flags + sep + all_target + release_target + sep + c_obj_debug + asm_obj_debug + sep + c_obj_release + asm_obj_release + sep + utils)
make_file_debug.write(default_flags + sep + all_target + sep + c_obj_debug + asm_obj_debug)
make_file_release.write(default_flags + sep + release_target + sep + c_obj_release + asm_obj_release)
