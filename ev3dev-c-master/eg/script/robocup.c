#include "robocup.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
}


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


// Condition functions

bool False(int noun0, float value0, int noun1, float value1)
{ return false; }

bool True(int noun0, float value0, int noun1, float value1)
{ return false; }


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


bool BranchLeft(int noun0, float value0, int noun1, float value1) { return true; }
bool BranchRight(int noun0, float value0, int noun1, float value1) { return true; }
bool JunctionLeft(int noun0, float value0, int noun1, float value1) { return true; }
bool JunctionRight(int noun0, float value0, int noun1, float value1) { return true; }
bool Line(int noun0, float value0, int noun1, float value1) { return true; }
bool Equal(int noun0, float value0, int noun1, float value1) { return true; }
bool NotEqual(int noun0, float value0, int noun1, float value1) { return true; }
bool Greater(int noun0, float value0, int noun1, float value1) { return true; }
bool GreaterEqual(int noun0, float value0, int noun1, float value1) { return true; }
bool Less(int noun0, float value0, int noun1, float value1) { return true; }
bool LessEqual(int noun0, float value0, int noun1, float value1) { return true; }


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


bool Backward(int noun0, float value0, int noun1, float value1) { return true; }
bool Follow(int noun0, float value0, int noun1, float value1) { return true; }
bool FollowLeft(int noun0, float value0, int noun1, float value1) { return true; }
bool FollowRight(int noun0, float value0, int noun1, float value1) { return true; }
bool Forward(int noun0, float value0, int noun1, float value1) { return true; }
bool Set(int noun0, float value0, int noun1, float value1) { return true; }
bool TurnLeft(int noun0, float value0, int noun1, float value1) { return true; }
bool TurnRight(int noun0, float value0, int noun1, float value1) { return true; }



// Action( noun, value, noun, value) until Condition(noun, value, noun, value) AND/OR Condition(noun, value, noun value)

/*
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
*/
typedef struct
{
	SSequence* pSequence;
	int sequenceCount;
} SProgram;



typedef struct 
{
	const char* name;
} SKeywordItem;

SKeywordItem g_Keywords[NUM_KEYWORDS] =
{
	{"UNTIL"},
	{"CONST"},
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
    bool (*pFunction)(int noun0, float value0, int noun1, float value1); 
} SActionItem;

SActionItem g_Actions[NUM_ACTIONS] =
{
	{"BACKWARD", 		Backward},
	{"FOLLOW", 			Follow},
	{"FOLLOWLEFT", 		FollowLeft},
	{"FOLLOWRIGHT", 	FollowRight},
	{"FORWARD", 		Forward},
	{"SET", 			Set},
	{"TURNLEFT", 		TurnLeft},
	{"TURNRIGHT", 		TurnRight},
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
    bool (*pFunction)(int noun0, float value0, int noun1, float value1); 
} SCondItem;

SCondItem g_Conditions[NUM_COND] =
{
	{"BRANCHLEFT", 		BranchLeft},
	{"BRANCHRIGHT", 	BranchRight},
	{"JUNCTIONLEFT", 	JunctionLeft},
	{"JUNCTIONRIGHT", 	JunctionRight},
	{"LINE", 			Line}
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
    bool (*pFunction)(int noun0, float value0, int noun1, float value1); 
} SOpItem;

SOpItem g_Operators[NUM_OP] =
{
	{"==", 				Equal},
	{"!=", 				NotEqual},
	{">", 				Greater},
	{">=", 				GreaterEqual},
	{"<", 				Less},
	{"<=", 				LessEqual},
};

EOp MatchOperator(const char* token)
{
	for(int i = 0; i < NUM_OP; i++)
	{
		if(strcasecmp(token, g_Operators[i].name) == 0)
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

typedef struct 
{
	const char* name;
    float value; 
} SVarItem;


SVarItem g_Vars[] =
{
	{"NIL",			0.0 },
	{"SPEED",		0.0 },
	{"ODOMETER",	0.0 },
	{"ANGLE",		0.0 },
	{"TIME",		0.0 }
};

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
		printf("%s=%f", g_Vars[eVar].name, value);
	}
	else
	{
		printf("NA");
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
	PrintAction(pSequence->eAction);
	printf(" ");
	PrintValue(pSequence->noun0, pSequence->value0);
	printf(" ");
	PrintValue(pSequence->noun1, pSequence->value1);
	printf(" ");

	printf("UNTIL ");

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



int ParseLine(char* in, SSequence* pItem)
{
	ESyntax eSyntax = S_ACTION;
	char* p = in;
	// for each line parse the program

//	SSequence item;
	SequenceInit(pItem);
//	SequencePrint(&item);

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
			EVar eKeyValue = MatchKeyValue(buffer, &value);
			EVar eVar = MatchVar(buffer);
			EOp eOp = MatchOperator(buffer);
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
					return 0;
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
					return 0;
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
					return 0;
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
				}
				else
				{
					printf("syntax error: expected an comparison got %s\n", buffer);
					return 0;
				}
				break;

			case S_COMPARISONB:
				if(eOp != NUM_OP)
				{
					eSyntax = S_KEYVALUE5;
//					printf("  OP(%s)\n", g_Operators[eOp]);
					pItem->eOpB = eOp;
				}
				else
				{
					printf("syntax error: expected an comparison got %s\n", buffer);
					return 0;
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
					return 0;
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

		SSequence item;
		ParseLine(buffer, &item);
		SequencePrint(&item);
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

