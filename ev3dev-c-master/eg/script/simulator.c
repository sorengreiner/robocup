#include "magnus.h"
#include "simulator.h"

#include <stdio.h>
#include <math.h>


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
	float speed = GetVar(V_SPEED);
	float heading = GetVar(V_HEADING);
	float odometer = GetVar(V_ODOMETER);
	float x = GetVar(V_XPOS);
	float y = GetVar(V_YPOS);

	// Update time variable
	time += delta;

	// Update heading and pos as approximated predicted values
	heading += delta*angle;
	heading = fmodf(heading, 360.0);
	odometer += delta*speed;
	x += delta*speed*cosf(3.14159*heading/180.0);
	y += delta*speed*sinf(3.14159*heading/180.0);

	SetVar(V_ANGLE, angle);
	SetVar(V_TIME, time);
	SetVar(V_SPEED, speed);
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



