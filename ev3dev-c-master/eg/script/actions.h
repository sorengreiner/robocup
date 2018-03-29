#ifndef _ACTIONS_H_
#define _ACTIONS_H_

#include "pid.h"

typedef struct
{
	float fLastLinePos;
	float fTarget;
	SPidr pidr;
} SFollowState;


typedef enum
{
	TARGETMODE_LEFTLINE = 0,
	TARGETMODE_RIGHTLINE,
	TARGETMODE_CENTER,
	TARGETMODE_HEADING,
} ETargetMode;


typedef struct
{
	float fDistance;	// Distance to complete when turning angle degrees
	float fHeading;		// Heading at entry
} STurnState;


typedef enum
{
    GYROSTATE_STEP1 = 0,
    GYROSTATE_STEP2,
    GYROSTATE_STEP3
} EGyroState;

typedef struct
{
    EGyroState eGyroState;
    float fTime;
} SGyroResetState;


// Action functions
bool Backward(SState* s, int noun0, float value0, int noun1, float value1);
bool Circle(SState* s, int noun0, float value0, int noun1, float value1);
bool Follow(SState* s, int noun0, float value0, int noun1, float value1);
bool FollowCourse(SState* s, int noun0, float value0, int noun1, float value1);
bool FollowLeft(SState* s, int noun0, float value0, int noun1, float value1);
bool FollowRight(SState* s, int noun0, float value0, int noun1, float value1);
bool Forward(SState* s, int noun0, float value0, int noun1, float value1);
bool GyroReset(SState* s, int noun0, float value0, int noun1, float value1);
bool Set(SState* s, int noun0, float value0, int noun1, float value1);
bool Stop(SState* s, int noun0, float value0, int noun1, float value1);
bool Tool(SState* s, int noun0, float value0, int noun1, float value1);  
bool TurnLeft(SState* s, int noun0, float value0, int noun1, float value1);
bool TurnRight(SState* s, int noun0, float value0, int noun1, float value1);
bool Wait(SState* s, int noun0, float value0, int noun1, float value1);


#endif
