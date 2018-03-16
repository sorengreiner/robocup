#ifndef _ROBOCUP_H_
#define _ROBOCUP_H_

#include <stdbool.h>
#include <stdint.h>


typedef struct 
{
	float fCarLength;
	float fCarWidth;
	float fBackWidth;

	float fAngle;
	float fSpeed;

	float fFrontWheelLeftAngle;
	float fFrontWheelRightAngle;
	float fBackWheelLeftSpeed;
	float fBackWheelRightSpeed;
} SCar;


#endif
