#include "robocup.h"
#include "magnus.h"
#include "line.h"
#include "brick.h"
#include "ev3_servo.h"
#include "keys.h"
#include "pid.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>


typedef struct
{
	uint64_t t0;
	uint64_t t1;
	float rx0;
	float ry0;
	float rz0;
	float rx1;
	float ry1;
	float rz1;
	float yaw;
} SInertialNavigation;


#define MOTOR_LEFT     OUTC
#define MOTOR_RIGHT    OUTB
#define MOTOR_TOOL     OUTD
#define MOTOR_BOTH     ( MOTOR_LEFT | MOTOR_RIGHT )
#define IR_CHANNEL     0 

int max_speed;         /* Motor maximal speed (will be detected) */ 
int max_speed_tool;         /* Motor maximal speed (will be detected) */ 
POOL_T ir;             /* IR sensor port (will be detected) */
POOL_T rc;
uint8_t snRight;
uint8_t snLeft;
uint8_t snColor;
uint8_t snLine;
uint8_t snGyro;

int leftCenter = -8;
int rightCenter = 2;
SCar car;
SInertialNavigation inertial;
SLine lineSensor;


void UpdateGyro();


//-----------------------------------------------------------------------------
// Conversion
//-----------------------------------------------------------------------------

float LinePosToPhysical(float pos)
{
	return (LINESENSOR_WIDTH_MM*pos - LINESENSOR_WIDTH_MM/2);
}


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


int AngleToServoLeft(float angle)
{
	int servo = (int)(-angle*118.0f/90.0f) + leftCenter;
	return servo;
}


int AngleToServoRight(float angle)
{
	int servo = (int)(-angle*118.0f/90.0f) + rightCenter;
	return servo;
}


int PositionToToolSp(float position)
{
	if(position < 0)
	{
		position = 0;
	}
	if(position > 1.0)
	{
		position = 1.0;
	}

	int sp = (int)(-position*380);
	return sp;
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
		curveLeft = v*(radius - pCar->fBackWidth/2);
		curveRight = v*(radius + pCar->fBackWidth/2);
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
	car.fBackWidth = 180.0;

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

	// Detect motor for tool
	bool bDetectTool = false;
	if ( tacho_is_plugged( MOTOR_TOOL, TACHO_TYPE__NONE_ )) 
	{
		bDetectTool = true;
		max_speed_tool = tacho_get_max_speed( MOTOR_TOOL, 0 );
		tacho_reset( MOTOR_TOOL );
	} 
	printf("%s Detecting tool motor at D\n", bDetectTool ? "[  OK  ]" : "[FAILED]");

	ev3_servo_init();

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

	bool bDetectGyro = false;
	if ( ev3_search_sensor( LEGO_EV3_GYRO, &snGyro, 0 )) 
	{
		set_sensor_mode_inx( snGyro, LEGO_EV3_GYRO_GYRO_CAL );
		printf("Calibrating gyro");
		sleep_ms(2000);
		set_sensor_mode_inx( snGyro, LEGO_EV3_GYRO_GYRO_ANG ); 
		UpdateGyro();
	
		bDetectGyro = true;
	}
	printf("%s Detecting gyro\n", bDetectGyro ? "[  OK  ]" : "[FAILED]");

	return bDetectMotors && bDetectServoRight && bDetectServoLeft && bDetectServoBatteryLevel && bDetectLineSensor && bDetectGyro;
}


//-----------------------------------------------------------------------------
// @brief speed	Speed in cm/s
// @brief wheel turn angle in degrees (angle of fiktive center wheel)
//-----------------------------------------------------------------------------
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


void UpdateTool(float speed, float position)
{
	int motorSetpoint = PositionToToolSp(position); 
	tacho_set_speed_sp( MOTOR_TOOL, speed );
	tacho_set_position_sp(MOTOR_TOOL, motorSetpoint);
	tacho_run_to_abs_pos( MOTOR_TOOL ); 		
}


SLine* GetLineSensor(void)
{
    return &lineSensor;
}


void UpdateLineSensor(void)
{
	int n = get_sensor_bin_data( snLine, lineSensor.data, 8);
	if(n == 8)
	{
		uint8_t* p = lineSensor.data;
		LineAnalyze(&lineSensor, GetVar(V_BLACK), GetVar(V_WHITE));
		LineDataPrint(&lineSensor);	
	}
}


