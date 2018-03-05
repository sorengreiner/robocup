#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

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


int main()
{
    UpdateProgram();
    UpdateProgram();
    UpdateProgram();
    UpdateProgram();
    UpdateProgram();
    UpdateProgram();
    UpdateProgram();
    UpdateProgram();
    return 0;
}

