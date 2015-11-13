CPP_CODE_FILES := $(wildcard *.cpp)
CPP_OBJ_FILES := $(addsuffix .o,$(basename $(CPP_CODE_FILES)))
H_FILES := $(wildcard *.h)
BINARY_FILES := $(basename $(CPP_CODE_FILES))

.PHONY: all
.SUFFIXES:

all: $(BINARY_FILES)


%: %.o
	g++ $< -std=c++11 -o $@ -pthread

%.o: %.cpp $(H_FILES)
	@echo "Building $@"
	g++ $< -std=c++11 -c -o $@ -g

1: socketDemo1 killParent
	python run.py 1

2: socketDemo2 killParent
	python run.py 2

3: socketDemo3 killParent
	python run.py 3

4: socketDemo4 killParent
	python run.py 4

clean:
	rm -f socketDemo1 socketDemo2 socketDemo3 socketDemo4 killParent *.o
