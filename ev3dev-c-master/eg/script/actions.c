#include "magnus.h"
#include "actions.h"
#include "car.h"

#include <stdio.h>

bool FollowTarget(SState* s, ETargetMode eTargetMode) 
{ 
	SFollowState* p = (SFollowState*)s->stack;
	if(s->index == 0)
	{
		p->fLastLinePos = 0.0f;
		p->pidr.max = 50;
		p->pidr.min = -50;
		p->pidr.Kp = GetVar(V_KP);
		p->pidr.Ki = GetVar(V_KI);
		p->pidr.Kd = GetVar(V_KD);
		p->pidr.error = 0;
		p->pidr.integral = 0;
	}

    SLine* pLineSensor =  GetLineSensor();
    
	// Get current target
	float fTarget = 0.0; // Center of line sensor is the target
	float pos = 0.0;
	switch(eTargetMode)
	{
	case TARGETMODE_LEFTLINE: 
		if(pLineSensor->nLeftEdges > 0)
		{
			p->fLastLinePos = LinePosToPhysical(pLineSensor->leftEdge);
		}
		pos = p->fLastLinePos;
		fTarget = 0.0;
		break;
	case TARGETMODE_RIGHTLINE: 
		if(pLineSensor->nRightEdges > 0)
		{
			p->fLastLinePos = LinePosToPhysical(pLineSensor->rightEdge);
		}
		pos = p->fLastLinePos;
		fTarget = 0.0;
		break;
	case TARGETMODE_CENTER: 
		if(pLineSensor->n > 0)
		{
			p->fLastLinePos = LinePosToPhysical(pLineSensor->p0);
		}
		pos = p->fLastLinePos;
		fTarget = 0.0;
		break;
	case TARGETMODE_HEADING: 
		break;
	default: break;
	}
	
	p->pidr.dt = s->dt;
	float fAngle = PidCompute(&p->pidr, fTarget, pos);
	float fSpeed = GetVar(V_SPEED);
	UpdateCar(fSpeed, fAngle);

	return false;
}


//-----------------------------------------------------------------------------
/// Public Action functions
//-----------------------------------------------------------------------------

bool Backward(SState* s, int noun0, float value0, int noun1, float value1) 
{ 
	if(s->index == 0)
	{
		float fSpeed = GetVar(V_SPEED);
		float fSteer = GetVar(V_STEER);
		UpdateCar(-fSpeed, fSteer);
	}

	return false;
}


bool Follow(SState* s, int noun0, float value0, int noun1, float value1) 
{ 
	return FollowTarget(s, TARGETMODE_CENTER);
}


bool FollowLeft(SState* s, int noun0, float value0, int noun1, float value1) 
{ 
	return FollowTarget(s, TARGETMODE_LEFTLINE);
}


bool FollowRight(SState* s, int noun0, float value0, int noun1, float value1) 
{ 
	return FollowTarget(s, TARGETMODE_RIGHTLINE);
}


bool Forward(SState* s, int noun0, float value0, int noun1, float value1) 
{ 
	if(s->index == 0)
	{
		float fSpeed = GetVar(V_SPEED);
		float fSteer = GetVar(V_STEER);
		UpdateCar(fSpeed, fSteer);
	}

	return false;
}


bool Set(SState* s, int noun0, float value0, int noun1, float value1) 
{ 
	if(s->index == 0)
	{
	}
	return true;
}


bool Stop(SState* s, int noun0, float value0, int noun1, float value1) 
{ 
	if(s->index == 0)
	{
		UpdateCar(0, 0);
	}
	return true;
}


bool Straight(SState* s, int noun0, float value0, int noun1, float value1) 
{ 
	SFollowState* p = (SFollowState*)s->stack;
	float heading = GetVar(V_HEADING);
	float course = GetVar(V_COURSE);
	if(s->index == 0)
	{
		p->pidr.max = 35;
		p->pidr.min = -35;
		p->pidr.Kp = GetVar(V_KP);
		p->pidr.Ki = GetVar(V_KI);
		p->pidr.Kd = GetVar(V_KD);
		p->pidr.error = 0;
		p->pidr.integral = 0;
		printf("heading:%g course:%g\n", heading, course);
	}
	
	p->pidr.dt = s->dt;
	float fAngle = PidCompute(&p->pidr, course, heading);
	float fSpeed = GetVar(V_SPEED);
	UpdateCar(fSpeed, -fAngle);
	return false;
}


bool Tool(SState* s, int noun0, float value0, int noun1, float value1)
{
	if(s->index == 0)
	{
		float fToolPos = GetVar(V_TOOLPOS);
		UpdateTool(100, fToolPos);
	}
	return true;
}


bool TurnLeft(SState* s, int noun0, float value0, int noun1, float value1)
{ 
	STurnState* p = (STurnState*)s->stack;
	float fAngle = GetVar(V_ANGLE);

	if(s->index == 0)
	{
		// Ignore V_STEER while we are turning, but use V_SPEED
		p->fHeading = GetVar(V_HEADING);
		float fSpeed = GetVar(V_SPEED);
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
	}

	float fHeading = GetVar(V_HEADING);
	float diff = fAngle - fHeading;
	float fSteer = -diff*2;
	if(fSteer > 0)
	{
		if(fSteer > 30)
		{
			fSteer = 30;
		}
		else if(fSteer < 5)
		{
			fSteer = 5;
		}
	}
	else
	{
		if(fSteer < - 30)
		{
			fSteer = -30;	
		}
		else if(fSteer > -5)
		{
			fSteer = -5;
		}
	}

	UpdateCar(GetVar(V_SPEED), fSteer);

	if(fHeading <= fAngle)
	{
		UpdateCar(GetVar(V_SPEED), 0);
		return true;
	}

	return false; 
}


bool TurnRight(SState* s, int noun0, float value0, int noun1, float value1)
{
	STurnState* p = (STurnState*)s->stack;
	float fAngle = GetVar(V_ANGLE);

	if(s->index == 0)
	{
		// Ignore V_STEER while we are turning, but use V_SPEED
		p->fHeading = GetVar(V_HEADING);
		float fSpeed = GetVar(V_SPEED);
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
	}

	float fHeading = GetVar(V_HEADING);
	float diff = fAngle - fHeading;
	float fSteer = -diff*2;

	if(fSteer > 0)
	{
		if(fSteer > 30)
		{
			fSteer = 30;
		}
		else if(fSteer < 5)
		{
			fSteer = 5;
		}
	}
	else
	{
		if(fSteer < - 30)
		{
			fSteer = -30;	
		}
		else if(fSteer > -5)
		{
			fSteer = -5;
		}
	}

	UpdateCar(GetVar(V_SPEED), fSteer);

	if(fHeading >= fAngle)
	{
		UpdateCar(GetVar(V_SPEED), 0);
		return true;
	}

	return false; 
}


bool Wait(SState* s, int noun0, float value0, int noun1, float value1) 
{ 
	if(s->index == 0)
	{
	}
	return false;
}


