TARGET_CPPS := RunTests.cpp
CPP_FILES := $(filter-out $(TARGET_CPPS),$(wildcard *.cpp))
OBJ_FILES := $(CPP_FILES:.cpp=.o)

CPP_FLAGS = --std=c++17 -Wall -Werror -Wpedantic -Ofast -march=native
LDFLAGS = "-Wl,-rpath,./libdivsufsort/build/lib" -L./libdivsufsort/build/lib -l:libdivsufsort.so

all: run-tests

$(OBJ_FILES) $(TARGET_CPPS:.cpp=.o): Makefile

run-tests: $(OBJ_FILES) RunTests.o
	g++ -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	g++ -c $(CPP_FLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -f *.o run-tests *~
