#include "robocup.h"
#include "magnus.h"
#include "brick.h"
#include "ev3_servo.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>


#define MOTOR_LEFT     OUTC
#define MOTOR_RIGHT    OUTB
#define MOTOR_BOTH     ( MOTOR_LEFT | MOTOR_RIGHT )
#define IR_CHANNEL     0 

int max_speed;         /* Motor maximal speed (will be detected) */ 
POOL_T ir;             /* IR sensor port (will be detected) */
POOL_T rc;
uint8_t snRight;
uint8_t snLeft;

//float speed = 0.0f;
//float angle = 0.0f;
int leftCenter = -10;
int rightCenter = 3;
SCar car;


int AngleToServoLeft(float angle)
{
	int servo = (int)(-angle*100.0f/90.0f) + leftCenter;
	return servo;
}


int AngleToServoRight(float angle)
{
	int servo = (int)(-angle*100.0f/90.0f) + rightCenter;
	return servo;
}


void CarComputeTurningAngle(SCar* pCar, float fAngle, float fSpeed)
{
	pCar->fAngle = fAngle;
	pCar->fSpeed = fSpeed;

	float v = (M_PI / 180.0f) * fAngle;
	float tanv = tan(v);

	float vr = atan(1.0f / (1.0f/tanv + (pCar->fCarWidth/2)/pCar->fCarLength) );
	float vl = atan(1.0f / (1.0f/tanv - (pCar->fCarWidth/2)/pCar->fCarLength) );

	pCar->fFrontWheelRightAngle = vr * 180.0f / M_PI;
	pCar->fFrontWheelLeftAngle = vl * 180.0f / M_PI;

	float curveCenter = pCar->fCarLength;
	float curveLeft = pCar->fCarLength;
	float curveRight = pCar->fCarLength;

	// Largest turning radius accepted. Larger than this and the car is considered going straight
	const float radiusMax = 10000.0f; // mm

	float angularLimit = atan(pCar->fCarLength/radiusMax);

	if(fabs(v) > angularLimit)
	{
		float radius = pCar->fCarLength/tanv;
		curveCenter = v*radius;
		curveLeft = v*(radius - pCar->fCarWidth/2);
		curveRight = v*(radius + pCar->fCarWidth/2);
	}

	pCar->fBackWheelLeftSpeed = fSpeed*curveLeft/curveCenter;
	pCar->fBackWheelRightSpeed = fSpeed*curveRight/curveCenter;
}


void CarPrint(const SCar* pCar)
{
	printf("LeftAngle:%f RightAngle:%f LeftSpeed:%f RightSpeed:%f\n", 
		pCar->fFrontWheelLeftAngle, 
		pCar->fFrontWheelRightAngle,
		pCar->fBackWheelLeftSpeed,
		pCar->fBackWheelRightSpeed
	);
}


bool RobocupInit( void )
{
	car.fCarLength = 200;
	car.fCarWidth = 190;

	char s[256];

	// Detect motors for back wheels
	bool bDetectMotors = false;
	if ( tacho_is_plugged( MOTOR_BOTH, TACHO_TYPE__NONE_ )) 
	{
		bDetectMotors = true;
		max_speed = tacho_get_max_speed( MOTOR_LEFT, 0 );
		tacho_reset( MOTOR_BOTH );
	} 
	printf("%s Detecting motors at B and C ", bDetectMotors ? "[  OK  ]" : "[FAILED]");

	ev3_servo_init();
/*
	int i;       
	for ( i = 0; i < DESC_LIMIT; i++ ) 
	{
		if ( ev3_servo[ i ].type_inx != SERVO_TYPE__NONE_ ) 
		{
			printf( "  type = %s\n", ev3_servo_type( ev3_servo[ i ].type_inx ));
			printf( "  port = %s\n", ev3_servo_port_name( i, s ));
			bServos = true;
		}
	}
*/

	// Detect servo for front right wheel
	bool bDetectServoRight = false;
	if ( ev3_search_servo_plugged_in( INPUT_1, SERVO_1, &snRight, 0 )) 
	{
		bDetectServoRight = true;
		set_servo_position_sp( snRight, 0 );
		set_servo_command_inx( snRight, SERVO_RUN);
	} 
	printf("%s Detecting servo right ", bDetectServoRight ? "[  OK  ]" : "[FAILED]");

	// Detect servo for front left wheel
	bool bDetectServoLeft = false;
	if ( ev3_search_servo_plugged_in( INPUT_1, SERVO_2, &snLeft, 0 )) 
	{
		set_servo_position_sp( snLeft, 0 );
		set_servo_command_inx(snLeft, SERVO_RUN);
	}
	printf("%s Detecting servo left ", bDetectServoLeft ? "[  OK  ]" : "[FAILED]");

	bool bDetectServoBatteryLevel = false;
	float battery = 0.0;
	rc = sensor_search( MS_8CH_SERVO );
	if(rc)
	{
		bDetectServoBatteryLevel = true;
		float battery = sensor_get_value0(rc, 0);		
		printf("battery %f mv\n", battery);
	}
	printf("%s Detecting servo battery ", bDetectServoBatteryLevel ? "[  OK  ]" : "[FAILED]");

	return bDetectMotors && bDetectServoRight && bDetectServoLeft && bDetectServoBatteryLevel;
}

void UpdateCar(float speed, float angle)
{
	CarComputeTurningAngle(&car, angle, speed);

	int motorSetpointLeft = -(int)(max_speed * car.fBackWheelLeftSpeed / 100); 
	int motorSetpointRight = -(int)(max_speed * car.fBackWheelRightSpeed / 100); 
	tacho_set_speed_sp( MOTOR_LEFT, motorSetpointLeft );
	tacho_set_speed_sp( MOTOR_RIGHT, motorSetpointRight );
	tacho_run_forever( MOTOR_BOTH ); 		

	int servoLeftSp = AngleToServoLeft(car.fFrontWheelLeftAngle);
	int servoRightSp = AngleToServoRight(car.fFrontWheelRightAngle); 
	set_servo_position_sp(snLeft, servoLeftSp);
	set_servo_position_sp(snRight, servoRightSp);
}

//-----------------------------------------------------------------------------
// Script handler functions
//-----------------------------------------------------------------------------


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

		float fSpeed = GetVar(V_SPEED);
		float fAngle = GetVar(V_ANGLE);
		UpdateCar(fSpeed, fAngle);

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

		float fSpeed = GetVar(V_SPEED);
		float fAngle = GetVar(V_ANGLE);
		UpdateCar(-fSpeed, fAngle);

		s->index++;
	}

	return false;
}


//-----------------------------------------------------------------------------
// Main entry
//-----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	RobocupInit();

	if(argc > 1)
	{
		// Load script
		FILE* file = fopen(argv[1], "r");
		fseek(file, 0l, SEEK_END);
		size_t size = ftell(file);
		fseek(file, 0l, SEEK_SET);
		char* p = malloc(size + 1);
		int n = fread(p, 1, size, file);
		p[n] = 0;
        SProgram program;
		if(Compile(p, &program))
        {
            // Run program
            PrintProgram(&program);
            RunProgram(&program);
            
            DeleteProgram(&program);
        }
		free(p);
		fclose(file);
	}
    return 0;
}
