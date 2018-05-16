all:
	g++ -o0 -o test -std=c++11 memory_bandwidth.cpp
	g++ -pthread -o0 -o test1 -std=c++11 cpu.cpp
