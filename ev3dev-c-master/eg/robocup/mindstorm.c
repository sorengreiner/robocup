#include "mindstorm.h"


#include <stdbool.h>
#include <stdint.h>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>

//-----------------------------------------------------------------------------
// Motor control
//-----------------------------------------------------------------------------

bool MotorOpen(SMotor* pMotor, EOutputPort outputport)
{
	return true;
}

void MotorClose(SMotor* pMotor)
{
}

bool MotorSetSpeed(SMotor* pMotor, float fSpeed)
{
	return true;
}


//-----------------------------------------------------------------------------
// Servo control
//-----------------------------------------------------------------------------

bool ServoOpen(SServo* pServo, EInputPort eInputPort, EServo eServo)
{
	return true;
}

void ServoClose(SServo* pServo)
{
}

bool ServoSetPosition(SServo* pServo)
{
	return true;
}


//-----------------------------------------------------------------------------
// Keys
//-----------------------------------------------------------------------------

#define GPIO_KEYS_PATH  "/dev/input/by-path/platform-gpio-keys.0-event" 

/*
enum {
	EV3_KEY__NONE_ = 0,

	EV3_KEY_UP     = 0x01L,
	EV3_KEY_DOWN   = 0x02L,
	EV3_KEY_LEFT   = 0x04L,
	EV3_KEY_RIGHT  = 0x08L,
	EV3_KEY_CENTER = 0x10L,
	EV3_KEY_BACK   = 0x20L,
};
*/

#define DESC_LIMIT    64
#define DESC_VEC_LEN  4 

typedef struct
{
	int fid;
	const char* devicename;
} SKeys;

SKeys keys = {-1, 0};

bool KeysOpen()
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

void KeysClose()
{
	if(keys.fid >= 0)
	{
		close( keys.fid );
	}
}

#define _TEST_KEY( K, R )  (( keyb[ KEY_##K >> 3 ] & ( 1 << ( KEY_##K & 7 ))) ? EV3_KEY_##R : 0 ) 

uint8_t KeysRead()
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


//-----------------------------------------------------------------------------
// IR Remote
//-----------------------------------------------------------------------------

bool RemoteOpen(SRemote* pRemote, EInputPort eInputPort)
{
	return true;
}

void RemoteClose(SRemote* pRemote)
{
}

uint8_t RemoteRead(SRemote* pRemote)
{
	return 0;
}


#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h>   // for nanosleep
#else
#include <unistd.h> // for usleep
#endif

void sleep_ms(int milliseconds) // cross-platform sleep function
{
#ifdef WIN32
    Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
#else
    usleep(milliseconds * 1000);
#endif
}


