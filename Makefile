CC = gcc
LINK= ld
ASM = nasm
ASM_FLAGS = -f elf64
Main =
LINK_FLAGS =
CC_FLAGS = -g -O3 -mavx512f -mavx512vl -I Include
# CC_FLAGS += -fsanitize=address -lasan# -I /lib64/gcc/x86_64-pc-linux-gnu/14.2.1/include/sanitizer


Sources =  $(wildcard $(Source)/*.cpp)
Objects = $(patsubst $(Source)/%.cpp,$(Build)/%.o,$(Sources))

Cpp = $(wildcard *.cpp)
H = $(wildcard *.h)

Build = Build
Source = src
Exe = Compiler

all: clean $(Build)/$(Exe)


$(Build)/%.o : $(Source)/%.cpp | $(Build)
	$(CC) $(CC_FLAGS) $< -c -o $@

$(Build)/$(Exe): $(Objects) | $(Build)
	$(CC) $(CC_FLAGS) $(Main) -o $(Build)/$(Exe) $(Objects)
	chmod 777 $(Build)/$(Exe)

$(Build):
	mkdir $(Build)

asm: $(Build)
	$(ASM) $(ASM_FLAGS) -l a.lst $(Source)/printf.s -o $(Build)/myprintf.o

run:
	$(Build)/$(Exe)

clean:
	rm -rf $(Build)
	clear

