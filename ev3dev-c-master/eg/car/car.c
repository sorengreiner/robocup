#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "brick.h"

#include "ev3_servo.h"


#include <math.h>

#define M_PI		3.14159265358979323846 

long TimeMilliseconds(void)
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts); // Works on FreeBSD
//	clock_gettime(CLOCK_REALTIME, &ts); // Works on Linux
	return (long)(ts.tv_nsec/1000000);
}


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

int g_bAlive = true; ///< Program is alive
int g_buttons = IR_REMOTE__NONE_;

#define MOTOR_LEFT     OUTC
#define MOTOR_RIGHT    OUTB
#define MOTOR_BOTH     ( MOTOR_LEFT | MOTOR_RIGHT )

#define IR_CHANNEL     0 

int max_speed;         /* Motor maximal speed (will be detected) */ 
POOL_T ir;             /* IR sensor port (will be detected) */
POOL_T rc;
uint8_t snRight;
uint8_t snLeft;

float speed = 0.0f;
float angle = 0.0f;

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


int init( void )
{
	car.fCarLength = 200;
	car.fCarWidth = 190;

	char s[256];

	if ( tacho_is_plugged( MOTOR_BOTH, TACHO_TYPE__NONE_ )) 
	{
		max_speed = tacho_get_max_speed( MOTOR_LEFT, 0 );
		tacho_reset( MOTOR_BOTH );
	} 
	else 
	{
		printf( "Please, plug LEFT motor in C port,\n"
		"RIGHT motor in B port and try again.\n"
		);
		/* Inoperative without motors */
		return ( 0 );
	}

	ir = sensor_search( LEGO_EV3_IR );
	if ( ir ) 
	{
		ir_set_mode_ir_remote( ir );

		printf(	"IR remote control:\n"
		"RED UP   & BLUE UP   - forward\n"
		"RED DOWN & BLUE DOWN - backward\n"
		"RED UP   | BLUE DOWN - left\n"
		"RED DOWN | BLUE UP   - right\n"
		);
	} 
	else 
	{
		printf( "IR sensor is NOT found.\n"
		"Please, use the EV3 brick buttons.\n"
		);
	}

	ev3_servo_init();
	 
	int i;       
	printf( "Found servo motors:\n" );
	for ( i = 0; i < DESC_LIMIT; i++ ) 
	{
		if ( ev3_servo[ i ].type_inx != SERVO_TYPE__NONE_ ) 
		{
			printf( "  type = %s\n", ev3_servo_type( ev3_servo[ i ].type_inx ));
			printf( "  port = %s\n", ev3_servo_port_name( i, s ));
		}
	}

	if ( ev3_search_servo_plugged_in( INPUT_1, SERVO_1, &snRight, 0 )) 
	{
		printf( "Servo motor right is found, setting position...\n" );
		set_servo_position_sp( snRight, 0 );
		set_servo_command_inx( snRight, SERVO_RUN);
	} 
	else 
	{
		printf( "Servo motor right is NOT found\n" );
	} 

	if ( ev3_search_servo_plugged_in( INPUT_1, SERVO_2, &snLeft, 0 )) 
	{
		printf( "Servo motor left is found, setting position...\n" );
		set_servo_position_sp( snLeft, 0 );
		set_servo_command_inx(snLeft, SERVO_RUN);
	} 
	else 
	{
		printf( "Servo motor left is NOT found\n" );
	} 

	rc = sensor_search( MS_8CH_SERVO );
	if(rc)
	{
		printf(	"Mindsensor 8Ch servo found\n" );
		INX_T mode = sensor_get_mode(rc);
		printf("mode: %d\n", mode);
		if(mode == MS_8CH_SERVO_V3)
		{
			printf("MS_8CH_SERVO_V3 found\n");
		}
		else
		{
			printf("Other mode found\n");
		}
		float battery = sensor_get_value0(rc, 0);		
		printf("battery %f mv\n", battery);
	}

	printf( "Press BACK on the EV3 brick for EXIT...\n" );

	return 1;
}


