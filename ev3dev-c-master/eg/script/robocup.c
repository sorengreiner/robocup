#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    AND = 0,
    OR,
    XOR
} EOperator;

typedef enum
{
    NIL = 0,
    SPEED,
    ODOMETER,
    VALUE,
} ENoun;

const char* g_NounName[] =
{
    "NIL",
    "SPEED",
    "ODOMETER",
    "VALUE"
};

void PrintNoun(int eNoun)
{
    if(eNoun < 4)
    {
        printf("%s", g_NounName[eNoun]);
    }
}



typedef struct
{
    bool (*pAction)(int noun0, float value0, int noun1, float value1); 
    int noun0; float value0; int noun1; float value1;
    
    bool (*pConditionA)(int noun0, float value0, int noun1, float value1); 
    int left_noun0; float left_value0; int left_noun1; float left_value1;
    
    EOperator eOperator;
    
    bool (*pConditionB)(int noun0, float value0, int noun1, float value1); 
    int right_noun0; float right_value0; int right_noun1; float right_value1;
} SSequence;

// Condition functions

bool False(int noun0, float value0, int noun1, float value1)
{ return false; }

bool True(int noun0, float value0, int noun1, float value1)
{ return false; }

bool Greater(int noun0, float value0, int noun1, float value1)
{
    printf("%f Greater %f", value0, value1);
        
    return value0 > value1;
}

bool DetectJunctionLeft(int noun0, float value0, int noun1, float value1)
{ 
    printf("DetectJunctionLeft ");
    return true; 
}

bool DetectBranchLeft(int noun0, float value0, int noun1, float value1)
{ 
    printf("DetectBranchLeft ");
    return true; 
}

bool Odometer(int noun0, float value0, int noun1, float value1)
{ 
    printf("Odometer ");
    return false; 
}


// Action functions

bool FollowLine(int noun0, float value0, int noun1, float value1)
{
    printf("FollowLine ");
    PrintNoun(noun0);
    printf("=%f ", value0);
    PrintNoun(noun1);
    printf("=%f ", value1);
    return true;
}

bool FollowLineLeft(int noun0, float value0, int noun1, float value1)
{
    printf("FollowLineLeft ");
    return true;
}

bool SetValue(int noun0, float value0, int noun1, float value1)
{
    printf("SetValue ");
    return true;
}



// Action( noun, value, noun, value) until Condition(noun, value, noun, value) AND/OR Condition(noun, value, noun value)


SSequence g_SequenceSimple[] =
{
    {FollowLine,        SPEED,      80, NIL, 0.0,     DetectJunctionLeft, NIL,  0.0, NIL,  0.0,    OR,           False, NIL, 0.0, NIL, 0.0}, // FollowLine SPEED=80 until DetectJunctionLeft()
    {FollowLine,        SPEED,      20, NIL, 0.0,     DetectBranchLeft,   NIL,  0.0, NIL,  0.0,    OR,           False, NIL, 0.0, NIL, 0.0}, // FollowLine SPEED=20 until DetectBranchLeft()
    {SetValue,          ODOMETER,   0,  NIL, 0.0,     0,                  NIL,  0.0, NIL,  0.0,    OR,           False, NIL, 0.0, NIL, 0.0}, // SetValue ODOMETER=0
    {FollowLineLeft,    SPEED,      10, NIL, 0.0,     Greater,       ODOMETER,  0.0, VALUE, 3.0,   OR,           False, NIL, 0.0, NIL, 0.0}, // FollowLineLeft SPEED=10 until ODOMETER > 3.0
};


int index = 0;

bool UpdateProgram()
{
    printf("line: %d ", index);
    const SSequence* pSequence = &g_SequenceSimple[index];
    
    if(pSequence->pAction)
    {
        if(!pSequence->pAction(pSequence->noun0, pSequence->value0, pSequence->noun1, pSequence->value1))
        {
            printf("\n");
            return false;
        }

        printf("until ");
        
        bool bEvalA = true;
        bool bEvalB = true;
        if(pSequence->pConditionA)
        {
            bEvalA = pSequence->pConditionA(pSequence->left_noun0, pSequence->left_value0, pSequence->left_noun1, pSequence->left_value1);
        }
        
        if(bEvalA)
        {
            index++;
            if(index > sizeof(g_SequenceSimple)/sizeof(g_SequenceSimple[0]))
            {
                return false;
            }
        }
    }
    printf("\n");
    return true;
}

typedef struct
{
	SSequence* pSequence;
	int sequenceCount;
} SProgram;


/*

follow speed=80 until junctionleft
follow speed=20 until branchleft
set odometer=0
followleft until odometer > 3.0
follow speed=30 until junctionright
turnright angle=90
forward speed=10 until line
set odometer=0
backward speed=10 until odometer > 0.25
turnright angle=90
*/

