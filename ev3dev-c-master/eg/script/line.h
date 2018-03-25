#ifndef _LINE_H_
#define _LINE_H_

#include <stdint.h>
#include <stdbool.h>

#define POINTS (8)
#define LINE_EDGE_THRESHOLD (0.1)
#define LINE_WIDTH (0.82)


typedef struct
{
    uint8_t data[POINTS];
    float norm[POINTS];
    uint8_t nCenter;
    float center;
	uint8_t nLeft;
	float left;
	uint8_t nRight;
	float right;
    float heading;
    float odometer;
    uint8_t full;
} SLine;

void LineInit(SLine* pLine);
void LinePrint(SLine* pLine);
void LineDataPrint(SLine* pLine);
void LineAnalyze(SLine* pLine, float black, float white, float threshold);

#define LINE_SENSOR_DEPTH (5)

typedef struct
{
    int first;
    int last;
    int count;
    SLine lines[LINE_SENSOR_DEPTH + 2];
} SLineSensor;

void LineSensorInit(SLineSensor* pLineSensor);
bool LineSensorJunctionLeft(SLineSensor* pLineSensor);
bool LineSensorJunctionRight(SLineSensor* pLineSensor);
void LineSensorPush(SLineSensor* pLineSensor, SLine* pLine);
bool LineSensorLeft(SLineSensor* pLineSensor, float* pPos);
bool LineSensorRight(SLineSensor* pLineSensor, float* pPos);
bool LineSensorCenter(SLineSensor* pLineSensor, float* pPos);
bool LineSensorLineFull(SLineSensor* pLineSensor);
bool LineSensorNoLine(SLineSensor* pLineSensor);
bool LineSensorAnyLine(SLineSensor* pLineSensor);
void LineSensorPrint(SLineSensor* pLineSensor);

#endif
