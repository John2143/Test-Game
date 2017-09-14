CC=g++ -std=c++11 -MMD
CFLAGS=-Wall -Wextra
COM=$(CC) $(CFLAGS)
D=
SOURCES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(SOURCES:.cpp=.o)))

EXECUTABLE = game
ifeq ($(shell uname),Linux)
LIBRARIES=-L. -lSDL2main -lSDL2 -lSDL2_image -lm -L./luasrc/src -llua -ldl -lGL
else
LIBRARIES=-L. -lSDL2main -lSDL2 -lOpengl32 -lglu32 -lSDL2_image -llua53
endif

.PHONY: clean all

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(COM) $^ $(LIBRARIES) -o $(EXECUTABLE).exe

obj/%.o: src/%.cpp
	$(COM) $(D) -Iinclude -c -o $@ $<

clean:
	rm obj/*.o
	rm obj/*.d
	rm $(EXECUTABLE).exe
