#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "line.h"

uint8_t g_data[] =
{
42,45,45,44,33,17,4,2,
42,46,45,45,33,19,7,2,
42,46,45,45,33,19,7,2,
42,46,45,45,33,19,7,2,
42,46,45,45,33,19,7,2,
40,44,46,48,38,26,12,6,
40,44,46,48,38,26,12,6,
40,44,46,48,38,26,12,6,
31,42,46,49,44,34,20,9,
31,42,46,49,44,34,20,9,
31,42,46,49,44,34,20,9,
24,37,43,49,47,41,31,21,
24,37,43,49,47,41,31,21,
24,37,43,49,47,41,31,21,
24,37,43,49,47,41,31,21,
14,33,42,47,47,43,36,27,
14,33,42,47,47,43,36,27,
14,33,42,47,47,43,36,27,
11,30,42,47,47,45,39,32,
11,30,42,47,47,45,39,32,
11,30,42,47,47,45,39,32,
7,25,38,47,48,46,42,39,
7,25,38,47,48,46,42,39,
7,25,38,47,48,46,42,39,
7,25,38,47,48,46,42,39,
7,25,38,47,48,46,42,39,
7,25,38,47,48,46,42,39,
7,25,38,47,48,46,42,39,
0,13,28,38,46,46,46,49,
0,13,28,38,46,46,46,49,
0,13,28,38,46,46,46,49,
0,11,25,37,46,46,47,49,
0,11,25,37,46,46,47,49,
0,11,25,37,46,46,47,49,
0,8,22,35,44,45,47,49,
0,8,22,35,44,45,47,49,
0,8,22,35,44,45,47,49,
0,8,22,35,44,45,47,49,
0,5,18,33,42,45,45,51,
0,5,18,33,42,45,45,51,
0,5,18,33,42,45,45,51,
0,0,14,25,38,42,47,53,
0,0,14,25,38,42,47,53,
0,0,14,25,38,42,47,53,
0,0,5,17,33,40,44,51,
0,0,5,17,33,40,44,51,
0,0,5,17,33,40,44,51,
0,0,5,17,33,40,44,51,
0,0,0,11,24,34,42,50,
0,0,0,11,24,34,42,50,
0,0,0,11,24,34,42,50,
0,0,0,5,20,29,41,50,
0,0,0,5,20,29,41,50,
0,0,0,5,20,29,41,50,
0,0,0,0,13,24,36,46,
0,0,0,0,13,24,36,46,
0,0,0,0,13,24,36,46,
0,0,0,0,13,24,36,46,
0,0,0,0,7,16,30,44,
0,0,0,0,7,16,30,44,
0,0,0,0,7,16,30,44,
0,0,0,0,3,12,27,40,
0,0,0,0,3,12,27,40,
0,0,0,0,3,12,27,40,
0,0,0,0,2,10,25,38,
0,0,0,0,2,10,25,38,
0,0,0,0,2,10,25,38,
0,0,0,0,2,10,25,38,
0,0,0,0,1,7,19,37,
0,0,0,0,1,7,19,37,
0,0,0,0,1,7,19,37,
0,0,0,0,0,2,13,31,
0,0,0,0,0,2,13,31,
0,0,0,0,0,2,13,31,
0,0,0,0,0,0,9,24,
0,0,0,0,0,0,9,24,
0,0,0,0,0,0,9,24,
0,0,0,0,0,0,9,24,
0,0,0,0,0,0,7,24,
0,0,0,0,0,0,7,24,
0,0,0,0,0,0,7,24,
0,0,0,0,0,0,4,20,
0,0,0,0,0,0,4,20,
0,0,0,0,0,0,4,20,
0,0,0,0,0,0,1,14,
0,0,0,0,0,0,1,14,
0,0,0,0,0,0,1,14,
0,0,0,0,0,0,1,14,
0,0,0,0,0,0,0,9,
0,0,0,0,0,0,0,9,
0,0,0,0,0,0,0,9,
0,0,0,0,0,0,0,4,
0,0,0,0,0,0,0,4,
0,0,0,0,0,0,0,4,
0,0,0,0,0,0,0,2,
0,0,0,0,0,0,0,2,
0,0,0,0,0,0,0,2,
0,0,0,0,0,0,0,2,
0,0,0,0,0,0,0,2,
0,0,0,0,0,0,0,2,
0,0,0,0,0,0,0,2,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,2,
0,0,0,0,0,0,0,2,
0,0,0,0,0,0,0,2,
0,0,0,0,0,0,0,2,
0,0,0,0,0,0,0,4,
0,0,0,0,0,0,0,4,
0,0,0,0,0,0,0,4,
0,0,0,0,0,0,0,6,
0,0,0,0,0,0,0,6,
0,0,0,0,0,0,0,6,
0,0,0,0,0,0,0,9,
0,0,0,0,0,0,0,9,
0,0,0,0,0,0,0,9,
0,0,0,0,0,0,0,9,
0,0,0,0,0,0,1,13,
0,0,0,0,0,0,1,13,
0,0,0,0,0,0,1,13,
0,0,0,0,0,0,4,18,
0,0,0,0,0,0,4,18,
0,0,0,0,0,0,4,18,
0,0,0,0,0,0,7,20,
0,0,0,0,0,0,7,20,
0,0,0,0,0,0,7,20,
0,0,0,0,0,0,7,20,
0,0,0,0,0,0,9,27,
0,0,0,0,0,0,9,27,
0,0,0,0,0,0,9,27,
0,0,0,0,0,2,13,32,
0,0,0,0,0,2,13,32,
0,0,0,0,0,2,13,32,
0,0,0,0,0,4,18,33,
0,0,0,0,0,4,18,33,
0,0,0,0,0,4,18,33,
0,0,0,0,0,4,18,33,
0,0,0,0,1,7,22,38,
0,0,0,0,1,7,22,38,
0,0,0,0,1,7,22,38,
0,0,0,0,2,9,27,40,
0,0,0,0,2,9,27,40,
0,0,0,0,2,9,27,40,
0,0,0,0,4,13,30,43,
0,0,0,0,4,13,30,43,
0,0,0,0,4,13,30,43,
0,0,0,0,4,13,30,43,
0,0,0,0,7,17,31,45,
0,0,0,0,7,17,31,45,
0,0,0,0,7,17,31,45,
0,0,0,0,10,19,33,45,
0,0,0,0,10,19,33,45,
0,0,0,0,10,19,33,45,
0,0,0,1,13,24,36,48,
0,0,0,1,13,24,36,48,
0,0,0,1,13,24,36,48,
0,0,0,1,13,24,36,48,
0,0,0,2,17,28,39,48,
0,0,0,2,17,28,39,48,
0,0,0,2,17,28,39,48,
0,0,0,5,18,31,40,49,
0,0,0,5,18,31,40,49,
0,0,0,5,18,31,40,49,
0,0,0,8,22,34,42,49,
0,0,0,8,22,34,42,49,
0,0,0,8,22,34,42,49,
0,0,0,8,22,34,42,49,
0,0,0,11,27,36,43,49,
0,0,0,11,27,36,43,49,
0,0,0,11,27,36,43,49,
0,0,5,16,33,40,44,51,
0,0,5,16,33,40,44,51,
0,0,5,16,33,40,44,51,
0,0,8,17,35,40,45,50,
0,0,8,17,35,40,45,50,
0,0,8,17,35,40,45,50,
0,0,8,17,35,40,45,50,
0,0,8,19,36,42,44,51,
0,0,8,19,36,42,44,51,
0,0,8,19,36,42,44,51,
0,0,14,25,38,43,45,50,
0,0,14,25,38,43,45,50,
0,0,14,25,38,43,45,50,
0,2,16,31,41,44,47,50,
0,2,16,31,41,44,47,50,
0,2,16,31,41,44,47,50,
0,2,16,31,41,44,47,50,
0,7,20,33,44,45,46,49,
0,7,20,33,44,45,46,49,
0,7,20,33,44,45,46,49,
0,11,25,36,46,46,46,48,
0,11,25,36,46,46,46,48,
0,11,25,36,46,46,46,48,
0,13,30,39,48,45,45,45,
0,13,30,39,48,45,45,45,
0,13,30,39,48,45,45,45,
0,13,30,39,48,45,45,45,
1,17,34,43,48,45,44,44,
1,17,34,43,48,45,44,44,
1,17,34,43,48,45,44,44,
4,23,36,46,49,46,42,38,
4,23,36,46,49,46,42,38,
4,23,36,46,49,46,42,38,
8,28,40,48,48,44,40,34,
8,28,40,48,48,44,40,34,
8,28,40,48,48,44,40,34,
8,28,40,48,48,44,40,34,
11,30,42,47,48,45,38,31,
11,30,42,47,48,45,38,31,
11,30,42,47,48,45,38,31,
14,34,42,49,49,44,36,28,
14,34,42,49,49,44,36,28,
14,34,42,49,49,44,36,28,
17,35,44,49,49,44,34,27,
17,35,44,49,49,44,34,27,
17,35,44,49,49,44,34,27,
17,35,44,49,49,44,34,27,
21,37,43,49,46,40,29,20,
46,40,29,20,4,21,37,43,
28,30,32,36,39,40,47,54,
30,31,33,36,39,40,47,53,
30,31,33,36,39,40,47,53,
30,31,33,36,39,40,47,53,
30,31,33,36,39,40,47,53,
30,31,33,36,39,40,47,53,
30,31,33,36,39,40,47,53,
30,31,33,36,39,40,47,53,
30,31,33,36,39,40,47,53,
30,31,33,36,39,40,47,53,
30,30,32,35,38,39,46,53,
30,30,32,35,38,39,46,53,
30,30,32,35,38,39,46,53,
30,30,32,35,38,39,46,53,
30,30,32,35,38,39,46,53,
30,30,32,35,38,39,46,53,
30,30,32,35,38,39,46,53,
30,30,32,35,38,39,46,53,
30,30,32,35,38,39,46,53,
27,28,31,34,38,39,47,54,
27,28,31,34,38,39,47,54,
27,28,31,34,38,39,47,54,
27,28,31,34,38,39,47,54,
27,28,31,34,38,39,47,54,
27,28,31,34,38,39,47,54,
27,28,31,34,38,39,47,54,
27,28,31,34,38,39,47,54,
27,28,31,34,38,39,47,54,
27,29,31,34,38,40,48,53,
31,34,36,41,46,45,51,60,
32,34,36,40,45,44,51,56
};


