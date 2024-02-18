dilemma: main.cpp
	g++ -o dilemma main.cpp -I/usr/include -D_REENTRANT -lSDL2 -lSDL2_ttf

clean:
	rm -f dilemma

rebuild: clean dilemma
