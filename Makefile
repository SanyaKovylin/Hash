CC = gcc
LINK= ld
ASM = nasm
ASM_FLAGS = -f elf64 
Main = 
LINK_FLAGS = 
CC_FLAGS = -g -O2 -mavx512f -mavx512vl -I Include -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers  -Wpointer-arith -Wstack-usage=8192 -Wstrict-aliasing -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

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
	$(CC) $(LINK_FLAGS) $(Main) -o $(Build)/$(Exe) $(Objects)
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

