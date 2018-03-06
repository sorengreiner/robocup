#ifndef _ROBOCUP_H_
#define _ROBOCUP_H_

#include <stdbool.h>
#include <stdint.h>


typedef enum
{
    B_AND = 0,
    B_OR,
    B_XOR,
	NUM_BOOLEANOPERATOR
} EBooleanOperator;

/*
typedef enum
{
    NIL = 0,
    SPEED,
    ODOMETER,
    TIME,
    VALUE
} ENoun;
*/

typedef enum
{
	V_NIL = 0,
	V_SPEED,
	V_ODOMETER,
	V_ANGLE,
	V_TIME,
	NUM_VARS
} EVar;

typedef enum
{
	K_UNTIL = 0,
	K_CONST,
	K_END,
	NUM_KEYWORDS
} EKeyword;

typedef enum
{
    A_BACKWARD = 0,
    A_FOLLOW,
    A_FOLLOWLEFT,
    A_FOLLOWRIGHT,
    A_FORWARD,
    A_SET,
    A_TURNLEFT,
    A_TURNRIGHT,
	NUM_ACTIONS
} EAction;

typedef enum
{
	C_BRANCHLEFT = 0,
	C_BRANCHRIGHT,
	C_JUNCTIONLEFT,
	C_JUNCTIONRIGHT,
	C_LINE,
	NUM_COND
} ECond;

typedef enum
{
	O_EQUAL = 0,
	O_NOT_EQUAL,
	O_GREATER,
	O_GREATER_EQUAL,
	O_LESS,
	O_LESS_EQUAL,
	NUM_OP
} EOp;

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
	S_BOOLEAN,
	S_CONDITIONB,
	S_COMPARISONB,
	S_KEYVALUE4,
	S_KEYVALUE5,
	S_RIGHTB,
	S_END
} ESyntax;


typedef struct
{
	EAction eAction;
    bool (*pAction)(int noun0, float value0, int noun1, float value1); 
    EVar noun0; 
	float value0; 
	EVar noun1; 
	float value1;
    
	ECond eConditionA;
    bool (*pConditionA)(int noun0, float value0, int noun1, float value1); 
    EVar left_noun0; 
	float left_value0; 
	EOp eOpA;
	EVar left_noun1; 
	float left_value1;
    
    EBooleanOperator eBooleanOperator;
    
	ECond eConditionB;
    bool (*pConditionB)(int noun0, float value0, int noun1, float value1); 
    EVar right_noun0; 
	float right_value0; 
	EOp eOpB;
	EVar right_noun1; 
	float right_value1;
} SSequence;

void SequenceInit(SSequence* pSequence);

ECond MatchCondition(const char* token);
EOp MatchOperator(const char* token);
EBooleanOperator MatchBooleanOperator(const char* token);
EAction MatchAction(const char* token);
EVar MatchVar(const char* token);


#endif
