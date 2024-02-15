dilemma: main.cpp
	g++ -fsanitize=address -o dilemma main.cpp -I/usr/include/SDL2 -D_REENTRANT -lSDL2 -lSDL2_ttf

clean:
	rm -f dilemma

rebuild: clean dilemma
