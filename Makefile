CC         = g++
SRC        = $(wildcard *.cpp) $(wildcard src/*/*.cpp)
OBJ        = $(addprefix obj/,$(notdir $(SRC:.cpp=.o)))  
DEPS       = $(SRC:%.cpp=obj/%.d)
CFLAGS     = -std=c++1y -Wall -fPIC -g
LDFLAGS    = -lSDL2 -lSDL2_image -lSDL2_ttf -L/usr/local/lib -lpthread -ldl
SQLITE_C   = sqlite/sqlite3.c
SQLITE_OBJ = obj/sqlite/sqlite3.o

EXE = pokedex

.PHONY: all clean run

all: $(EXE)

$(EXE): $(OBJ) $(SQLITE_OBJ)
	$(CC) $(OBJ) $(SQLITE_OBJ) $(CFLAGS) $(LDFLAGS) -o $@

obj/sqlite/sqlite3.o: $(SQLITE_C) | obj/sqlite
	gcc -c $(SQLITE_C) -lpthread -ldl -Isqlite/ -o $@

obj/%.o: %.cpp | obj
	$(CC) -MMD -MP -c $< $(CFLAGS) -o $@ 

obj/sqlite: obj
	mkdir obj/sqlite

obj:
	mkdir obj

run:
	./$(EXE)

clean:
	rm -r obj

-include $(DEPS)
