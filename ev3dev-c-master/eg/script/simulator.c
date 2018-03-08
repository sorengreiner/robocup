#include "magnus.h"
#include "simulator.h"

#include <stdio.h>

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

