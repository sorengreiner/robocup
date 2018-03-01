#ifndef _CAR_H_
#define _CAR_H_

#include <stdio.h>
#include <stdbool.h>

#include "mindstorm.h"


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

int AngleToServoLeft(float angle);
int AngleToServoRight(float angle);

void CarInit(float fLength, float fWidth);

void CarComputeTurningAngle(SCar* pCar, float fAngle, float fSpeed);

void CarPrint(const SCar* pCar);

#endif

