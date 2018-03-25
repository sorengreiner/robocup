#include "conditions.h"
#include "line.h"
#include "car.h"

#include <stdio.h>
#include <math.h>


bool Approximate(SState* s, int noun0, float value0, int noun1, float value1)
{ 
	float left = value0;
	float right = value1;

	if(noun0 < NUM_VARS && noun0 != V_NIL)
	{
		left = GetVar(noun0);
	}

	if(noun1 < NUM_VARS && noun1 != V_NIL)
	{
		right = GetVar(noun1);
	}

	return fabs(left - right) < 0.5; 
}


bool ApproximateNotEqual(SState* s, int noun0, float value0, int noun1, float value1)
{ 
	float left = value0;
	float right = value1;

	if(noun0 < NUM_VARS && noun0 != V_NIL)
	{
		left = GetVar(noun0);
	}

	if(noun1 < NUM_VARS && noun1 != V_NIL)
	{
		right = GetVar(noun1);
	}

	return fabs(left - right) > 0.5; 
}


bool BranchLeft(SState* s, int noun0, float value0, int noun1, float value1)
{ 
    return true; 
}


bool BranchRight(SState* s, int noun0, float value0, int noun1, float value1)
{
    return true; 
}


bool Equal(SState* s, int noun0, float value0, int noun1, float value1)
{ 
	float left = value0;
	float right = value1;

	if(noun0 < NUM_VARS && noun0 != V_NIL)
	{
		left = GetVar(noun0);
	}

	if(noun1 < NUM_VARS && noun1 != V_NIL)
	{
		right = GetVar(noun1);
	}

	return left == right; 
}


bool Greater(SState* s, int noun0, float value0, int noun1, float value1)
{ 
	float left = value0;
	float right = value1;

	if(noun0 < NUM_VARS && noun0 != V_NIL)
	{
		left = GetVar(noun0);
	}

	if(noun1 < NUM_VARS && noun1 != V_NIL)
	{
		right = GetVar(noun1);
	}

	return left > right; 
}


bool GreaterEqual(SState* s, int noun0, float value0, int noun1, float value1)
{ 
	float left = value0;
	float right = value1;

	if(noun0 < NUM_VARS && noun0 != V_NIL)
	{
		left = GetVar(noun0);
	}

	if(noun1 < NUM_VARS && noun1 != V_NIL)
	{
		right = GetVar(noun1);
	}

	return left >= right; 
}


bool JunctionLeft(SState* s, int noun0, float value0, int noun1, float value1)
{
    SLineSensor* pLineSensor =  GetLineSensor();
    return LineSensorJunctionLeft(pLineSensor); 
}
    

bool JunctionRight(SState* s, int noun0, float value0, int noun1, float value1) 
{ 
    SLineSensor* pLineSensor =  GetLineSensor();
    return LineSensorJunctionRight(pLineSensor); 
}


bool Less(SState* s, int noun0, float value0, int noun1, float value1)
{ 
	float left = value0;
	float right = value1;

	if(noun0 < NUM_VARS && noun0 != V_NIL)
	{
		left = GetVar(noun0);
	}

	if(noun1 < NUM_VARS && noun1 != V_NIL)
	{
		right = GetVar(noun1);
	}

	return left < right; 
}


bool LessEqual(SState* s, int noun0, float value0, int noun1, float value1)
{ 
	float left = value0;
	float right = value1;

	if(noun0 < NUM_VARS && noun0 != V_NIL)
	{
		left = GetVar(noun0);
	}

	if(noun1 < NUM_VARS && noun1 != V_NIL)
	{
		right = GetVar(noun1);
	}

	return left <= right; 
}


bool Line(SState* s, int noun0, float value0, int noun1, float value1)
{
    SLineSensor* pLineSensor =  GetLineSensor();
    return LineSensorAnyLine(pLineSensor);
}


bool NoLine(SState* s, int noun0, float value0, int noun1, float value1)
{
    SLineSensor* pLineSensor =  GetLineSensor();
    return LineSensorNoLine(pLineSensor);
}


bool NotEqual(SState* s, int noun0, float value0, int noun1, float value1)
{ 
	float left = value0;
	float right = value1;

	if(noun0 < NUM_VARS && noun0 != V_NIL)
	{
		right = GetVar(noun0);
	}

	if(noun1 < NUM_VARS && noun1 != V_NIL)
	{
		right = GetVar(noun1);
	}

	return left != right; 
}


