CC=g++ -std=c++11
CFLAGS=-Wall -Wextra -Werror -Wno-reorder
ifeq ($(PROD), "true")
	CFLAGS += -03
else
	CFLAGS += -g
endif
COM=$(CC) $(CFLAGS)
SOURCES := $(wildcard src/*.cpp)
SOURCES := $(filter-out $(wildcard src/*_L.cpp), $(SOURCES))
OBJ_FILES := $(patsubst %.cpp,%.o,$(SOURCES))
I=-Iinclude -I/usr/include/lua5.3
EXECUTABLE = game
ifeq ($(shell uname),Linux)
LIBRARIES=-L. -lSDL2main -lSDL2 -lSDL2_image -lm -llua -ldl -lGL
else
LIBRARIES=-L. -lSDL2main -lSDL2 -lOpengl32 -lglu32 -lSDL2_image -llua53
I+=-Iluasrc/src
endif

.PHONY: clean all

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(COM) $^ $(LIBRARIES) -o $(EXECUTABLE).exe

src/%.o: src/%.cpp
	$(COM) $(D) $(I) -c -o $@ $<

clean:
	rm -f src/*.o
	rm -f src/*.d
	rm -f $(EXECUTABLE).exe