typedef enum
{
	K_NA = 0,
	K_BRANCHLEFT,
	K_BRANCHRIGHT,
	K_JUNCTIONLEFT,
	K_JUNCTIONRIGHT,
	K_LINE,
    K_BACKWARD,
    K_FOLLOW,
    K_FOLLOWLEFT,
    K_FOLLOWRIGHT,
    K_FORWARD,
    K_SET,
    K_TURNLEFT,
    K_TURNRIGHT,
    K_UNTIL,
	K_CONST,
	K_EQUAL,
	K_GREATER,
	K_GREATER_EQUAL,
	K_LESS,
	K_LESS_EQUAL,
	K_END,
	NUM_KEYWORDS
} EKeyword;


const char* g_Keywords[] =
{
	"NA",
	"BRANCHLEFT",
	"BRANCHRIGHT",
	"JUNCTIONLEFT",
	"JUNCTIONRIGHT",
	"LINE",
    "BACKWARD",
    "FOLLOW",
    "FOLLOWLEFT",
    "FOLLOWRIGHT",
    "FORWARD",
    "SET",
    "TURNLEFT",
    "TURNRIGHT",
    "UNTIL",
	"CONST",
	"=",
	">",
	">=",
	"<",
	"<=",
	"#"
};


EKeyword MatchKeyword(const char* token)
{
	for(int i = 0; i < NUM_KEYWORDS; i++)
	{
		if(strcasecmp(token, g_Keywords[i]) == 0)
		{
			return (EKeyword)i;
		}
	}
	return K_NA;
}


typedef enum
{
	V_NA = 0,
	V_SPEED,
	V_ODOMETER,
	V_ANGLE,
	NUM_VARS
} EVar;

const char* g_Vars[] =
{
	"NA",
	"SPEED",
	"ODOMETER",
	"ANGLE",
};

EVar MatchKeyValue(const char* token, float* value)
{
	char* pe = strchr(token, '=');
	char* pv = 0;
	if(pe != 0)
	{
		*pe = 0;
		pv = pe + 1;
	}

	if(pv)
	{
		if(sscanf(pv, "%f", value) == 1)
		{
//			printf("value %f\n", *value);
		}
	}

	for(int i = 0; i < NUM_VARS; i++)
	{
		if(strcasecmp(token, g_Vars[i]) == 0)
		{
			return (EVar)i;
		}
	}
	return V_NA;
}

EVar MatchVar(const char* token)
{
	for(int i = 0; i < NUM_VARS; i++)
	{
		if(strcasecmp(token, g_Vars[i]) == 0)
		{
			return (EVar)i;
		}
	}
	return V_NA;
}



typedef enum
{
	S_ACTION = 0,
	S_KEYVALUE0,
	S_KEYVALUE1,
	S_UNTIL,
	S_CONDITIONA,
	S_COMPARISONA,
	S_KEYVALUE2,
	S_KEYVALUE3,
	S_RIGHTA,
	S_CONDITIONB,
	S_COMPARISONB,
	S_KEYVALUE4,
	S_KEYVALUE5,
	S_RIGHTB,
	S_END,
} ESyntax;


bool Action(EKeyword eKeyword)
{
	switch(eKeyword)
	{
	case K_BACKWARD:
    case K_FOLLOW:
    case K_FOLLOWLEFT:
    case K_FOLLOWRIGHT:
    case K_FORWARD:
    case K_SET:
    case K_TURNLEFT:
    case K_TURNRIGHT:
		return true;
	default:
		return false;
	}
}

bool Comparison(EKeyword eKeyword)
{
	switch(eKeyword)
	{
	case K_EQUAL:
	case K_GREATER:
	case K_GREATER_EQUAL:
	case K_LESS:
	case K_LESS_EQUAL:
		return true;
	default:
		return false;
	}

}

bool Condition(EKeyword eKeyword)
{
	switch(eKeyword)
	{
	case K_BRANCHLEFT:
	case K_BRANCHRIGHT:
	case K_JUNCTIONLEFT:
	case K_JUNCTIONRIGHT:
	case K_LINE:
		return true;
	default:
		return false;
	}

}


