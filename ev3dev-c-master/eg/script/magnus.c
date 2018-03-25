#include "magnus.h"
#include "keys.h"
#include "actions.h"
#include "conditions.h"
#include "car.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>


#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h>   // for nanosleep
#else
#include <unistd.h> // for usleep
#endif


uint64_t TimeMilliseconds(void)
{
#ifdef WIN32
	return GetTickCount();
#else
struct timespec ts;

    if (clock_gettime (CLOCK_MONOTONIC, &ts) == 0)
	{
        return (uint64_t) (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
	}
    else
	{
        return 0;
	}
#endif
}


void SequenceInit(SSequence* pSequence)
{
	pSequence->eAction = NUM_ACTIONS;
	pSequence->pAction = 0;
	pSequence->noun0 = NUM_VARS;
	pSequence->value0 = 0;
	pSequence->noun1 = NUM_VARS;
	pSequence->value1 = 0;

	pSequence->eConditionA = NUM_COND;
	pSequence->pConditionA = 0;
	pSequence->left_noun0 = NUM_VARS;
	pSequence->left_value0 = 0;
	pSequence->eOpA = NUM_OP;
	pSequence->left_noun1 = NUM_VARS;
	pSequence->left_value1 = 0;

	pSequence->eBooleanOperator = NUM_BOOLEANOPERATOR;

	pSequence->eConditionB = NUM_COND;
	pSequence->pConditionB = 0;
	pSequence->right_noun0 = NUM_VARS;
	pSequence->right_value0 = 0;
	pSequence->eOpB = NUM_OP;
	pSequence->right_noun1 = NUM_VARS;
	pSequence->right_value1 = 0;
    
    pSequence->pNext = 0;
}


//-----------------------------------------------------------------------------
// Keywords
//-----------------------------------------------------------------------------


typedef struct 
{
	const char* name;
} SKeywordItem;

SKeywordItem g_Keywords[NUM_KEYWORDS] =
{
	{"until"},
	{"const"},
	{"#"}
};

EKeyword MatchKeyword(const char* token)
{
	for(int i = 0; i < NUM_KEYWORDS; i++)
	{
		if(strcasecmp(token, g_Keywords[i].name) == 0)
		{
			return (EKeyword)i;
		}
	}
	return NUM_KEYWORDS;
}


//-----------------------------------------------------------------------------
// Actions
//-----------------------------------------------------------------------------

typedef struct 
{
	const char* name;
    bool (*pFunction)(SState* s, int noun0, float value0, int noun1, float value1); 
} SActionItem;

SActionItem g_Actions[NUM_ACTIONS] =
{
	{"backward", 		Backward},
    {"circle",          Circle},
	{"follow", 			Follow},
	{"followcourse", 	FollowCourse},
	{"followleft", 		FollowLeft},
	{"followright", 	FollowRight},
   	{"forward", 		Forward},
	{"set", 			Set},
	{"stop", 			Stop},
	{"turnleft", 		TurnLeft},
	{"turnright", 		TurnRight},
	{"wait", 			Wait},
	{"tool", 			Tool},
};

EAction MatchAction(const char* token)
{
	for(int i = 0; i < NUM_ACTIONS; i++)
	{
		if(strcasecmp(token, g_Actions[i].name) == 0)
		{
			return (EAction)i;
		}
	}
	return NUM_ACTIONS;
}


//-----------------------------------------------------------------------------
// Conditions
//-----------------------------------------------------------------------------

typedef struct 
{
	const char* name;
    bool (*pFunction)(SState* s, int noun0, float value0, int noun1, float value1); 
} SCondItem;

SCondItem g_Conditions[NUM_COND] =
{
	{"branchleft", 		BranchLeft},
	{"branchright", 	BranchRight},
	{"junctionleft", 	JunctionLeft},
	{"junctionright", 	JunctionRight},
	{"line", 			Line},
	{"noline", 			NoLine},
};

ECond MatchCondition(const char* token)
{
	for(int i = 0; i < NUM_COND; i++)
	{
		if(strcasecmp(token, g_Conditions[i].name) == 0)
		{
			return (ECond)i;
		}
	}
	return NUM_COND;
}


//-----------------------------------------------------------------------------
// Operators
//-----------------------------------------------------------------------------

typedef struct 
{
	const char* name;
    bool (*pFunction)(SState* s, int noun0, float value0, int noun1, float value1); 
} SOpItem;

SOpItem g_Operators[NUM_OP] =
{
	{"==", 				Equal},
	{"!=", 				NotEqual},
	{">=", 				GreaterEqual},
	{">", 				Greater},
	{"<=", 				LessEqual},
	{"<", 				Less},
	{"!~", 				ApproximateNotEqual},
	{"~", 				Approximate},
};

EOp MatchOperator(const char* token)
{
	for(int i = 0; i < NUM_OP; i++)
	{
		if(strcmp(token, g_Operators[i].name) == 0)
		{
			return (EOp)i;
		}
	}
	return NUM_OP;
}


//-----------------------------------------------------------------------------
// Boolean Operators
//-----------------------------------------------------------------------------

typedef struct 
{
	const char* name;
} SBoolOpItem;

SBoolOpItem g_BooleanOperators[NUM_BOOLEANOPERATOR] =
{
	{"and"},
	{"or"},
	{"xor"}
};

EBooleanOperator MatchBooleanOperator(const char* token)
{
	for(int i = 0; i < NUM_BOOLEANOPERATOR; i++)
	{
		if(strcasecmp(token, g_BooleanOperators[i].name) == 0)
		{
			return (EBooleanOperator)i;
		}
	}
	return NUM_BOOLEANOPERATOR;
}


//-----------------------------------------------------------------------------
// Vars
//-----------------------------------------------------------------------------

SVarItem g_Vars[NUM_VARS] =
{
	{"nil",			0.0 },
	{"true",		1.0 },
	{"false",		0.0 },
	{"speed",		0.0 },
	{"odometer",	0.0 },
	{"lodometer",	0.0 },
	{"rodometer",	0.0 },
	{"aodometer",	0.0 },
	{"mark",		0.0 },
	{"angle",		0.0 },
	{"radius",		0.0 },
	{"time",		0.0 },
	{"heading",		0.0 },
	{"xpos",		0.0 },
	{"ypos",		0.0 },
	{"steer",		0.0 },
	{"kp",			1.0 },
	{"ki",			0.0 },
	{"kd",			0.0 },
	{"kps",			1.0 },
	{"kis",			0.0 },
	{"kds",			0.0 },
	{"white",		50.0 },
	{"black",		30.0 },
    {"threshold",	0.66 },
	{"toolpos",		 0.0 },
	{"toolspeed",	 10.0 },
	{"course",	     0.0 },
	{"prox",	   100.0 },
};


void SetVar(EVar eVar, float value)
{
	if(eVar < NUM_VARS)
	{
		g_Vars[eVar].value = value;
	}
}

float GetVar(EVar eVar)
{
	return g_Vars[eVar].value;
}


EVar MatchVar(const char* token)
{
	for(int i = 0; i < NUM_VARS; i++)
	{
		if(strcasecmp(token, g_Vars[i].name) == 0)
		{
			return (EVar)i;
		}
	}
	return NUM_VARS;
}


EVar MatchKeyValue(const char* token, float* pValue)
{
	char* pe = strchr(token, '=');
	char* pv = 0;
	if(pe != 0)
	{
		*pe = 0;
		pv = pe + 1;
	}
	else
	{
		return NUM_VARS;
	}
	
	if(sscanf(pv, "%f", pValue) != 1)
	{
		return NUM_VARS;
	}

	for(int i = 0; i < NUM_VARS; i++)
	{
		if(strcasecmp(token, g_Vars[i].name) == 0)
		{
			return (EVar)i;
		}
	}

	return NUM_VARS;
}

//-----------------------------------------------------------------------------
/// Helper
//-----------------------------------------------------------------------------


void PrintAction(EAction eAction)
{
	if(eAction < NUM_ACTIONS)
	{
		printf("%s", g_Actions[eAction].name);
	}
	else
	{
		printf("NA");
	}
}

void PrintValue(EVar eVar, float value)
{
	if(eVar < NUM_VARS)
	{
		printf("%s=%g", g_Vars[eVar].name, value);
	}
	else
	{
		printf("NA");
	}
}

void PrintValuePretty(EVar eVar, float value)
{
	if(eVar < NUM_VARS && eVar != V_NIL)
	{
		printf("%s", g_Vars[eVar].name);
	}
	else
	{
		printf("%g", value);
	}
}

void PrintCondition(ECond eCond)
{
	if(eCond < NUM_COND)
	{
		printf("%s", g_Conditions[eCond].name);
	}
	else
	{
		printf("NA");
	}
}

void PrintOperator(EOp eOp)
{
	if(eOp < NUM_OP)
	{
		printf("%s", g_Operators[eOp].name);
	}
	else
	{
		printf("NA");
	}
}

void PrintBoolean(EBooleanOperator eBool)
{
	if(eBool < NUM_BOOLEANOPERATOR)
	{
		printf("%s", g_BooleanOperators[eBool].name);
	}
	else
	{
		printf("NA");
	}
}

void SequencePrint(SSequence* pSequence)
{
    printf("%4d   ", pSequence->line);
	PrintAction(pSequence->eAction);
	printf(" ");
	PrintValue(pSequence->noun0, pSequence->value0);
	printf(" ");
	PrintValue(pSequence->noun1, pSequence->value1);
	printf(" ");

	printf("until ");

	PrintCondition(pSequence->eConditionA);
	printf(" ");
	PrintValue(pSequence->left_noun0, pSequence->left_value0);
	printf(" ");
	PrintOperator(pSequence->eOpA);
	printf(" ");
	PrintValue(pSequence->left_noun1, pSequence->left_value1);
	printf(" ");

	PrintBoolean(pSequence->eBooleanOperator);
	printf(" ");

	PrintCondition(pSequence->eConditionB);
	printf(" ");
	PrintValue(pSequence->right_noun0, pSequence->right_value0);
	printf(" ");
	PrintOperator(pSequence->eOpB);
	printf(" ");
	PrintValue(pSequence->right_noun1, pSequence->right_value1);
	printf("\n");
}


void SequencePrintPretty(SSequence* pSequence)
{
    printf("%4d   ", pSequence->line);
    if(pSequence->eAction != NUM_ACTIONS)
    {
        PrintAction(pSequence->eAction);
        if(pSequence->noun0 != NUM_VARS)
        {
            printf(" ");
            PrintValue(pSequence->noun0, pSequence->value0);
        }
        
        if(pSequence->noun1 != NUM_VARS)
        {
            printf(" ");
            PrintValue(pSequence->noun1, pSequence->value1);
        }

        if(pSequence->pConditionA)
        {
            printf(" ");
            printf("until ");

            if(pSequence->eConditionA != NUM_COND)
            {
                PrintCondition(pSequence->eConditionA);
                printf(" ");
            }
            else
            {
                PrintValuePretty(pSequence->left_noun0, pSequence->left_value0);
                printf(" ");
                PrintOperator(pSequence->eOpA);
                printf(" ");
                PrintValuePretty(pSequence->left_noun1, pSequence->left_value1);
                printf(" ");
            }
            
            if(pSequence->eBooleanOperator != NUM_BOOLEANOPERATOR)
            {
                PrintBoolean(pSequence->eBooleanOperator);
                printf(" ");

                if(pSequence->eConditionB != NUM_COND)
                {
                    PrintCondition(pSequence->eConditionB);
                    printf(" ");
                }
                else
                {
                    PrintValuePretty(pSequence->right_noun0, pSequence->right_value0);
                    printf(" ");
                    PrintOperator(pSequence->eOpB);
                    printf(" ");
                    PrintValuePretty(pSequence->right_noun1, pSequence->right_value1);
                }
            }
        }
    }
	printf("\n");
   
}


bool ParseLine(char* in, SSequence* pItem)
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
//			printf("  token[%d]: '%s'\n", len, buffer);
	
			// Search keywords
			EKeyword eKeyword = MatchKeyword(buffer);
			EAction eAction = MatchAction(buffer);
			ECond eCond = MatchCondition(buffer);
			float value = 0;
			EOp eOp = MatchOperator(buffer);
			EVar eVar = MatchVar(buffer);
			EVar eKeyValue = MatchKeyValue(buffer, &value);
			EBooleanOperator eBooleanOperator = MatchBooleanOperator(buffer);

			switch(eSyntax)
			{
			case S_ACTION:
				if(eAction != NUM_ACTIONS)
				{
					eSyntax = S_KEYVALUE0;
//					printf("  ACTION=%s\n", g_Actions[eAction].name);
					pItem->eAction = eAction;
					pItem->pAction = g_Actions[eAction].pFunction;
				}
				else
				{
					printf("syntax error: expected an action got %s\n", buffer);
					return false;
				}
				break;
			case S_KEYVALUE0:
				if(eKeyword == K_UNTIL)
				{
//					printf("  UNTIL\n");
					eSyntax = S_CONDITIONA;
				}
				else if(eKeyValue != NUM_VARS)
				{
//					printf("  KV(%s,%f)\n", g_Vars[eKeyValue].name, value);
					pItem->noun0 = eKeyValue;
					pItem->value0 = value;
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
				else if(eKeyValue != NUM_VARS)
				{
//					printf("  KV(%s,%f)\n", g_Vars[eKeyValue].name, value);
					pItem->noun1 = eKeyValue;
					pItem->value1 = value;
					eSyntax = S_UNTIL;
				}
				break;
			case S_UNTIL:
				if(eKeyword == K_UNTIL)
				{
//					printf("  UNTIL\n");
					eSyntax = S_CONDITIONA;
				}
				else if(eKeyword == K_END)
				{
					return 0;
				}
				else
				{
					printf("syntax error: expected until\n");
					return false;
				}
				break;
			case S_CONDITIONA:
				if(eCond != NUM_COND)
				{
					eSyntax = S_KEYVALUE2;
//					printf("  CONDA=%s\n", g_Conditions[eCond].name);
					pItem->eConditionA = eCond;
					pItem->pConditionA = g_Conditions[eCond].pFunction;
				}
				else if(eVar != NUM_VARS)
				{
					eSyntax = S_COMPARISONA;
//					printf("  V(%s)\n", g_Vars[eVar].name);
					pItem->left_noun0 = eVar;					
				}
				else if(sscanf(buffer, "%f", &value) == 1)
				{
					eSyntax = S_COMPARISONA;
//					printf("  F(%f)\n", value);
					pItem->left_noun0 = V_NIL;					
					pItem->left_value0 = value;					
				}
				else
				{
					printf("syntax error: expected a condition\n");
					return false;
				}
				break;
			case S_KEYVALUE2:
				if(eVar != NUM_VARS)
				{
					eSyntax = S_KEYVALUE3;
//					printf("  V(%s)\n", g_Vars[eVar].name);
					pItem->left_noun0 = eVar;					
				}
				else if(sscanf(buffer, "%f", &value) == 1)
				{
					eSyntax = S_KEYVALUE3;
//					printf("  F(%f)\n", value);
					pItem->left_noun0 = V_NIL;					
					pItem->left_value0 = value;					
				}
				break;
			case S_KEYVALUE3:
				if(eVar != NUM_VARS)
				{
					eSyntax = S_BOOLEAN;
//					printf("  V(%s)\n", g_Vars[eVar].name);
					pItem->left_noun1 = eVar;
				}
				else if(sscanf(buffer, "%f", &value) == 1)
				{
					eSyntax = S_BOOLEAN;
//					printf("  F(%f)\n", value);
					pItem->left_noun1 = V_NIL;					
					pItem->left_value1 = value;					
				}
				break;

			case S_BOOLEAN:
				if(eBooleanOperator != NUM_BOOLEANOPERATOR)
				{
					eSyntax = S_CONDITIONB;
//					printf("  BO(%s)\n", g_BooleanOperators[eBooleanOperator].name);
					pItem->eBooleanOperator = eBooleanOperator;
				}

				break;

			case S_COMPARISONA:
				if(eOp != NUM_OP)
				{
					eSyntax = S_KEYVALUE3;
//					printf("  OP(%s)\n", g_Operators[eOp]);
					pItem->eOpA = eOp;
					pItem->pConditionA = g_Operators[eOp].pFunction;
				}
				else
				{
					printf("syntax error: expected an comparison got %s\n", buffer);
					return false;
				}
				break;

			case S_COMPARISONB:
				if(eOp != NUM_OP)
				{
					eSyntax = S_KEYVALUE5;
//					printf("  OP(%s)\n", g_Operators[eOp]);
					pItem->eOpB = eOp;
					pItem->pConditionB = g_Operators[eOp].pFunction;
				}
				else
				{
					printf("syntax error: expected an comparison got %s\n", buffer);
					return false;
				}
				break;

			case S_CONDITIONB:
				if(eCond != NUM_COND)
				{
					eSyntax = S_KEYVALUE4;
//					printf("  CONDB=%s\n", g_Conditions[eCond].name);
					pItem->eConditionB = eCond;
					pItem->pConditionB = g_Conditions[eCond].pFunction;
				}
				else if(eVar != NUM_VARS)
				{
					eSyntax = S_COMPARISONB;
//					printf("  V(%s)\n", g_Vars[eVar].name);
					pItem->right_noun0 = eVar;					
				}
				else if(sscanf(buffer, "%f", &value) == 1)
				{
					eSyntax = S_COMPARISONB;
//					printf("  F(%f)\n", value);
					pItem->right_noun0 = V_NIL;					
					pItem->right_value0 = value;					
				}
				else
				{
					printf("syntax error: expected a condition\n");
					return false;
				}
				break;

			case S_KEYVALUE4:
				if(eVar != NUM_VARS)
				{
					eSyntax = S_KEYVALUE5;
//					printf("  V(%s)\n", g_Vars[eVar].name);
					pItem->right_noun0 = eVar;					
				}
				else if(sscanf(buffer, "%f", &value) == 1)
				{
					eSyntax = S_KEYVALUE5;
//					printf("  F(%f)\n", value);
					pItem->right_noun0 = V_NIL;					
					pItem->right_value0 = value;					
				}
				break;
			case S_KEYVALUE5:
				if(eVar != NUM_VARS)
				{
					eSyntax = S_END;
//					printf("  V(%s)\n", g_Vars[eVar].name);
					pItem->right_noun1 = eVar;
				}
				else if(sscanf(buffer, "%f", &value) == 1)
				{
					eSyntax = S_BOOLEAN;
//					printf("  F(%f)\n", value);
					pItem->right_noun1 = V_NIL;					
					pItem->right_value1 = value;					
				}
				break;

			case S_END:
				if(eKeyword == K_END)
				{
					return false;
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

	return true;
}


bool Compile(char* in, SProgram* pProgram)
{
    pProgram->pFirst = 0;
    pProgram->pLast = 0;
    
	char* p = in;
	// for each line parse the program
    int line = 1;
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

		SSequence item;
        SequenceInit(&item);
		if(!ParseLine(buffer, &item))
        {
            printf("error parsing program line:%d\n", line);
            return false;
        }
        
        SSequence* pSequence = malloc(sizeof(SSequence));
        *pSequence = item;
        pSequence->line = line;
        if(pProgram->pFirst == 0)
        {
            pProgram->pFirst = pSequence;
            pProgram->pLast = pSequence;
        }
        pProgram->pLast->pNext = pSequence;
        pProgram->pLast = pSequence;
        
        line++;
	}
	return true;
}

#define PROGRAM_UPDATE_RATE (30)

void RunProgram(SProgram* pProgram)
{
    printf("Run program\n");
    SSequence* pSequence = pProgram->pFirst;
	SState s = {0};

	uint64_t t0 = TimeMilliseconds();
    while(pSequence)
    {
		uint64_t t1 = t0;
		t0 = TimeMilliseconds();
		float delta = (t0 - t1);
//        printf("dt:%f\n", delta);
		
		bool bProceed = false;
		if(pSequence->pAction)
		{
			if(s.index == 0)
			{
				SequencePrintPretty(pSequence);
				AssignVar(pSequence->noun0, pSequence->value0);
				AssignVar(pSequence->noun1, pSequence->value1);
			}
			
			UpdateVars(delta/1000.0);
			s.dt = delta/1000.0;
			
			bool bImmidiateReturn = pSequence->pAction(&s, pSequence->noun0, pSequence->value0, pSequence->noun1, pSequence->value1);
			
			if(bImmidiateReturn)
			{
				bProceed = true;
			}
			else
			{
				if((pSequence->pConditionA != 0) && (pSequence->pConditionB != 0) && (pSequence->eBooleanOperator != NUM_BOOLEANOPERATOR))
				{
					switch(pSequence->eBooleanOperator)
					{
						case B_AND:
						bProceed = pSequence->pConditionA(&s, pSequence->left_noun0, pSequence->left_value0, pSequence->left_noun1, pSequence->left_value1)
						&& pSequence->pConditionB(&s, pSequence->right_noun0, pSequence->right_value0, pSequence->right_noun1, pSequence->right_value1);
						break;
					
						case B_OR:
						bProceed = pSequence->pConditionA(&s, pSequence->left_noun0, pSequence->left_value0, pSequence->left_noun1, pSequence->left_value1)
						|| pSequence->pConditionB(&s, pSequence->right_noun0, pSequence->right_value0, pSequence->right_noun1, pSequence->right_value1);
						break;
					}
				}
				else if(pSequence->pConditionA != 0)
				{
					bProceed = pSequence->pConditionA(&s, pSequence->left_noun0, pSequence->left_value0, pSequence->left_noun1, pSequence->left_value1);
				}
				else
				{
				}
			}
		}
		else
		{
			bProceed = true;
		}

		s.index++;
		
		uint8_t keys = KeysRead();
		if(keys == EV3_KEY_BACK)
		{
			printf("Program cancelled\n");
			break;
		} 

		if(bProceed)
		{
		    pSequence = pSequence->pNext;
		    s.index = 0;
		}
		
    	uint64_t tend = TimeMilliseconds();
        int tdiff = (int)(tend - t0);
        if(tdiff < PROGRAM_UPDATE_RATE)
        {
            // throttle the update period
            int remain = PROGRAM_UPDATE_RATE - tdiff;
            sleep_ms(remain);
        }
        else
        {
            // sleep anyway to allow rest of system to get a time slice
            sleep_ms(2);
        }
    }
}


void PrintProgram(SProgram* pProgram)
{
    printf("Print program\n");
    SSequence* pSequence = pProgram->pFirst;
    while(pSequence)
    {
		SequencePrint(pSequence);
        pSequence = pSequence->pNext;
    }
}


void DeleteProgram(SProgram* pProgram)
{
    printf("Delete program\n");
    SSequence* pSequence = pProgram->pFirst;
    while(pSequence)
    {
        SSequence* p = pSequence;
        pSequence = pSequence->pNext;
        free(p);
    }
}


bool LoadProgram(SProgram* pProgram, const char* filename)
{
    bool bProgramLoaded = false;
    FILE* file = fopen(filename, "r");
    if(file != 0)
    {
        // Load script
        fseek(file, 0l, SEEK_END);
        size_t size = ftell(file);
        fseek(file, 0l, SEEK_SET);
        char* p = malloc(size + 1);
        int n = fread(p, 1, size, file);
        p[n] = 0;
        if(Compile(p, pProgram))
        {
            bProgramLoaded = true;
        }
        free(p);
        fclose(file);
	return bProgramLoaded;
    }
    else
    {
        printf("Program file %s not found\n", filename);
        return false;
    }
}


