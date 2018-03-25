#include "line.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>


void LineInit(SLine* pLine)
{
   	pLine->nCenter = 0;
    pLine->center = 0.0f;
	pLine->nLeft = 0;
	pLine->left = 0;
	pLine->nRight = 0;
	pLine->right = 0;
    pLine->full = 0;
}


void LinePrint(SLine* pLine)
{

    printf("in{");
    for(int i = 0; i < POINTS; i++)
    {
        printf("%3d ", pLine->data[i]);
    }
    printf("}");

    printf("l%d:%2.3f c%d:%2.3f r%d:%2.3f odo:%g full:%d", pLine->nLeft, pLine->left, pLine->nCenter, pLine->center, pLine->nRight, pLine->right, pLine->odometer, pLine->full);
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

    if(pLine->nLeft > 0)
    {
        int index = pLine->left*POINTS*5 + POINTS*5;
        if(index >= 0 && index < 3*POINTS*5 + 2)
        {
            view[index] = 'L';
        }
    }

    if(pLine->nRight > 0)
    {
        int index = pLine->right*POINTS*5 + POINTS*5;
        if(index >= 0 && index < 3*POINTS*5 + 2)
        {
            view[index] = 'R';
        }
    }

    if(pLine->nCenter > 0)
    {
        int index = pLine->center*POINTS*5 + POINTS*5;
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


void SearchLeftEdges2(SLine* pLine, float threshold)
{
	// Search for crossings

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

				pLine->nLeft = 1;
				pLine->left = (x + i)/(POINTS-1);
				break;
			}
		}
	}
}


void SearchRightEdges2(SLine* pLine, float threshold)
{
	// Search for crossings

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

				pLine->nRight = 1;
				pLine->right = (i - x)/(POINTS-1);
				break;
			}
		}
	}
}


void SearchCenter(SLine* pLine, float threshold)
{
	// Compute weighted average
    int count = 0;
    float p = 0.0;
    float sum = 0.0;
    for(int i = 0; i < POINTS; i++)
    {
		float val = (float)pLine->norm[i];
        if(val >= threshold)
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
        pLine->center = p/(POINTS-1);
    }
    pLine->nCenter = count;
}


void LineAnalyze(SLine* pLine, float black, float white, float threshold)
{
	// Normalize values to [black, white]
    for(int i = 0; i < POINTS; i++)
    {
		float val = ((float)pLine->data[i] - black)/(white - black);
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

    // if 7 or 8 are above threshold we consider the line full
    int nAbove = 0;
    for(int i = 0; i < POINTS; i++)
    {
        if(pLine->norm[i] > threshold)
        {
            nAbove++;
        }
    }
    
    if(nAbove >= POINTS - 1)
    {
        pLine->full = 1;
    }
    else
    {
        pLine->full = 0;
    }
    
    pLine->nCenter = 0;
    pLine->center = 0.0f;
	pLine->nLeft = 0;
	pLine->left = 0;
	pLine->nRight = 0;
	pLine->right = 0;

    SearchCenter(pLine, threshold);
	SearchLeftEdges2(pLine,threshold);
	SearchRightEdges2(pLine, threshold);

	if(pLine->nLeft && !pLine->nRight)
	{
		pLine->right = pLine->left + LINE_WIDTH;
		pLine->nRight = 1;
	}
	else if(!pLine->nLeft && pLine->nRight)
	{
		pLine->left = pLine->right - LINE_WIDTH;
		pLine->nLeft = 1;
	}
}


void LineSensorInit(SLineSensor* pLineSensor)
{
    pLineSensor->first = 0;
    pLineSensor->last = 0;
    pLineSensor->count = 0;
}

bool LineSensorJunctionLeft(SLineSensor* pLineSensor)
{
    // A junction requires the newest line is full and there is a previous left line
    int n = pLineSensor->count;
    if(pLineSensor->lines[0].full == 0)
    {
        return false;
    }
    
    for(int i = 0; i < n; i++)
    {
        SLine* pLine = &pLineSensor->lines[i];
        if(pLine->nLeft > 0)
        {
            return true;
        }
    }
    return false;
}

bool LineSensorJunctionRight(SLineSensor* pLineSensor)
{
    // A junction requires the newest line is full and there is a previous left line
    int n = pLineSensor->count;
    if(pLineSensor->lines[0].full == 0)
    {
        return false;
    }
    
    for(int i = 0; i < n; i++)
    {
        SLine* pLine = &pLineSensor->lines[i];
        if(pLine->nRight > 0)
        {
            return true;
        }
    }
    return false;
}

void LineSensorPush(SLineSensor* pLineSensor, SLine* pLine)
{
    if(pLineSensor->count > 0)
    {
        int n = pLineSensor->count;
        if(n == LINE_SENSOR_DEPTH)
        {
            n = LINE_SENSOR_DEPTH-1;
        }
        // move old records
        for(int i = n - 1; i >= 0; i--)
        {
            pLineSensor->lines[i + 1] = pLineSensor->lines[i];
        }
    }
    
    if(pLineSensor->count < LINE_SENSOR_DEPTH)
    {
        pLineSensor->count++;
    }
    pLineSensor->lines[0] = *pLine;
}

bool LineSensorLeft(SLineSensor* pLineSensor, float* pPos)
{
    // Search for a record in history with a left edge and return its value
    int n = pLineSensor->count;
    for(int i = 0; i < n; i++)
    {
        SLine* pLine = &pLineSensor->lines[i];
        if(pLine->nLeft > 0)
        {
            *pPos = pLine->left;
            return true;
        }
    }
    return false;
}

bool LineSensorRight(SLineSensor* pLineSensor, float* pPos)
{
    // Search for a record in history with a left edge and return its value
    int n = pLineSensor->count;
    for(int i = 0; i < n; i++)
    {
        SLine* pLine = &pLineSensor->lines[i];
        if(pLine->nRight > 0)
        {
            *pPos = pLine->right;
            return true;
        }
    }
    return false;
}

bool LineSensorCenter(SLineSensor* pLineSensor, float* pPos)
{
    // Search for a record in history with a left edge and return its value
    int n = pLineSensor->count;
    for(int i = 0; i < n; i++)
    {
        SLine* pLine = &pLineSensor->lines[i];
        if(pLine->nCenter > 0)
        {
            *pPos = pLine->center;
            return true;
        }
    }
    return false;
}

bool LineSensorLineFull(SLineSensor* pLineSensor)
{
    if(pLineSensor->lines[0].full == 1)
    {
        return true;
    }
    return false;
}

bool LineSensorNoLine(SLineSensor* pLineSensor)
{
    // Check if there is no lines back in history
    int n = pLineSensor->count;
    for(int i = 0; i < n; i++)
    {
        SLine* pLine = &pLineSensor->lines[i];
        if((pLine->nLeft > 0) || (pLine->nRight > 0 || pLine->nCenter))
        {
            return false;
        }
    }
    return true;
}

bool LineSensorAnyLine(SLineSensor* pLineSensor)
{
    // Check if there is any lines back in history
    int n = pLineSensor->count;
    for(int i = 0; i < n; i++)
    {
        SLine* pLine = &pLineSensor->lines[i];
        if((pLine->nLeft > 0) || (pLine->nRight > 0 || pLine->nCenter))
        {
            return true;
        }
    }
    return false;
}

void LineSensorPrint(SLineSensor* pLineSensor)
{
    int n = pLineSensor->count;
    printf("linedata count:%d\n", n);
    
    for(int i = 0; i < n; i++)
    {
        printf("%d: ", i);
        LinePrint(&pLineSensor->lines[i]);
        printf("\n");
    }
}


// 01234


