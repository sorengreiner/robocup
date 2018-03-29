#ifndef _CAR_H_
#define _CAR_H_


#include <stdbool.h>
#include <stdint.h>

#include "line.h"


#define LINESENSOR_WIDTH_MM (46.0)
#define BACKWHEEL_DIAMETER_MM ( 81.6 )
#define TACHO_RESOLUTION ( 360 )


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


SCar* GetCar(void);
void UpdateCar(float speed, float angle);
void UpdateTool(float speed, float position);
void UpdateVars(float delta);
void AssignVar(int noun, float value);
void GyroModeCal(void);
void GyroModeRate(void);
void GyroModeAngle(void);


float LinePosToPhysical(float pos);

SLineSensor* GetLineSensor(void);

#endif
