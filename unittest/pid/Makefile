
PROGRAM = test

OBJECTS_C = main.o pidr.o

FLAGS_C = -std=gnu99 -O2

FLAGS_LINKER = -lm

HEADERS =

INCLUDES = -I.. -I../../ev3dev-c-master/eg/script

VPATH = .:../../ev3dev-c-master/eg/script

CC = gcc

all: $(PROGRAM)

	
$(OBJECTS_C): %.o: %.c $(HEADERS) $(MAKEFILE)
	$(CC) $(FLAGS_C) $(INCLUDES) -c $< -o $@
	@echo compiling $<

$(PROGRAM): $(OBJECTS_C) $(HEADERS) $(MAKEFILE)
	$(CC) -o $@ $(OBJECTS_C) $(FLAGS_LINKER)
	@echo linking $@

		
	
clean:
	-rm -f *.o 	
	-rm -f $(PROGRAM)

