#include <stdio.h>
#include <stdbool.h>
#include "brick.h"

#include "ev3_servo.h"
int g_bAlive = true; ///< Program is alive
int g_buttons = IR_REMOTE__NONE_;

#define MOTOR_LEFT     OUTC
#define MOTOR_RIGHT    OUTB
#define MOTOR_BOTH     ( MOTOR_LEFT | MOTOR_RIGHT )

#define IR_CHANNEL     0 

int max_speed;         /* Motor maximal speed (will be detected) */ 
POOL_T ir;             /* IR sensor port (will be detected) */
POOL_T rc;

float speed = 0.0f;
float angle = 0.0f;
uint8_t sn;

int init( void )
{
	uint8_t command[8];
	uint8_t response[8];
	char s[256];

	if ( tacho_is_plugged( MOTOR_BOTH, TACHO_TYPE__NONE_ )) {  /* any type of motor */
		max_speed = tacho_get_max_speed( MOTOR_LEFT, 0 );
		tacho_reset( MOTOR_BOTH );
	} else {
		printf( "Please, plug LEFT motor in C port,\n"
		"RIGHT motor in B port and try again.\n"
		);
		/* Inoperative without motors */
		return ( 0 );
	}

	ir = sensor_search( LEGO_EV3_IR );
	if ( ir ) {
		ir_set_mode_ir_remote( ir );

		printf(	"IR remote control:\n"
		"RED UP   & BLUE UP   - forward\n"
		"RED DOWN & BLUE DOWN - backward\n"
		"RED UP   | BLUE DOWN - left\n"
		"RED DOWN | BLUE UP   - right\n"
		);
	} else {
		printf( "IR sensor is NOT found.\n"
		"Please, use the EV3 brick buttons.\n"
		);
	}

	ev3_servo_init();

	
	
	 
	int i;       
printf( "Found servo motors:\n" );
	for ( i = 0; i < DESC_LIMIT; i++ ) {
		if ( ev3_servo[ i ].type_inx != SERVO_TYPE__NONE_ ) {
			printf( "  type = %s\n", ev3_servo_type( ev3_servo[ i ].type_inx ));
			printf( "  port = %s\n", ev3_servo_port_name( i, s ));
		}
	}
	if ( ev3_search_servo_plugged_in( INPUT_1, SERVO_1, &sn, 0 )) {
		printf( "Servo motor is found, setting position...\n" );
		set_servo_position_sp( sn, 0 );
	} else {
		printf( "Servo motor is NOT found\n" );
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
		command[0] = 0x41;
		response[0] = 0;
		get_sensor_address(rc, (char*)command, 1);
		printf("address:%d\n", (int)command[0]);
		command[0] = 0x41;
		set_sensor_bin_data(rc, command, 1);
		get_sensor_bin_data(rc, response, 1);
		printf("value0: %d\n", response[0]);
		float val = sensor_get_value1(rc, 0);		
		printf("val: %f\n", val);
	}

	printf( "Press BACK on the EV3 brick for EXIT...\n" );

	return ( 1 );
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


void UpdateIr()
{
	if ( ir == SOCKET__NONE_ )
	{
		return;
	}

	bool bSpeedChanged = false;
	bool bAngleChanged = false;
	int buttons;
	buttons = sensor_get_value( IR_CHANNEL, ir, IR_REMOTE__NONE_ );

	if(buttons != g_buttons)
	{
		g_buttons = buttons;

		switch( buttons )
		{
			case RED_UP_BLUE_UP:
				break;
	
			case RED_DOWN_BLUE_DOWN:
				break;
	
			case RED_UP:
				if(speed < 100.0)
				{
					speed += 5.0f;
					bSpeedChanged = true;
				}
				break;
	
			case RED_UP_BLUE_DOWN:
				break;
	
			case BLUE_DOWN:
				if(angle > -50.0f)
				{
					angle -= 5.0f;
					bAngleChanged = true;
				}
				break;
	
			case BLUE_UP:
				if(angle < 50.0f)
				{
					angle += 5.0f;
					bAngleChanged = true;
				}
				break;
	
			case RED_DOWN_BLUE_UP:
				break;
	
			case RED_DOWN:
				if(speed > -100.0)
				{
					speed -= 5.0f;
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

		int motorSetpoint = (int)(max_speed * speed / 100); 
		tacho_set_speed_sp( MOTOR_BOTH, motorSetpoint );
		tacho_run_forever( MOTOR_BOTH ); 		
	}

	if(bAngleChanged)
	{
		printf("angle %f\n", angle);
		set_servo_position_sp(sn, (int)angle);
/*
		uint16_t pulse = (float)((angle * 500 / 50) + 1500);
		printf("pulse %d\n", pulse);
		uint8_t cmd[3];
		cmd[0] = 0x42;
		cmd[1] = pulse & 0x00FF;
		cmd[2] = (pulse >> 8) & 0x00FF;
		set_sensor_bin_data(rc, cmd, 3);

		cmd[0] = 0x44;
		set_sensor_bin_data(rc, cmd, 3);

		uint8_t response[2];
		response[0] = 0;
		response[1] = 0;
		set_sensor_bin_data(rc, cmd, 1);
		get_sensor_bin_data(rc, response, 1);
		
		printf("val: %d %d\n", (int)response[0]);
*/		
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

	while ( g_bAlive ) 
	{
		UpdateBrick();
		UpdateIr();
		UpdateDrive();
		sleep_ms(10);
	}
	brick_uninit();

	printf( "*** ( EV3 ) Bye! ***\n" );
	return ( 0 );
}
