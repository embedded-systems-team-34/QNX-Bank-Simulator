HEADERS = customer.h queue.h
OBJECTS = main.o queue.o

default: main

%.o: %.c $(HEADERS)
	gcc -c $< -pthread -lrt -o $@

main: $(OBJECTS)
	gcc $(OBJECTS) -pthread -lrt -o $@

clean:
	rm -f $(OBJECTS) main