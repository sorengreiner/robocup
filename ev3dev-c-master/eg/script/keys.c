#include "keys.h"

#include <stdbool.h>
#include <stdint.h>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <string.h>
#include <stdio.h>



//-----------------------------------------------------------------------------
// Keys
//-----------------------------------------------------------------------------

#define GPIO_KEYS_PATH  "/dev/input/by-path/platform-gpio-keys.0-event" 

#define DESC_LIMIT    64
#define DESC_VEC_LEN  4 

typedef struct
{
	int fid;
	const char* devicename;
} SKeys;

SKeys keys = {-1, 0};

bool KeysOpen(void)
{
	if(keys.fid >= 0)
	{
		return false;
	}

	keys.fid = open( GPIO_KEYS_PATH, O_RDONLY ); 

	if ( keys.fid < 0 )
	{
		return false;
	}

	return true;
}

void KeysClose(void)
{
	if(keys.fid >= 0)
	{
		close( keys.fid );
	}
}

#define _TEST_KEY( K, R )  (( keyb[ KEY_##K >> 3 ] & ( 1 << ( KEY_##K & 7 ))) ? EV3_KEY_##R : 0 ) 

uint8_t KeysRead(void)
{
	if(keys.fid < 0)
	{
		return 0;
	}

	uint8_t keyb[( KEY_MAX + 7 ) / 8 ]; 
	uint8_t status;

	ioctl( keys.fid, EVIOCGKEY( sizeof( keyb )), &keyb );
	status = _TEST_KEY( UP, UP )        | _TEST_KEY( DOWN, DOWN )
	     | _TEST_KEY( LEFT, LEFT )    | _TEST_KEY( RIGHT, RIGHT )
	     | _TEST_KEY( ENTER, CENTER ) | _TEST_KEY( BACKSPACE, BACK );

	return status;
}

