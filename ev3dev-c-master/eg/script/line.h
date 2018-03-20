#ifndef _LINE_H_
#define _LINE_H_

#include <stdint.h>


#define POINTS (8)
#define LINE_EDGE_THRESHOLD (0.1)


typedef struct
{
    uint8_t data[POINTS];
    float norm[POINTS];
    uint8_t n;
    float p0;
	uint8_t nLeftEdges;
	float leftEdge;

	uint8_t nRightEdges;
	float rightEdge;
} SLine;

void LineInit(SLine* pLine);
void LinePrint(SLine* pLine);
void LineAnalyze(SLine* pLine, float base, float high);




#endif
