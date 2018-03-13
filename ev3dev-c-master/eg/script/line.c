#include "line.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>


void LineInit(SLine* pLine)
{
   	pLine->n = 0;
    pLine->p0 = 0.0f;
	pLine->nLeftEdges = 0;
	pLine->leftEdge = 0;

	pLine->nRightEdges = 0;
	pLine->rightEdge = 0;
}


void LinePrint(SLine* pLine)
{
    printf("in{");
    for(int i = 0; i < POINTS; i++)
    {
        printf("%3d ", pLine->data[i]);
    }
    printf("}");

    printf("left[%d]{%f} ", pLine->nLeftEdges, pLine->leftEdge);
    printf("right[%d]{%f} ", pLine->nRightEdges, pLine->rightEdge);
}

// Compute w3 point weighted average around pos
float WeightedAverage(int8_t* derivative, int i, int n)
{
	float a = i > 0 ? derivative[i - 1] : 0;
	float b = derivative[i];
	float c = i < n - 1 ? derivative[i + 1] : 0;

	float sum = a+b+c;
	float w = (a*(i-1) + b*i + c*(i+1))/sum;
	return w;
}


void SearchLeftEdges(SLine* pLine)
{
	// Search for left edges
	int8_t derivative[POINTS + 1];
	for(int i = 0; i < POINTS + 1; i++)
	{
		int8_t diff;
		if(i > 0)
		{
			diff = pLine->data[i] - pLine->data[i - 1];
		}
		else
		{
			diff = pLine->data[i];
		}

		derivative[i] = 0;
		if(diff > 0)
		{
			derivative[i] = diff;
		}
	}

//	printf(" dp:{"); for(int i = 0; i < POINTS + 1; i++) { printf("%3d ", (int)derivative[i]); } printf("} ");

	// Search for positive peaks
	float max = 0;
	int maxpos = 0;
	for(int i = 0; i < 9; i++)
	{
		if(derivative[i] > max)
		{
			max = derivative[i];
			maxpos = i;
		}
	}

	if(max > LINE_EDGE_THRESHOLD)
	{
		pLine->nLeftEdges = 1;
		pLine->leftEdge = WeightedAverage(derivative, maxpos, POINTS + 1);
	}
}


void SearchRightEdges(SLine* pLine)
{
	// Search for right edges
	int8_t derivative[POINTS + 1];
	for(int i = 0; i < POINTS + 1; i++)
	{
		int8_t diff;
		if(i > 0)
		{
			diff = pLine->data[i] - pLine->data[i - 1];
		}
		else
		{
			diff = pLine->data[i];
		}

		derivative[i] = 0;
		if(diff < 0)
		{
			derivative[i] = -diff;
		}
	}

//	printf(" dp:{"); for(int i = 0; i < POINTS + 1; i++) { printf("%3d ", (int)derivative[i]); } printf("} ");

	// Search for positive peaks
	float max = 0;
	int maxpos = 0;
	for(int i = 0; i < 9; i++)
	{
		if(derivative[i] > max)
		{
			max = derivative[i];
			maxpos = i;
		}
	}

	if(max > LINE_EDGE_THRESHOLD)
	{
		pLine->nRightEdges = 1;
		pLine->rightEdge = WeightedAverage(derivative, maxpos, POINTS + 1);
	}
}



void LineAnalyze(SLine* pLine)
{
    pLine->n = 0;
    pLine->p0 = 0.0f;
	pLine->nLeftEdges = 0;
	pLine->leftEdge = 0;

	pLine->nRightEdges = 0;
	pLine->rightEdge = 0;

	// Compute weighted average
    int count = 0;
    float p = 0.0;
    float sum = 0.0;
    for(int i = 0; i < POINTS; i++)
    {
        if(pLine->data[i] > LINE_CENTER_THRESHOLD)
        {
            count = 1;
            sum += pLine->data[i];
            p += pLine->data[i]*i;
        }
    }
    
    p = p/sum;
    if(count > 0)
    {
        pLine->p0 = p;
    }
    pLine->n = count;

	SearchLeftEdges(pLine);
	SearchRightEdges(pLine);
}