void InertialNavigationInit(void)
{
	inertial.t0 = TimeMilliseconds() - 1;
	inertial.t1 = inertial.t0;
	inertial.rx0 = 0;
	inertial.ry0 = 0;
	inertial.rz0 = 0;
	inertial.rx1 = 0;
	inertial.ry1 = 0;
	inertial.rz1 = 0;
	inertial.yaw = 0;		
}


//-----------------------------------------------------------------------------
// Read gyro and update yaw
//-----------------------------------------------------------------------------

void UpdateGyro(void)
{
	float value;
	uint64_t t = TimeMilliseconds();
	if(get_sensor_value0(snGyro, &value) > 0)
	{
		inertial.yaw = value;
	}
}

//-----------------------------------------------------------------------------
// Update vars read from physical sensors and set
//-----------------------------------------------------------------------------

void UpdateVars(float delta)
{
	float time = GetVar(V_TIME);
	UpdateGyro();
	// Update odometer
	int tachoLeft = tacho_get_position( MOTOR_LEFT, 0 );
	int tachoRight = tacho_get_position( MOTOR_RIGHT, 0 );
	float odometerLeft = TachoToMeter(-tachoLeft);
	float odometerRight = TachoToMeter(-tachoRight);
	float odometerAbsolute = (odometerLeft + odometerRight)/2;
	float heading = inertial.yaw;
	float odometer = GetVar(V_ODOMETER);
	float mark = GetVar(V_MARK);
	odometer = odometerAbsolute - GetVar(V_MARK);

	SetVar(V_ODOMETER, odometer);
	SetVar(V_LODOMETER, odometerLeft);
	SetVar(V_RODOMETER, odometerRight);
	SetVar(V_AODOMETER, odometerAbsolute);
	SetVar(V_HEADING, heading);

	time += delta;

	SetVar(V_TIME, time);

	UpdateLineSensor();
}


//-----------------------------------------------------------------------------
// Assign var and perform side-effects
//-----------------------------------------------------------------------------

void AssignVar(int noun, float value)
{
	switch(noun)
	{
	case V_NIL:
	case V_TRUE:
	case V_FALSE:
		break;
	case V_SPEED:
		SetVar(noun, value);
		break;
	case V_ODOMETER:
		{
			SetVar(noun, value);
			float fOdometerAbsolute = GetVar(V_AODOMETER);
			SetVar(V_MARK, fOdometerAbsolute);
		}
		break;
	case V_LODOMETER:
		{
		SetVar(noun, value);
		int tacho = MeterToTacho(value);
		tacho_set_position( MOTOR_LEFT, tacho );
		}
		break;
	case V_RODOMETER:
		{
		SetVar(noun, value);
		int tacho = MeterToTacho(value);
		tacho_set_position( MOTOR_RIGHT, tacho );
		}
		break;
	case V_AODOMETER:
		{
		SetVar(noun, value);
		int tacho = MeterToTacho(value);
		tacho_set_position( MOTOR_LEFT, tacho );
		tacho_set_position( MOTOR_RIGHT, tacho );
		}
		break;
	case V_MARK:
	case V_ANGLE:
	case V_RADIUS:
	case V_TIME:
	case V_HEADING:
	case V_XPOS:
	case V_YPOS:
	case V_STEER:
	default:
		SetVar(noun, value);
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

	if(!KeysOpen())
	{
		printf("keys wont open\n");
		return 1;
	}
 
	LineInit(&lineSensor);
	InertialNavigationInit();

	if(!RobocupInit())
	{
		printf("Robocup init failed\n");
		return 1;
	}

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
            RunProgram(&program);
            
            DeleteProgram(&program);
        }
		free(p);
		fclose(file);
	}

	UpdateCar(0,0);
	sleep_ms(500);

	tacho_set_speed_sp( MOTOR_BOTH, 0 );
	tacho_run_forever( MOTOR_BOTH );
	set_servo_command_inx(snLeft, SERVO_FLOAT);
	set_servo_command_inx(snRight, SERVO_FLOAT);	
	tacho_reset(MOTOR_TOOL);
	KeysClose();

	brick_uninit();	
    return 0;
}
