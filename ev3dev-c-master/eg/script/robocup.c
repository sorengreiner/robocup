#include "robocup.h"
#include "magnus.h"
#include "line.h"
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
uint8_t snColor;
uint8_t snLine;

int leftCenter = -7;
int rightCenter = 4;
SCar car;


int AngleToServoLeft(float angle)
{
	int servo = (int)(-angle*106.0f/90.0f) + leftCenter;
	return servo;
}


int AngleToServoRight(float angle)
{
	int servo = (int)(-angle*106.0f/90.0f) + rightCenter;
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

	float fBackWidth = 180.0;
	if(fabs(v) > angularLimit)
	{
		float radius = pCar->fCarLength/tanv;
		curveCenter = v*radius;
		curveLeft = v*(radius - fBackWidth/2);
		curveRight = v*(radius + fBackWidth/2);
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
	car.fCarLength = 208;
	car.fCarWidth = 143.46;

	char s[256];

	// Detect motors for back wheels
	bool bDetectMotors = false;
	if ( tacho_is_plugged( MOTOR_BOTH, TACHO_TYPE__NONE_ )) 
	{
		bDetectMotors = true;
		max_speed = tacho_get_max_speed( MOTOR_LEFT, 0 );
		tacho_reset( MOTOR_BOTH );
	} 
	printf("%s Detecting motors at B and C\n", bDetectMotors ? "[  OK  ]" : "[FAILED]");

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
	printf("%s Detecting servo right\n", bDetectServoRight ? "[  OK  ]" : "[FAILED]");

	// Detect servo for front left wheel
	bool bDetectServoLeft = false;
	if ( ev3_search_servo_plugged_in( INPUT_1, SERVO_2, &snLeft, 0 )) 
	{
		bDetectServoLeft = true;
		set_servo_position_sp( snLeft, 0 );
		set_servo_command_inx(snLeft, SERVO_RUN);
	}
	printf("%s Detecting servo left\n", bDetectServoLeft ? "[  OK  ]" : "[FAILED]");

	bool bDetectServoBatteryLevel = false;
	float battery = 0.0;
	rc = sensor_search( MS_8CH_SERVO );
	if(rc)
	{
		bDetectServoBatteryLevel = true;
		float battery = sensor_get_value0(rc, 0);		
		printf("battery %f mv\n", battery);
	}
	printf("%s Detecting servo battery\n", bDetectServoBatteryLevel ? "[  OK  ]" : "[FAILED]");

	bool bDetectColorSensor = false;
	if ( ev3_search_sensor( LEGO_EV3_COLOR, &snColor, 0 )) 
	{
		bDetectColorSensor = true;
		set_sensor_mode( snColor, "COL-REFLECT" ); 
	}
	printf("%s Detecting color sensor\n", bDetectColorSensor ? "[  OK  ]" : "[FAILED]");

	bool bDetectLineSensor = false;
	if ( ev3_search_sensor( MS_LIGHT_ARRAY, &snLine, 0 )) 
	{
		bDetectLineSensor = true;
	}
	printf("%s Detecting line sensor\n", bDetectLineSensor ? "[  OK  ]" : "[FAILED]");

	return bDetectMotors && bDetectServoRight && bDetectServoLeft && bDetectServoBatteryLevel && bDetectLineSensor;
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


SLine lineSensor;


void UpdateLineSensor(void)
{
	uint8_t values[8];
	int n = get_sensor_bin_data( snLine, lineSensor.data, 8);
	if(n == 8)
	{
		LineAnalyze(&lineSensor);
//		printf("le: %d %f re: %d %f\n", lineSensor.nLeftEdges, lineSensor.leftEdge, lineSensor.nRightEdges, lineSensor.rightEdge);
	}
}

//-----------------------------------------------------------------------------
// Script handler functions
//-----------------------------------------------------------------------------


bool Line(SState* s, int noun0, float value0, int noun1, float value1)
{
	return (lineSensor.nLeftEdges > 0) || (lineSensor.nRightEdges > 0);
}


#define LINESENSOR_WIDTH_MM (46.0)

float LinePosToPhysical(float pos)
{
	return -(LINESENSOR_WIDTH_MM*pos/POINTS - LINESENSOR_WIDTH_MM/2);
}


typedef struct
{
	float fLastLinePos;
} SFollowState;


bool Follow(SState* s, int noun0, float value0, int noun1, float value1) 
{ 
	SFollowState* p = (SFollowState*)s->stack;
	if(s->index == 0)
	{
		printf("Follow\n");
		p->fLastLinePos = 0.0f;
	}

	float fSpeed = GetVar(V_SPEED);

	if(lineSensor.n > 0)
	{
		p->fLastLinePos = LinePosToPhysical(lineSensor.p0);
	}

	float fAngle = p->fLastLinePos;
	UpdateCar(fSpeed, fAngle);

	return false;
}


bool FollowLeft(SState* s, int noun0, float value0, int noun1, float value1) 
{ 
	SFollowState* p = (SFollowState*)s->stack;
	if(s->index == 0)
	{
		printf("FollowLeft\n");
		p->fLastLinePos = 0.0f;
	}

	float fSpeed = GetVar(V_SPEED);

	if(lineSensor.nLeftEdges > 0)
	{
		p->fLastLinePos = LinePosToPhysical(lineSensor.leftEdge);
	}

	float fAngle = p->fLastLinePos;
	UpdateCar(fSpeed, fAngle);

	return false;
}


bool FollowRight(SState* s, int noun0, float value0, int noun1, float value1) 
{ 
	SFollowState* p = (SFollowState*)s->stack;
	if(s->index == 0)
	{
		printf("FollowRight\n");
		p->fLastLinePos = 0.0f;
	}

	float fSpeed = GetVar(V_SPEED);

	if(lineSensor.nRightEdges > 0)
	{
		p->fLastLinePos = LinePosToPhysical(lineSensor.rightEdge);
	}

	float fAngle = p->fLastLinePos;
	UpdateCar(fSpeed, fAngle);

	return false;
}


bool Forward(SState* s, int noun0, float value0, int noun1, float value1) 
{ 
	if(s->index == 0)
	{
		printf("Forward\n");
		float fSpeed = GetVar(V_SPEED);
		float fAngle = GetVar(V_ANGLE);
		UpdateCar(fSpeed, fAngle);
	}

	return false;
}


bool Backward(SState* s, int noun0, float value0, int noun1, float value1) 
{ 
	if(s->index == 0)
	{
		printf("Backward\n");
		float fSpeed = GetVar(V_SPEED);
		float fAngle = GetVar(V_ANGLE);
		UpdateCar(-fSpeed, fAngle);
	}

	return false;
}

#define BACKWHEEL_DIAMETER_MM ( 81.6 )
#define TACHO_RESOLUTION ( 360 )

int MeterToTacho(float meter)
{
	int tacho = (int)(1000*TACHO_RESOLUTION*meter/(M_PI*BACKWHEEL_DIAMETER_MM));
	return tacho;
}

float TachoToMeter(int tacho)
{
	float meter = tacho*M_PI*(BACKWHEEL_DIAMETER_MM/1000)/TACHO_RESOLUTION;
	return meter;
}


void UpdateVars(float delta)
{
	float time = GetVar(V_TIME);
	// Update odometer
	int tachoLeft = tacho_get_position( MOTOR_LEFT, 0 );
	int tachoRight = tacho_get_position( MOTOR_RIGHT, 0 );
//	printf("tacho: %d, %d\n", tachoLeft, tachoRight);
	float distanceLeft = TachoToMeter(-tachoLeft);
	float distanceRight = TachoToMeter(-tachoRight);
	float distance = (distanceLeft + distanceRight)/2;
//	printf("d:%f dl:%f dr:%f\n", distance, distanceLeft, distanceRight);
	SetVar(V_ODOMETER, distance);

	time += delta;

	SetVar(V_TIME, time);

	UpdateLineSensor();
}

void AssignVar(int noun, float value)
{
	switch(noun)
	{
	case V_NIL:
		break;
	case V_SPEED:
		SetVar(noun, value);
		break;
	case V_ODOMETER:
		{
			SetVar(noun, value);
			int tacho = MeterToTacho(value);
			tacho_set_position( MOTOR_LEFT, tacho );
			tacho_set_position( MOTOR_RIGHT, tacho );
		}
		break;
	case V_ANGLE:
		SetVar(noun, value);
		break;
	case V_TIME:
		SetVar(noun, value);
		break;
	case V_HEADING:
		SetVar(noun, value);
		break;
	case V_XPOS:
		SetVar(noun, value);
		break;
	case V_YPOS:
		SetVar(noun, value);
		break;
	default:
		break;
	}
}


//-----------------------------------------------------------------------------
// Main entry
//-----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	if(!brick_init())
	{
		return 1;
	}

	LineInit(&lineSensor);

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

	tacho_set_speed_sp( MOTOR_BOTH, 0 );
	tacho_run_forever( MOTOR_BOTH );
	set_servo_command_inx(snLeft, SERVO_FLOAT);
	set_servo_command_inx(snRight, SERVO_FLOAT);	

	brick_uninit();	
    return 0;
}
