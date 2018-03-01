
all:
	arm-linux-gnueabi-gcc -std=gnu99 -O2 -o robocup.o -c robocup.c
	arm-linux-gnueabi-gcc -std=gnu99 -O2 -o car.o -c car.c
	arm-linux-gnueabi-gcc -std=gnu99 -O2 -o mindstorm.o -c mindstorm.c
	arm-linux-gnueabi-gcc -std=gnu99 -O2 -o robo robocup.o car.o mindstorm.o -lm -lev3dev-c -pthread

clean:
	-rm -f *.o 	
	-rm -f robo
