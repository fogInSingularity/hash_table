SOURCES = *.c
FLAGS = -Wall -Wextra
ASAN_FLAGS = -fsanitize=address,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
INCLUDE = -I../src/include/ -I../src/lib_code/include/

all:
	@gcc $(SOURCES) $(FLAGS) $(ASAN_FLAGS) $(INCLUDE) -o file_parser -g 