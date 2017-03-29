HEADERS = customer.h queue.h teller.h stats.h
OBJECTS = main.o queue.o teller.o stats.o

default: main

%.o: %.c $(HEADERS)
	gcc -c $< -pthread -lrt -o $@    
    
main: $(OBJECTS)
	gcc $(OBJECTS) -pthread -lrt -o $@

clean:
	rm -f $(OBJECTS) main