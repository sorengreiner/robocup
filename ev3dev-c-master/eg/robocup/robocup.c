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

	SRemote remote;
	if(!RemoteOpen(&remote, INPUTPORT_4))
	{
		printf("remote wont open\n");
		return 1;
	}

	while(true)
	{
		uint8_t keys = KeysRead();
		uint8_t remoteStatus = RemoteRead(&remote);

		printf("keys=%02X remote=%02X\n", keys, remoteStatus);
	
		if(keys == EV3_KEY_BACK)
		{
			break;
		}
	
		sleep_ms(10);
	}

	KeysClose();
	RemoteClose(&remote);

	printf( "Goodbye\n" );

	return 0;
}
