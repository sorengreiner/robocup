#ifndef _PID_H_
#define _PID_H_

#include <stdint.h>
#include <stdbool.h>


typedef struct
{
	float dt;
	float max;
	float min;
	float Kp;
	float Ki;
	float Kd;
	float error;
	float integral;
} SPidr;



float PidCompute( SPidr* pPid, float target, float pos);

#endif
