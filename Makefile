LIBS  = -lraylib -lm -ldl -lglfw -lGL -lopenal -pthread -ldl
SRC=$(wildcard *.c)
CFLAGS = -Wall -Wno-unused-variable -Wno-unused-function -Wno-unused-but-set-variable -Wno-unused-value -Wno-unused-label -Wno-unused-parameter -Wno-unused-result

run: compile
	./game.out

compile: $(SRC)
	gcc -o game.out $^ $(CFLAGS) $(LIBS)

valgrind: game.out
	valgrind --leak-check=full \
			--show-leak-kinds=all \
			--track-origins=yes \
			--verbose \
			./game.out

debug: $(SRC)
	gcc -o game.out $^ -g $(CFLAGS) $(LIBS)

