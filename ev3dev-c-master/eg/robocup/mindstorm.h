#ifndef _MINDSTORM_H_
#define _MINDSTORM_H_

#include <stdbool.h>
#include <stdint.h>

#define M_PI		3.14159265358979323846 

typedef enum
{
	OUTPUTPORT_1 = 1,
	OUTPUTPORT_2 = 2,
	OUTPUTPORT_3 = 3,
	OUTPUTPORT_4 = 4
} EOutputPort;


typedef enum
{
	INPUTPORT_1 = 1,
	INPUTPORT_2 = 2,
	INPUTPORT_3 = 3,
	INPUTPORT_4 = 4
} EInputPort;


bool SensorInit(void);

//-----------------------------------------------------------------------------
// Motor control
//-----------------------------------------------------------------------------

typedef struct
{
	int fid;
	const char* devicename;
	EOutputPort ePort;
} SMotor;

bool MotorOpen(SMotor* pMotor, EOutputPort outputport);
void MotorClose(SMotor* pMotor);
bool MotorSetSpeed(SMotor* pMotor, float fSpeed);


//-----------------------------------------------------------------------------
// Servo control
//-----------------------------------------------------------------------------

typedef enum
{
	SERVO_0 = 0,
	SERVO_1 = 1,
	SERVO_2 = 2,
	SERVO_3 = 3,
	SERVO_4 = 4,
	SERVO_5 = 5,
	SERVO_6 = 6,
	SERVO_7 = 7,
	SERVO_8 = 8,
} EServo;

typedef struct
{
	int fid;
	const char* devicename;
	EInputPort ePort;
	EServo eServo;
} SServo;

bool ServoOpen(SServo* pServo, EInputPort eInputPort, EServo eServo);
void ServoClose(SServo* pServo);
bool ServoSetPosition(SServo* pServo);

//-----------------------------------------------------------------------------
// Keys
//-----------------------------------------------------------------------------

enum
{
	EV3_KEY__NONE_ = 0,

	EV3_KEY_UP     = 0x01L,
	EV3_KEY_DOWN   = 0x02L,
	EV3_KEY_LEFT   = 0x04L,
	EV3_KEY_RIGHT  = 0x08L,
	EV3_KEY_CENTER = 0x10L,
	EV3_KEY_BACK   = 0x20L,
};

bool KeysOpen();
void KeysClose();
uint8_t KeysRead();

//-----------------------------------------------------------------------------
// IR Remote
//-----------------------------------------------------------------------------

enum {
	IR_REMOTE__NONE_   = 0,

	RED_UP             = 1,
	RED_DOWN           = 2,
	BLUE_UP            = 3,
	BLUE_DOWN          = 4,
	RED_UP_BLUE_UP     = 5,
	RED_UP_BLUE_DOWN   = 6,
	RED_DOWN_BLUE_UP   = 7,
	RED_DOWN_BLUE_DOWN = 8,
	BEACON_MODE_ON     = 9,
	RED_UP_RED_DOWN    = 10,
	BLUE_UP_BLUE_DOWN  = 11,
};

typedef struct
{
	int fid;
	const char* devicename;
	EInputPort ePort;
	EServo eServo;
} SRemote;


bool RemoteOpen(SRemote* pRemote, EInputPort eInputPort);
void RemoteClose(SRemote* pRemote);
uint8_t RemoteRead(SRemote* pRemote);

//-----------------------------------------------------------------------------
// Time
//-----------------------------------------------------------------------------
long TimeMilliseconds(void);

extern void sleep_ms( int ms );

#endif

