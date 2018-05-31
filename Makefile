HOST_COMPILER	:= g++
CCFLAGS		:= -std=c++11 -O0
LDFLAGS		:= -pthread

EXE		= cpu_sys_os.x memory.x memory_bandwidth.x pagefault.x




all: clean build

build: $(EXE)

%.x: %.cpp
	$(HOST_COMPILER) $(CCFLAGS) $(LDFLAGS) -o $@ $<

     
clean:
	rm -rf $(EXE)
