#include "magnus.h"
#include "simulator.h"
#include "line.h"

#include <stdio.h>
#include <math.h>


typedef struct 
{
	float fCarLength;
	float fCarWidth;

	float fAngle;
	float fSpeed;

	float fFrontWheelLeftAngle;
	float fFrontWheelRightAngle;
	float fBackWheelLeftSpeed;
	float fBackWheelRightSpeed;
} SCar;



SLine lineSensor;
SCar car;


void UpdateCar(float speed, float angle)
{
	printf("UpdateCar %f %f", speed, angle);
}


bool Line(SState* s, int noun0, float value0, int noun1, float value1)
{
	return (lineSensor.nLeftEdges > 0) || (lineSensor.nRightEdges > 0);
}


#define LINESENSOR_WIDTH_MM (46.0)

float LinePosToPhysical(float pos)
{
	return -(LINESENSOR_WIDTH_MM*pos/POINTS - LINESENSOR_WIDTH_MM/2);
}


typedef struct
{
	float fLastLinePos;
} SFollowState;


bool Follow(SState* s, int noun0, float value0, int noun1, float value1) 
{ 
	if(s->index == 0)
	{
		printf("Follow\n");
		if(noun0 < NUM_VARS)
		{
			SetVar(noun0, value0);
		}

		if(noun1 < NUM_VARS)
		{
			SetVar(noun1, value1);
		}
		s->index++;
	}

	return false;
}


bool FollowLeft(SState* s, int noun0, float value0, int noun1, float value1) 
{ 
	SFollowState* p = (SFollowState*)s->stack;
	if(s->index == 0)
	{
		printf("FollowLeft\n");
		p->fLastLinePos = 0.0f;
	}

	float fSpeed = GetVar(V_SPEED);

	if(lineSensor.nLeftEdges > 0)
	{
		p->fLastLinePos = LinePosToPhysical(lineSensor.leftEdge);
	}

	float fAngle = p->fLastLinePos;
	UpdateCar(fSpeed, fAngle);

	return false;
}


bool FollowRight(SState* s, int noun0, float value0, int noun1, float value1) 
{ 
	SFollowState* p = (SFollowState*)s->stack;
	if(s->index == 0)
	{
		printf("FollowRight\n");
		p->fLastLinePos = 0.0f;
	}

	float fSpeed = GetVar(V_SPEED);

	if(lineSensor.nRightEdges > 0)
	{
		p->fLastLinePos = LinePosToPhysical(lineSensor.rightEdge);
	}

	float fAngle = p->fLastLinePos;
	UpdateCar(fSpeed, fAngle);

	return false;
}


bool Forward(SState* s, int noun0, float value0, int noun1, float value1) 
{ 
	if(s->index == 0)
	{
		printf("Forward\n");
		if(noun0 < NUM_VARS)
		{
			SetVar(noun0, value0);
		}

		if(noun1 < NUM_VARS)
		{
			SetVar(noun1, value1);
		}
		s->index++;
	}

	return false;
}



bool Backward(SState* s, int noun0, float value0, int noun1, float value1) 
{ 
	if(s->index == 0)
	{
		printf("Backward\n");
		if(noun0 < NUM_VARS)
		{
			SetVar(noun0, value0);
		}

		if(noun1 < NUM_VARS)
		{
			SetVar(noun1, value1);
		}
		s->index++;
	}

	return false;
}


void UpdateVars(float delta)
{
	float angle = GetVar(V_ANGLE);
	float time = GetVar(V_TIME);
	float speedcmps = GetVar(V_SPEED);
	float heading = GetVar(V_HEADING);
	float odometer = GetVar(V_ODOMETER);
	float x = GetVar(V_XPOS);
	float y = GetVar(V_YPOS);

	// Update time variable
	time += delta;

	float speed = speedcmps/100;

	// Update heading and pos as approximated predicted values
	heading += delta*angle;
	heading = fmodf(heading, 360.0);
	odometer += delta*speed;
	x += delta*speed*cosf(3.14159*heading/180.0);
	y += delta*speed*sinf(3.14159*heading/180.0);

	SetVar(V_ANGLE, angle);
	SetVar(V_TIME, time);
	SetVar(V_SPEED, speedcmps);
	SetVar(V_HEADING, heading);
	SetVar(V_ODOMETER, odometer);
	SetVar(V_XPOS, x);
	SetVar(V_YPOS, y);

	printf("time=%f heading=%f odometer=%f x=%f y=%f speed=%f angle=%f\n", time, heading, odometer, x, y, speed, angle);
}

void AssignVar(int noun, float value)
{
	SetVar(noun, value);
}


typedef struct
{
	float fOdometer;	// Odometer value at entry
	float fDistance;	// Distance to complete when turning angle degrees
} STurnState;


bool TurnLeft(SState* s, int noun0, float value0, int noun1, float value1)
{ 
	STurnState* p = (STurnState*)s->stack;

	if(s->index == 0)
	{
		printf("TurnLeft\n");
		// Ignore V_ANGLE while we are turning, but use V_SPEED
		p->fOdometer = GetVar(V_ODOMETER);
		float fSpeed = GetVar(V_SPEED);
		float fAngle = GetVar(V_ANGLE);
		float fRadius = GetVar(V_RADIUS); // Turn radius in meter
		// Limit turn radius
		if(fRadius < 0.2)
		{
			fRadius = 0.2;
		}

		if(fRadius > 100.0)
		{
			fRadius = 100.0;
		}

		p->fDistance = fRadius*fAngle*M_PI/180.0;

		float fWheelAngle = 180.0*atan2(car.fCarLength/1000, fRadius)/M_PI;
		printf("angle=%f wangle=%f radius=%f\n", fAngle, fWheelAngle, fRadius);
		UpdateCar(fSpeed, fWheelAngle);
	}

	float fOdometer = GetVar(V_ODOMETER);
//	printf("%f %f %f\n", fOdometer, p->fOdometer, p->fDistance);
	if(fOdometer - p->fOdometer > p->fDistance)
	{
		SetVar(V_ANGLE, 0);
		return true;
	}

	return false; 
}

bool TurnRight(SState* s, int noun0, float value0, int noun1, float value1)
{
	STurnState* p = (STurnState*)s->stack;

	if(s->index == 0)
	{
		printf("TurnRight\n");
		// Ignore V_ANGLE while we are turning, but use V_SPEED
		p->fOdometer = GetVar(V_ODOMETER);
		float fSpeed = GetVar(V_SPEED);
		float fAngle = GetVar(V_ANGLE);
		float fRadius = GetVar(V_RADIUS); // Turn radius in meter
		// Limit turn radius
		if(fRadius < 0.2)
		{
			fRadius = 0.2;
		}

		if(fRadius > 100.0)
		{
			fRadius = 100.0;
		}

		p->fDistance = fRadius*fAngle*M_PI/180.0;

		float fWheelAngle = 180.0*atan2(car.fCarLength/1000, fRadius)/M_PI;
		UpdateCar(fSpeed, -fWheelAngle);
	}

	float fOdometer = GetVar(V_ODOMETER);
//	printf("%f %f %f\n", fOdometer, p->fOdometer, p->fDistance);
	if(fOdometer - p->fOdometer > p->fDistance)
	{
		SetVar(V_ANGLE, 0);
		return true;
	}

	return false; 
}


bool Stop(SState* s, int noun0, float value0, int noun1, float value1) 
{ 
	if(s->index == 0)
	{
		printf("Stop\n");
		SetVar(V_SPEED, 0);
		SetVar(V_ANGLE, 0);
		UpdateCar(0, 0);
	}

	return false;
}