int ParseLine(char* in)
{
	ESyntax eSyntax = S_ACTION;
	char* p = in;
	// for each line parse the program
	while(*p != 0)
	{
		char buffer[256];
		char* ps = p;
		while((*p != ' ') && (*p != 0))
		{
			p++;
		}

		char* pe = p + 1;
		while(*p == ' ')
		{
			p++;
		}

		int len = pe - ps - 1;
		if(len > 0)
		{
			memcpy(buffer, ps, len);
			buffer[len] = 0;
//			printf("  token %d: \"%s\"\n", len, buffer);
	
			// Search keywords
			EKeyword eKeyword = MatchKeyword(buffer);
			float value = 0;
			EVar eKeyValue = MatchKeyValue(buffer, &value);
			EVar eVar = MatchVar(buffer);

			switch(eSyntax)
			{
			case S_ACTION:
				if(Action(eKeyword))
				{
					eSyntax = S_KEYVALUE0;
					printf("  ACTION=%s\n", g_Keywords[eKeyword]);
				}
				else
				{
					printf("syntax error: expected an action got %s\n", buffer);
					return 0;
				}
				break;
			case S_KEYVALUE0:
				if(eKeyword == K_UNTIL)
				{
					printf("  UNTIL\n");
					eSyntax = S_CONDITIONA;
				}
				else if(eKeyValue != V_NA)
				{
					printf("  KV(%s,%f)\n", g_Vars[eKeyValue], value);
					eSyntax = S_KEYVALUE1;
				}
				else
				{
					eSyntax = S_END;
				}
				break;
			case S_KEYVALUE1:
				if(eKeyword == K_UNTIL)
				{
					eSyntax = S_CONDITIONA;
				}
				else if(eKeyValue != V_NA)
				{
					printf("  KV(%s,%f)\n", g_Vars[eKeyValue], value);
					eSyntax = S_UNTIL;
				}
				break;
			case S_UNTIL:
				if(eKeyword == K_UNTIL)
				{
					printf("  UNTIL\n");
					eSyntax = S_CONDITIONA;
				}
				else if(eKeyword == K_END)
				{
					return 0;
				}
				else
				{
					printf("syntax error: expected until\n");
					return 0;
				}
				break;
			case S_CONDITIONA:
				if(Condition(eKeyword))
				{
					eSyntax = S_KEYVALUE2;
					printf("  CONDA=%s\n", g_Keywords[eKeyword]);
				}
				else if(eVar != V_NA)
				{
					eSyntax = S_COMPARISONA;
					printf("  V(%s)\n", g_Vars[eVar]);
				}
				else if(sscanf(buffer, "%f", &value) == 1)
				{
					eSyntax = S_COMPARISONA;
					printf("  F(%f)\n", value);
				}
				else
				{
					printf("syntax error: expected a condition\n");
					return 0;
				}
				break;
			case S_KEYVALUE2:
				if(eVar != V_NA)
				{
					eSyntax = S_KEYVALUE3;
					printf("  V(%s)\n", g_Vars[eVar]);
				}
				else if(sscanf(buffer, "%f", &value) == 1)
				{
					eSyntax = S_KEYVALUE3;
					printf("  F(%f)\n", value);
				}
				break;
			case S_KEYVALUE3:
				if(eVar != V_NA)
				{
					eSyntax = S_CONDITIONB;
					printf("  V(%s)\n", g_Vars[eVar]);
				}
				else if(sscanf(buffer, "%f", &value) == 1)
				{
					eSyntax = S_CONDITIONB;
					printf("  F(%f)\n", value);
				}
				break;

			case S_RIGHTA:
				if(eVar != V_NA)
				{
					eSyntax = S_CONDITIONB;
					printf("  V(%s)\n", g_Vars[eVar]);
				}
				else if(sscanf(buffer, "%f", &value) == 1)
				{
					eSyntax = S_CONDITIONB;
					printf("  F(%f)\n", value);
				}
				else
				{
					printf("syntax error: expected right side of comparison\n");
					return 0;
				}
				break;

			case S_COMPARISONA:
				if(Comparison(eKeyword))
				{
					eSyntax = S_RIGHTA;
					printf("  COMPA=%s\n", g_Keywords[eKeyword]);
				}
				else
				{
					printf("syntax error: expected an comparison got %s\n", buffer);
					return 0;
				}
				break;

			case S_END:
				if(eKeyword == K_END)
				{
					return 0;
				}
				printf("syntax error: missing end\n");
				break;

			default:
				break;
			}

		}
		else
		{
			break;
		}
	}

	return 0;
}


SProgram* Compile(char* in)
{
	char* p = in;
	// for each line parse the program
	while(*p != 0)
	{
		char buffer[256];
		char* ps = p;
		while((*p != '\n') && (*p != 0))
		{
			p++;					
		}
		p++;
		int len = p - ps - 1;
		memcpy(buffer, ps, len);
		buffer[len] = ' ';
		buffer[len + 1] = '#';
		buffer[len + 2] = ' ';
		buffer[len + 3] = 0;
		printf("\nline: \"%s\"\n", buffer);

		ParseLine(buffer);
	}
	return 0;
}


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
		SProgram* pProgram = Compile(p);
		free(p);
		fclose(file);
	}
/*
    UpdateProgram();
    UpdateProgram();
    UpdateProgram();
    UpdateProgram();
    UpdateProgram();
    UpdateProgram();
    UpdateProgram();
    UpdateProgram();
*/
    return 0;
}