#define LINESENSOR_WIDTH_MM (46.0)

float LinePosToPhysical(float pos)
{
	return (LINESENSOR_WIDTH_MM*pos - LINESENSOR_WIDTH_MM/2);
}


void LineDataPrint(SLine* pLine)
{
    printf("");
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

/*
    uint8_t data[POINTS];
    float norm[POINTS];
    uint8_t n;
    float p0;
	uint8_t nLeftEdges;
	float leftEdge;
*/


int main(int argc, char* argv[])
{
	if(argc == 2)
	{
		FILE* file = fopen(argv[1], "r");
		if(!file)
		{
			printf("File not found\n");
		}
	    char line[256];
	
	    while (fgets(line, sizeof(line), file)) 
		{
			int data[8];
			if(sscanf(line, "%d %d %d %d %d %d %d %d", data,data+1,data+2,data+3,data+4,data+5,data+6,data+7) == 8)
			{
		        SLine line;
				for(int i = 0; i < 8; i++)
				{
					line.data[i] = data[i];
				}
		        
		        LineAnalyze(&line, 30, 50);
//		        LinePrint(&line);
                LineDataPrint(&line);
			}
	    }
	
	    fclose(file);
	}
	else
	{
	    uint32_t nLines = sizeof(g_data)/8;
	    for(uint32_t i = 0; i < nLines; i++)
	    {
	        SLine line;
	        memcpy(line.data, g_data + 8*i, 8);
	        
	        LineAnalyze(&line, 30, 50);
	        LinePrint(&line);
			printf("");
			printf("\n");
	    }
    }
	
    return 0;
}
