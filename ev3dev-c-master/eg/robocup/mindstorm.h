#ifndef _MINDSTORM_H_
#define _MINDSTORM_H_

#include <stdbool.h>
#include <stdint.h>

#define M_PI		3.14159265358979323846 

typedef enum
{
	OUTPUTPORT_0 = 0,
	OUTPUTPORT_1 = 1,
	OUTPUTPORT_2 = 2,
	OUTPUTPORT_3 = 3
} EOutputPort;


typedef enum
{
	INPUTPORT_0 = 0,
	INPUTPORT_1 = 1,
	INPUTPORT_2 = 2,
	INPUTPORT_3 = 3
} EInputPort;


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

enum {
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

