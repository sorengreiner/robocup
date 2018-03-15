#include "robocup.h"
#include "magnus.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>

//-----------------------------------------------------------------------------
// Main entry
//-----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	if(argc > 1)
	{
		// Load script
		FILE* file = fopen(argv[1], "rb");
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
            PrintProgram(&program);
//            RunProgram(&program);
            
            DeleteProgram(&program);
        }
		free(p);
		fclose(file);
	}
    return 0;
}
