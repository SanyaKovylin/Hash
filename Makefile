CC = gcc
LINK= ld
ASM = nasm

ASM_FLAGS = -f elf64
CC_FLAGS = -g -O3 -mavx512f -mavx512vl -I Include --no-inline
CC_FLAGS += -D NOFIND
CC_FLAGS += -fsanitize=address
# CC_FLAGS += -D NOSTRCMP
# CC_FLAGS += -D NOALIGN
# CC_FLAGS += -D NOHASH



Sources =  $(wildcard $(Source)/*.cpp)
Objects = $(patsubst $(Source)/%.cpp,$(Build)/%.o,$(Sources))
Sources1 =  $(wildcard $(Source1)/*.cpp)
Objects1 = $(patsubst $(Source1)/%.cpp,$(Build1)/%.o,$(Sources1))

Cpp = $(wildcard *.cpp)
H = $(wildcard *.h)

Build = Build
Build1 = BuildProc
Source = src
Source1 = process
Exe = Table
Proc = Process

all: clean $(Build)/$(Exe) $(Build1)/$(Proc)

$(Build1)/%.o : $(Source1)/%.cpp | $(Build1)
	$(CC) $(CC_FLAGS) $< -c -o $@

$(Build)/%.o : $(Source)/%.cpp | $(Build)
	$(CC) $(CC_FLAGS) $< -c -o $@

$(Build1)/$(Proc): $(Objects1) $(Objects) | $(Build)
	$(CC) $(CC_FLAGS) $(Main) -o $@ $(Objects1) Build/hash.o
	chmod 777 $(Build1)/$(Proc)

$(Build)/$(Exe): asm $(Objects) | $(Build)
	$(CC) $(CC_FLAGS) $(Main) -o $@ $(Build)/mystrcmp.o $(Objects)
	chmod 777 $(Build)/$(Exe)

$(Build):
	mkdir -p $(Build)

$(Build1):
	mkdir $(Build1)

asm: $(Build)
	$(ASM) $(ASM_FLAGS) $(Source)/mystrcmp.s -o $(Build)/mystrcmp.o

run: all
	$(Build)/$(Exe)

proc: all
	$(Build1)/$(Proc)

obj:
	$(CC) $(CC_FLAGS) Objects/obj.cpp -o Build/Objects
	Build/Objects

link:
	$(CC) $(CC_FLAGS) $(Main) -o $(Build)/$(Exe) mystrcmp.o $(Objects)
	chmod 777 $(Build)/$(Exe)

clean:
	rm -rf $(Build)
	rm -rf $(Build1)
	clear

perf:
	perf record -g $(Build)/$(Exe)
	perf report --sort=symbol --no-children

perfstat:
	perf stat -r 3 -d $(Build)/$(Exe)

time:
	hyperfine --warmup 3 -r 5 $(Build)/$(Exe)
