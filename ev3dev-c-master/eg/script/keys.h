#ifndef _KEYS_H_
#define _KEYS_H_

#include <stdbool.h>
#include <stdint.h> 
#include "brick.h"

//-----------------------------------------------------------------------------
// Keys
//-----------------------------------------------------------------------------
/*
enum
{
	EV3_KEY__NONE_ = 0,

	EV3_KEY_UP     = 0x01L,
	EV3_KEY_DOWN   = 0x02L,
	EV3_KEY_LEFT   = 0x04L,
	EV3_KEY_RIGHT  = 0x08L,
	EV3_KEY_CENTER = 0x10L,
	EV3_KEY_BACK   = 0x20L,
};
*/
bool KeysOpen(void);
void KeysClose(void);
uint8_t KeysRead(void); 

#endif
