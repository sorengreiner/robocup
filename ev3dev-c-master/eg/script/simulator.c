#include "magnus.h"
#include "simulator.h"
#include "line.h"
#include "car.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h>   // for nanosleep
#else
#include <unistd.h> // for usleep
#endif


void sleep_ms(int milliseconds) // cross-platform sleep function
{
#ifdef WIN32
    Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
#else
    usleep(milliseconds * 1000);
#endif
}



SLine lineSensor;

SLine* GetLineSensor(void)
{
    return &lineSensor;
}


void UpdateLineSensor(void)
{
    uint8_t data[8] = {40,40,60,100,100,100,60,40};
    memcpy(lineSensor.data, data, 8);
    LineAnalyze(&lineSensor, GetVar(V_BLACK), GetVar(V_WHITE), 0.66f);
//    LineDataPrint(&lineSensor);	
}


void UpdateTool(float speed, float position)
{
}


float LinePosToPhysical(float pos)
{
	return (LINESENSOR_WIDTH_MM*pos - LINESENSOR_WIDTH_MM/2);
}


void UpdateCar(float speed, float angle)
{
}


void UpdateVars(float delta)
{
	float angle = GetVar(V_ANGLE);
	float time = GetVar(V_TIME);
	float speed = GetVar(V_SPEED);
	float heading = GetVar(V_HEADING);
	float odometer = GetVar(V_ODOMETER);
	float x = GetVar(V_XPOS);
	float y = GetVar(V_YPOS);

	// Update time variable
	time += delta;

	// Update heading and pos as approximated predicted values
	heading += delta*angle;
	heading = fmodf(heading, 360.0);
	odometer += delta*(speed/100);
	x += delta*(speed/100)*cosf(3.14159*heading/180.0);
	y += delta*(speed/100)*sinf(3.14159*heading/180.0);

	SetVar(V_ANGLE, angle);
	SetVar(V_TIME, time);
	SetVar(V_SPEED, speed);
	SetVar(V_HEADING, heading);
	SetVar(V_ODOMETER, odometer);
	SetVar(V_XPOS, x);
	SetVar(V_YPOS, y);

    UpdateLineSensor();
//	printf("time=%f heading=%f odometer=%f x=%f y=%f speed=%f angle=%f\n", time, heading, odometer, x, y, speed, angle);
}


void AssignVar(int noun, float value)
{
	SetVar(noun, value);
}


//-----------------------------------------------------------------------------
// Main entry
//-----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
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
    return 0;
}



