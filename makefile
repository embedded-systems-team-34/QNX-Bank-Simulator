HEADERS = customer.h queue.h
OBJECTS = main.o queue.o

default: main

%.o: %.c $(HEADERS)
	gcc -c $< -o $@

main: $(OBJECTS)
	gcc $(OBJECTS) -o $@

clean:
	rm -f $(OBJECTS) main