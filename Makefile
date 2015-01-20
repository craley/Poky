CC      = g++
SRC     = $(wildcard *.cpp) $(wildcard src/*/*.cpp)
OBJ     = $(addprefix obj/,$(notdir $(SRC:.cpp=.o)))  
DEPS    = $(SRC:%.cpp=obj/%.d)
CFLAGS  = -std=c++1y -Wall -fPIC -g
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -L/usr/local/lib

EXE = pokedex

.PHONY: all clean run

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(LDFLAGS) -o $@

obj/%.o: %.cpp | obj
	$(CC) -MMD -MP -c $< $(CFLAGS) -o $@ 

obj:
	mkdir obj

run:
	./$(EXE)

clean:
	rm $(OBJ) $(DEPS)

-include $(DEPS)
