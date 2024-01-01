LIBS  = -lraylib -lm -ldl -lglfw -lGL -lopenal -pthread -ldl -luuid
SRC=$(wildcard *.c)
CFLAGS = -Wall -Wno-unused-variable -Wno-unused-function -Wno-unused-but-set-variable -Wno-unused-value -Wno-unused-label -Wno-unused-parameter -Wno-unused-result

run: compile
	./game.out

compile: $(SRC)
	gcc -o game.out $^ $(CFLAGS) $(LIBS)

valgrind: $(SRC)
	gcc -o game.out $^ -g $(CFLAGS) $(LIBS)
	valgrind -q --track-origins=yes ./game.out


valgrind-long: debug
	valgrind --leak-check=full \
			--show-leak-kinds=all \
			--track-origins=yes \
			--verbose \
			./game.out

debug: $(SRC)
	gcc -o game.out -fsanitize=address $^ -g $(CFLAGS) $(LIBS)

gdb: debug
	gdb ./game.out core

