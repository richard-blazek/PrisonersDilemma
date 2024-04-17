dilemma: main.c libs/io.c libs/player.c libs/players.c
	gcc -o dilemma main.c libs/io.c libs/player.c libs/players.c -I/usr/include -D_REENTRANT -lSDL2 -lSDL2_ttf

clean:
	rm -f dilemma

rebuild: clean dilemma
