all: main

main: main.o game_state.o valid_input.o color.o
	$(CC) $(CFLAGS) -o cyfish main.o game_state.o valid_input.o color.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o cyfish
