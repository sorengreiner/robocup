#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "pidr.h"



int main(int argc, char* argv[])
{
	SPidr pid;
	pid.dt = 0.05;
	pid.max = 40;
	pid.min = 40;
	pid.Kp = 2;
	pid.Ki = 0;
	pid.Kd = 0;
	pid.error = 0;
	pid.integral = 0;

	float fSteer = 0;
	float fTarget = 90;
	float fPos = 0;
	pid.dt = 0.05;
	float fRadius = 0.5; // m
	float fSpeed = 0.5; // m/s
	fSteer = PidCompute(&pid, 90, 0);
	printf("s:%f target:%f pos:%f\n", fSteer, fTarget, fPos);
	fSteer = PidCompute(&pid, 90, 1);
	printf("s:%f target:%f pos:%f\n", fSteer, fTarget, fPos);
   

    return 0;
}
