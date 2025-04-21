CC = gcc
LINK= ld
ASM = nasm
ASM_FLAGS = -f elf64
# Main =
# LINK_FLAGS = -g -I Include
CC_FLAGS = -g -O3 -mavx512f -mavx512vl -I Include
# CC_FLAGS += -fsanitize=address -lasan# -I /lib64/gcc/x86_64-pc-linux-gnu/14.2.1/include/sanitizer

# CC_FLAGS += -D NOSTRCMP
# CC_FLAGS += -D NOHASH
CC_FLAGS += -D NOLOAD

Sources =  $(wildcard $(Source)/*.cpp)
Objects = $(patsubst $(Source)/%.cpp,$(Build)/%.o,$(Sources))
Sources1 =  $(wildcard $(Source1)/*.cpp)
Objects1 = $(patsubst $(Source1)/%.cpp,$(Build1)/%.o,$(Sources1))

Cpp = $(wildcard *.cpp)
H = $(wildcard *.h)

Build = Build
Source = src
Source1 = process
Exe = Table
Proc = Process

all: clean $(Build)/$(Exe) $(Build)/$(Proc)

$(Build)/%.o : $(Source)/%.cpp | $(Build)
	$(CC) $(CC_FLAGS) $< -c -o $@

$(Build1)/%.o : $(Source1)/%.cpp | $(Build1)
	$(CC) $(CC_FLAGS) $< -c -o $@

$(Build)/$(Proc): $(Objects1) $(Objects) | $(Build)
	$(CC) $(CC_FLAGS) $(Main) -o $@ $(Objects1) Build/hash.o
	chmod 777 $(Build)/$(Proc)

$(Build)/$(Exe): asm $(Objects) | $(Build)
	$(CC) $(CC_FLAGS) $(Main) -o $@ $(Build)/mystrcmp.o $(Objects)
	chmod 777 $(Build)/$(Exe)

# $(Build)/$(Proc): $(Objects1) | $(Build)
# 	$(CC) $(CC_FLAGS) $(Main) -o $@ $<
# 	chmod 777 $(Build)/$(Proc)

$(Build):
	mkdir $(Build)

asm: $(Build)
	$(ASM) $(ASM_FLAGS) -l a.lst $(Source)/mystrcmp.s -o $(Build)/mystrcmp.o

run: all
	$(Build)/$(Exe)

proc: all
	$(Build)/$(Proc)

clean:
	rm -rf $(Build)
	clear

