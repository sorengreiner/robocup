#include "pid.h"

#include <stdlib.h>
//#include <math.h>

//#define M_PI		3.14159265358979323846 



float PidCompute( SPidr* pPid, float target, float pos)
{
    // Calculate error
    float error = target - pos;

    // Proportional term
    float Pout = pPid->Kp * error;

    // Integral term
    pPid->integral += error * pPid->dt;
    float Iout = pPid->Ki * pPid->integral;

    // Derivative term
    float derivative = (error - pPid->error) / pPid->dt;
    float Dout = pPid->Kd * derivative;

    // Calculate total output
    float output = Pout + Iout + Dout;

    // Restrict to max/min
    if( output > pPid->max )
	{
        output = pPid->max;
	}
    else if( output < pPid->min )
	{
        output = pPid->min;
	}

    // Save error to previous error
    pPid->error = error;

	return output;	
}


