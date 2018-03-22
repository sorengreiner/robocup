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
/*
    printf("norm{");
    for(int i = 0; i < POINTS; i++)
    {
        printf("%4.2f ", pLine->norm[i]);
    }
    printf("}");
*/
    printf("l%d:%2.3f c:%2.3f r%d:%2.3f", pLine->nLeftEdges, pLine->leftEdge, pLine->p0, pLine->nRightEdges, pLine->rightEdge);
}

void LineDataPrint(SLine* pLine)
{
    for(int i = 0; i < POINTS; i++)
    {
        printf("%3d  ", pLine->data[i]);
    }
    printf(" ");
    
    char view[3*POINTS*5 + 2];
    view[3*POINTS*5 + 1] = 0;
    memset(view, '.', sizeof(view) - 1);
    memset(view + POINTS*5, '-', POINTS*5 - 1);

    if(pLine->nLeftEdges > 0)
    {
        int index = pLine->leftEdge*POINTS*5 + POINTS*5;
        if(index >= 0 && index < 3*POINTS*5 + 2)
        {
            view[index] = 'L';
        }
    }

    if(pLine->nRightEdges > 0)
    {
        int index = pLine->rightEdge*POINTS*5 + POINTS*5;
        if(index >= 0 && index < 3*POINTS*5 + 2)
        {
            view[index] = 'R';
        }
    }

    if(pLine->n > 0)
    {
        int index = pLine->p0*POINTS*5 + POINTS*5;
        if(index >= 0 && index < 3*POINTS*5 + 2)
        {
            view[index] = 'C';
        }
    }
    
    printf("%s\n", view);
    
}



// Compute w3 point weighted average around pos
float WeightedAverage(float* derivative, int i, int n)
{
	float a = i > 0 ? derivative[i - 1] : 0;
	float b = derivative[i];
	float c = i < n - 1 ? derivative[i + 1] : 0;

	float sum = a+b+c;
	float w = (a*(i-1) + b*i + c*(i+1))/sum;
	return w;
}

void SearchLeftEdges2(SLine* pLine)
{
	// Search for crossings
	float threshold = 0.66;

	if(pLine->norm[0] < threshold)
	{
		for(int i = 0; i < POINTS - 1; i++)
		{
			float y0 = pLine->norm[i];
			float y1 = pLine->norm[i + 1];
	
			if((y0 < threshold) && (y1 >= threshold))
			{
				// crossing found. Compute the weighted x pos
				float x = (threshold-y0)/(y1-y0);

				pLine->nLeftEdges = 1;
				pLine->leftEdge = (x + i)/(POINTS-1);
				break;
			}
		}
	}
}


void SearchRightEdges2(SLine* pLine)
{
	// Search for crossings
	float threshold = 0.66;

	if(pLine->norm[POINTS - 1] < threshold)
	{
		for(int i = POINTS - 1; i > 0; i--)
		{
			float y0 = pLine->norm[i];
			float y1 = pLine->norm[i - 1];
	
			if((y0 < threshold) && (y1 >= threshold))
			{
				// crossing found. Compute the weighted x pos
				float x = (threshold-y0)/(y1-y0);

				pLine->nRightEdges = 1;
				pLine->rightEdge = (i - x)/(POINTS-1);
				break;
			}
		}
	}
}




void SearchLeftEdges(SLine* pLine)
{
	// Search for left edges
	float derivative[POINTS + 1];
	for(int i = 0; i < POINTS + 1; i++)
	{
		float diff;
		if(i > 0)
		{
			diff = pLine->norm[i] - pLine->norm[i - 1];
		}
		else
		{
			diff = pLine->norm[i];
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
	float derivative[POINTS + 1];
	for(int i = 0; i < POINTS + 1; i++)
	{
		float diff;
		if(i > 0)
		{
			diff = pLine->norm[i] - pLine->norm[i - 1];
		}
		else
		{
			diff = pLine->norm[i];
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



void LineAnalyze(SLine* pLine, float base, float high)
{
	// Normalize values to [base, high]
    for(int i = 0; i < POINTS; i++)
    {
		float val = ((float)pLine->data[i] - base)/(high - base);
		if(val < 0)
		{
			val = 0;
		}
		else if(val > 1)
		{
			val = 1;
		}
		pLine->norm[i] = val;
	}

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
		float val = (float)pLine->norm[i];
        if(val >= 0)
        {
            count = 1;
            sum += val;
            p += val*i;
        }
    }
    
	if(sum > 0.0001)
	{
    	p = p/sum;
	}
    if(count > 0)
    {
        pLine->p0 = p/(POINTS-1);
    }
    pLine->n = count;

	SearchLeftEdges2(pLine);
	SearchRightEdges2(pLine);

	if(pLine->nLeftEdges && !pLine->nRightEdges)
	{
		pLine->rightEdge = pLine->leftEdge + LINE_WIDTH;
		pLine->nRightEdges = 1;
	}
	else if(!pLine->nLeftEdges && pLine->nRightEdges)
	{
		pLine->leftEdge = pLine->rightEdge - LINE_WIDTH;
		pLine->nLeftEdges = 1;
	}
}
