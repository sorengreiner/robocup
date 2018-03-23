#ifndef _CONDITIONS_H_
#define _CONDITIONS_H_

#include "magnus.h"

// Condition functions
bool Approximate(SState* s, int noun0, float value0, int noun1, float value1);
bool ApproximateNotEqual(SState* s, int noun0, float value0, int noun1, float value1);
bool BranchLeft(SState* s, int noun0, float value0, int noun1, float value1);
bool BranchRight(SState* s, int noun0, float value0, int noun1, float value1);
bool Equal(SState* s, int noun0, float value0, int noun1, float value1);
bool Greater(SState* s, int noun0, float value0, int noun1, float value1);
bool GreaterEqual(SState* s, int noun0, float value0, int noun1, float value1);
bool JunctionLeft(SState* s, int noun0, float value0, int noun1, float value1);
bool JunctionRight(SState* s, int noun0, float value0, int noun1, float value1);
bool Less(SState* s, int noun0, float value0, int noun1, float value1);
bool LessEqual(SState* s, int noun0, float value0, int noun1, float value1);
bool Line(SState* s, int noun0, float value0, int noun1, float value1);
bool NoLine(SState* s, int noun0, float value0, int noun1, float value1);
bool NotEqual(SState* s, int noun0, float value0, int noun1, float value1);


#endif
