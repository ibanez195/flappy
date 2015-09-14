all:
	gcc -g -Wall src/main.c -o flappy -lncurses -lm
clean:
	rm flappy
