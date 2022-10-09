CC=g++

CFLAGS+=-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wmissing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE

ASM_OBJ+=asm.o flog.o asm_funcs.o text.o

PROC_OBJ+=proc.o proc_funcs.o stack.o text.o flog.o

all: wtfasm.exe wtfproc.exe

wtfproc.exe: $(PROC_OBJ)
	$(CC) $(PROC_OBJ) $(CFLAGS) -o wtfproc.exe

wtfasm.exe: $(ASM_OBJ)
	$(CC) $(ASM_OBJ) $(CFLAGS) -o wtfasm.exe

*.o: *.cpp
	$(CC) $(CFLAGS) *.cpp -c

clean:
	rm -rf *.o *.exe*
	clear