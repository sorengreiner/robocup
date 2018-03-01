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

	while(true)
	{
		uint8_t keys = KeysRead();

		printf("keys=%02X\n", keys);
	
		if(keys == EV3_KEY_BACK)
		{
			break;
		}
	
		sleep_ms(10);
	}

	KeysClose();

	printf( "Goodbye\n" );

	return 0;
}
