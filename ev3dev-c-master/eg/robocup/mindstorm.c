#include "mindstorm.h"


#include <stdbool.h>
#include <stdint.h>


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

bool KeysOpen()
{
	return true;
}

void KeysClose()
{
}

uint8_t KeysRead()
{
	return 0;
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


