
OBJECTS_C = robocup.o car.o mindstorm.o

FLAGS_C = -std=gnu99 -O2

FLAGS_LINKER = -lm -lev3dev-c -pthread

HEADERS =

INCLUDES = -I..

VPATH = ..

CC = arm-linux-gnueabi-gcc

all: robocup

	
$(OBJECTS_C): %.o: %.c $(HEADERS) $(MAKEFILE)
	$(CC) $(FLAGS_C) $(INCLUDES) -c $< -o $@
	@echo compiling $<

robocup: $(OBJECTS_C) $(HEADERS) $(MAKEFILE)
	$(CC) -o $@ $(OBJECTS_C) $(FLAGS_LINKER)
	@echo linking $@

	
.PHONY: test
test:
	arm-linux-gnueabi-gcc -std=gnu99 -O2 -o test.o -c test.c
	arm-linux-gnueabi-gcc -std=gnu99 -O2 -o test test.o -lm -pthread	
	
	
clean:
	-rm -f *.o 	
	-rm -f robocup
	-rm -f test
