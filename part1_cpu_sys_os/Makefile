HOST_COMPILER	:= g++
CCFLAGS		:= -std=c++11 -O0
LDFLAGS		:= -pthread

EXE		= cpu_sys_os.x




all: clean build

build: $(EXE)

%.x: %.cpp
	$(HOST_COMPILER) $(CCFLAGS) $(LDFLAGS) -o $@ $<

     
clean:
	rm -rf $(EXE)
