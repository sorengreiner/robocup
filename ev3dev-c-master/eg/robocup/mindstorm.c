#include "mindstorm.h"

#include "sensors.h"
#include <stdbool.h>
#include <stdint.h>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <string.h>
#include <stdio.h>

typedef struct
{
	int portno;
    const char* filename;
} SSensorDetect;


const char* g_SensorFilenames[] =
{
    "/sys/class/lego-sensor/sensor0",
    "/sys/class/lego-sensor/sensor1",
    "/sys/class/lego-sensor/sensor2",
    "/sys/class/lego-sensor/sensor3",
};


SSensorDetect g_SensorDetect[NUM_INPUTPORTS] =
{
    {1, 0},
    {2, 0},
    {3, 0},
    {4, 0},
};

// Identify the attached sensors per in port
bool SensorScan(void)
{
    char name[128];
    for(int i = 0; i < NUM_INPUTPORTS; i++)
    {
        // test address of file
        memset(name, 0, sizeof(name));
        snprintf(name, sizeof(name) - 1 , "%s/address", g_SensorFilenames[i]);
        int fid = open(name, O_RDONLY);
        if(fid >= 0)
        {
            char buffer[128];
            memset(buffer, 0, sizeof(buffer));
            int n = read(fid, buffer, sizeof(buffer)-1);
            if(n > 0)
            {
//                printf("input: %s", buffer);
                int port = 0;
                if(sscanf(buffer, "in%d:", &port) == 1)
                {
                    if(port >= 1 && port <= 4)
                    {
                        g_SensorDetect[port - 1].filename = g_SensorFilenames[i];
//                        printf("Sensor %d at filename %s\n", port, g_SensorFilenames[i]);
                    }
                }
            }
 
            close(fid);
        }
    }

	// summary
    for(int i = 0; i < 4; i++)
    {
        int port = i + 1;
        if(g_SensorDetect[i].filename)
        {
            printf("in%d '%s'\n", port, g_SensorDetect[i].filename);
        }

    }	 
}


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

#define PATH_MODE  "/sys/class/lego-sensor/sensor///mode"


bool RemoteOpen(SRemote* pRemote, EInputPort eInputPort)
{
   /* 
    char command[] = "IR_REMOTE";
    FILE* f = open("/sys/class/lego_sensor/sensor0/mode", O_WRONLY
    
sensor_set_mode( pool, LEGO_EV3_IR_IR_REMOTE );
	char s[] = PATH_MODE;
	*modp_uitoa10( sn, s + PATH_PREF_LEN ) = '/';

	return ev3_write_char_array( s, value ); 
size_t ev3_write_char_array( const char *fn, char *value )
{
	return ev3_write( fn, value );
} 
size_t ev3_write( const char *fn, char *value )
{
	return ev3_write_binary( fn, value, strlen( value ));
} 
	FILE *f;
	size_t result;

	f = fopen( fn, "w" );
	if ( f == NULL ) return ( 0 );

	result = fwrite( data, 1, sz, f );
	fclose( f );
	return ( result ); 
*/
	return true;
}

void RemoteClose(SRemote* pRemote)
{
}

uint8_t RemoteRead(SRemote* pRemote)
{
	return 0;
}


//-----------------------------------------------------------------------------
// Time
//-----------------------------------------------------------------------------


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


