CC=g++

CFLAGS+=-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wmissing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE

ASM_OBJ+=asm/asm.o lib/flog.o asm/asm_funcs.o lib/text.o

PROC_OBJ+=processor/proc.o processor/proc_funcs.o lib/stack.o lib/text.o lib/flog.o

all: wtfasm.exe wtfproc.exe

wtfproc.exe: $(PROC_OBJ)
	$(CC) $(PROC_OBJ) $(CFLAGS) -o wtfproc.exe

wtfasm.exe: $(ASM_OBJ)
	$(CC) $(ASM_OBJ) $(CFLAGS) -o wtfasm.exe

*.o: *.cpp
	$(CC) $(CFLAGS) *.cpp -c

clean:
	rm -rf asm/*.o asm/*.exe*
	rm -rf processor/*.o processor/*.exe*
	rm -rf lib/*.o lib/*.exe*
	rm -rf *.o *.exe*
	clear

run:
	make
	./wtfasm.exe program.code
	./wtfproc.exe a.wtf

.PHONY: clean run