void UpdateBrick()
{
	uint8_t keys;

	keys = brick_keys();

	switch(keys)
	{
		case EV3_KEY_BACK:
			g_bAlive = false;
		break;

		default:
		break;
	}
}


void UpdateIr(float delta)
{
	if ( ir == SOCKET__NONE_ )
	{
		return;
	}

	bool bSpeedChanged = false;
	bool bAngleChanged = false;
	int buttons;
	buttons = sensor_get_value( IR_CHANNEL, ir, IR_REMOTE__NONE_ );

//	if(buttons != g_buttons)
	{
		g_buttons = buttons;

		switch( buttons )
		{
			case RED_UP_BLUE_UP:
				break;
	
			case RED_DOWN_BLUE_DOWN:
				speed = 0.0f;
				angle = 0.0f;
				break;
	
			case RED_UP:
				if(speed < 100.0)
				{
					speed += 50.0f*delta;
					bSpeedChanged = true;
				}
				break;
	
			case RED_UP_BLUE_DOWN:
				break;
	
			case BLUE_DOWN:
				if(angle > -60.0f)
				{
					angle -= 50.0f*delta;
					bAngleChanged = true;
				}
				break;
	
			case BLUE_UP:
				if(angle < 60.0f)
				{
					angle += 50.0f*delta;
					bAngleChanged = true;
				}
				break;
	
			case RED_DOWN_BLUE_UP:
				break;
	
			case RED_DOWN:
				if(speed > -100.0)
				{
					speed -= 50.0f*delta;
					bSpeedChanged = true;
				}
				break;
	
			case IR_REMOTE__NONE_:
			case RED_UP_RED_DOWN:
			case BLUE_UP_BLUE_DOWN:
			case BEACON_MODE_ON:
				break; 
		}

	}

	if(bSpeedChanged)
	{
		printf("speed %f\n", speed);
	}

	if(bAngleChanged)
	{
		printf("angle %f\n", angle);
	}

	if(bSpeedChanged || bAngleChanged)
	{
		CarComputeTurningAngle(&car, angle, speed);
//		CarPrint(&car);

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
}


void UpdateDrive()
{
}



int main( void )
{
	printf( "Waiting the EV3 brick online...\n" );
	if ( !brick_init()) return ( 1 );

	printf( "*** ( EV3 ) Hello! ***\n" );
	g_bAlive = init();


	struct timespec res;
	clock_getres(CLOCK_MONOTONIC, &res);
	printf("res:%ld\n", res.tv_nsec);

	while ( g_bAlive ) 
	{
		uint32_t timeBegin = TimeMilliseconds();
		UpdateBrick();
		UpdateIr(0.1f);
		UpdateDrive();

		uint32_t timeEnd = TimeMilliseconds();

//		printf("t:%ld\n", (long)timeNow);		
		uint32_t timeDelta = timeEnd >= timeBegin ? timeEnd - timeBegin : 1000 + timeEnd - timeBegin;

//		printf("b:%u e:%u d:%u\n", timeBegin, timeEnd, timeDelta);

//		sleep_ms(100);

		if(timeDelta < 100)
		{
			sleep_ms(100 - timeDelta);
		}
		else
		{
			printf( "warning overflow delta\n" );
		}


//		timeLast = timeNow;
	}

	tacho_set_speed_sp( MOTOR_BOTH, 0 );
	tacho_run_forever( MOTOR_BOTH );
	set_servo_position_sp(snLeft, 0);
	set_servo_position_sp(snRight, 0);
	sleep_ms(500);
	set_servo_command_inx(snLeft, SERVO_FLOAT);
	set_servo_command_inx(snRight, SERVO_FLOAT);
	brick_uninit();

	printf( "*** ( EV3 ) Bye! ***\n" );
	return ( 0 );
}
