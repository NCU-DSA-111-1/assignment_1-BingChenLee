build = $(wildcard build/*.o)

LDFLAGS = -pthread -lpthread
CFLAGS = -g -Wall -Werror

FORCE: main

main: main.o layer.o neuron.o
	$(CC) $(LDFLAGS) $(build) -o bin/main -lm

layer.o: 
	$(CC) $(CFLAGS) -c src/layer.c -o build/layer.o

main.o: 
	$(CC) $(CFLAGS) -c src/main.c -o build/main.o

neuron.o: 
	$(CC) $(CFLAGS) -c src/neuron.c -o build/neuron.o

# remove object files and executable when user executes "make clean"
clean:
	rm *.o backprop
