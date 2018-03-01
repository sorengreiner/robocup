#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "mindstorm.h"
#include "car.h"


int main( void )
{
	printf( "Robocup\n" );

	if(!KeysOpen())
	{
		printf("keys wont open\n");
		return 1;
	}

	for(int i = 0; i < 100; i++)
	{
		uint8_t keys = KeysRead();

		printf("keys=%0X2\n", keys);
	
		sleep_ms(100);
	}

	KeysClose();

	printf( "Goodbye\n" );

	return 0;
}
