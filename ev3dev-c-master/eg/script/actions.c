#include "magnus.h"
#include "actions.h"
#include "car.h"

#include <stdio.h>
#include <math.h>



bool FollowTarget(SState* s, ETargetMode eTargetMode) 
{ 
	SFollowState* p = (SFollowState*)s->stack;
	if(s->index == 0)
	{
		p->fLastLinePos = 0.0f;
		p->pidr.max = GetVar(V_KLIMIT);
		p->pidr.min = -GetVar(V_KLIMIT);
		p->pidr.Kp = GetVar(V_KP);
		p->pidr.Ki = GetVar(V_KI);
		p->pidr.Kd = GetVar(V_KD);
		p->pidr.error = 0;
		p->pidr.integral = 0;
	}

    SLineSensor* pLineSensor =  GetLineSensor();
    
	// Get current target
	float fTarget = 0.0; // Center of line sensor is the target
	float pos = 0.0;
    float val;
	switch(eTargetMode)
	{
	case TARGETMODE_LEFTLINE:
		if(LineSensorLeft(pLineSensor, &val))
		{
			p->fLastLinePos = LinePosToPhysical(val);
		}
		pos = p->fLastLinePos;
		fTarget = 0.0;
		break;
	case TARGETMODE_RIGHTLINE: 
		if(LineSensorRight(pLineSensor, &val))
		{
			p->fLastLinePos = LinePosToPhysical(val);
		}
		pos = p->fLastLinePos;
		fTarget = 0.0;
		break;
	case TARGETMODE_CENTER: 
		if(LineSensorCenter(pLineSensor, &val))
		{
			p->fLastLinePos = LinePosToPhysical(val);
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


bool Straight(SState* s, float course, float speed) 
{ 
    SFollowState* p = (SFollowState*)s->stack;
    if(s->index == 0)
    {
        // The minimum turning radius determines the limits on the wheel angle in the PID regulator
        float radius = GetVar(V_RADIUS);
        float fCarLength = GetCar()->fCarLength/1000.0;  // Car length in m
        float fAngleLimit = 180.0*atan(fCarLength/fabs(radius))/M_PI;
        p->pidr.max = fAngleLimit;
        p->pidr.min = -fAngleLimit;
        p->pidr.Kp = GetVar(V_KPS);
        p->pidr.Ki = GetVar(V_KIS);
        p->pidr.Kd = GetVar(V_KDS);
        p->pidr.error = 0;
        p->pidr.integral = 0;
    }
	
    p->pidr.dt = s->dt;
    float heading = GetVar(V_HEADING);
    float fAngle = PidCompute(&p->pidr, course, heading);
    int reverse = 1;
    if(speed < 0)
    {
        reverse = -1;
    }
    UpdateCar(speed, -fAngle*reverse);
    return false;
}


//-----------------------------------------------------------------------------
/// Public Action functions
//-----------------------------------------------------------------------------

bool Backward(SState* s, int noun0, float value0, int noun1, float value1) 
{ 
    if(s->index == 0)
    {
        SetVar(V_COURSE, GetVar(V_HEADING));
    }
    float course = GetVar(V_COURSE);
    float speed = GetVar(V_SPEED);
    return Straight(s, course, -speed);
}


bool Circle(SState* s, int noun0, float value0, int noun1, float value1) 
{ 
	if(s->index == 0)
	{
		float fSpeed = GetVar(V_SPEED);
		float fSteer = GetVar(V_STEER);
		UpdateCar(fSpeed, fSteer);
	}

	return false;
}


bool Follow(SState* s, int noun0, float value0, int noun1, float value1) 
{ 
	return FollowTarget(s, TARGETMODE_CENTER);
}


bool FollowCourse(SState* s, int noun0, float value0, int noun1, float value1) 
{ 
    float course = GetVar(V_COURSE);
    float speed = GetVar(V_SPEED);
    return Straight(s, course, speed);
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
        SetVar(V_COURSE, GetVar(V_HEADING));
    }
    float course = GetVar(V_COURSE);
    float speed = GetVar(V_SPEED);
    return Straight(s, course, speed);
}


bool GyroReset(SState* s, int noun0, float value0, int noun1, float value1)
{
    SGyroResetState* p = (SGyroResetState*)s->stack;
    if(s->index == 0)
    {
        p->eGyroState = GYROSTATE_STEP1;
        p->fTime = GetVar(V_TIME);
    }
    
    float fTime = GetVar(V_TIME);
    switch(p->eGyroState)
    {
        case GYROSTATE_STEP1:
            // Wait until car is still before starting calibration
            if(fTime > p->fTime + 1.0)
            {
                GyroModeCal();
                p->eGyroState = GYROSTATE_STEP2;
                p->fTime = GetVar(V_TIME);
            }
        break;
        case GYROSTATE_STEP2:
            
            if(fTime > p->fTime + 4.0)
            {
                GyroModeAngle();
                p->eGyroState = GYROSTATE_STEP3;
                p->fTime = GetVar(V_TIME);
            }
        break;
        case GYROSTATE_STEP3:
            if(fTime > p->fTime + 1.0)
            {
                return true;
            }
        break;
    }
    
    return false;
}


bool Set(SState* s, int noun0, float value0, int noun1, float value1) 
{ 
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


bool Tool(SState* s, int noun0, float value0, int noun1, float value1)
{
    if(s->index == 0)
    {
        float fToolPos = GetVar(V_TOOLPOS);
        float fToolSpeed = GetVar(V_TOOLSPEED);
        UpdateTool(fToolSpeed, fToolPos);
    }
    return true;
}


bool TurnLeft(SState* s, int noun0, float value0, int noun1, float value1)
{
    float speed = GetVar(V_SPEED);
    if(s->index == 0)
    {
        if(speed > 0)
        {
            SetVar(V_COURSE, GetVar(V_HEADING) - GetVar(V_ANGLE));
        }
        else
        {
            SetVar(V_COURSE, GetVar(V_HEADING) + GetVar(V_ANGLE));
        }
    }
    
    float course = GetVar(V_COURSE);
    return Straight(s, course, speed);
}


bool TurnRight(SState* s, int noun0, float value0, int noun1, float value1)
{
    float speed = GetVar(V_SPEED);
    if(s->index == 0)
    {
        if(speed > 0)
        {
            SetVar(V_COURSE, GetVar(V_HEADING) + GetVar(V_ANGLE));
        }
        else
        {
            SetVar(V_COURSE, GetVar(V_HEADING) - GetVar(V_ANGLE));
        }
    }
    
    float course = GetVar(V_COURSE);
    return Straight(s, course, speed);
}


bool Wait(SState* s, int noun0, float value0, int noun1, float value1) 
{ 
	return false;
}


