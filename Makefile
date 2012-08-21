
all: ./events.so
	gcc bot.c -o bot -ldl -lircclient -lssl -rdynamic

clean:
	rm -f *.o bot *.so

./events.so:
	./make_shared

run:
	LD_LIBRARY_PATH="." ./bot $(SERVER)
