
PROGRAM = roborun

OBJECTS_C = robocup.o magnus.o line.o keys.o

FLAGS_C = -std=gnu99 -O2

FLAGS_LINKER = -lm -lev3dev-c -pthread

HEADERS =

INCLUDES = -I.. -I../../../source/ev3

VPATH = ..

CC = arm-linux-gnueabi-gcc

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

