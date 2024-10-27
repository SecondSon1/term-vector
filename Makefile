CC=clang++
SRC=$(shell find src -name "*.cpp")
OBJ=$(SRC:.cpp=.o)
BIN=bin
APP=vector

INCFLAGS=
LIBFLAGS=
CCFLAGS=-std=c++20 -Wextra -Wshadow -Wconversion -Wfloat-equal -g
CCFLAGS+=-fsanitize=undefined,bounds,address

all: build

dirs:
	mkdir -p $(BIN)

%.o: %.cpp
	$(CC) -o $@ -c $< $(INCFLAGS) $(CCFLAGS)

build: dirs $(OBJ)
	$(CC) -o $(BIN)/$(APP) $(filter %.o,$^) $(LIBFLAGS) $(CCFLAGS)

run: build
	$(BIN)/$(APP)

clean:
	rm -rf $(OBJ)
	rm -rf $(BIN)
