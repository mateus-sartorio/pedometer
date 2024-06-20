#pragma once

#include <stdint.h>

#include "macros.h"

#define BUTTON1 BIT(9)
#define BUTTON2 BIT(10)

void Button_Init(uint32_t buttons);
uint32_t Button_Read(void);
uint32_t Button_ReadChanges(void);
uint32_t Button_ReadPressed(void);
uint32_t Button_ReadReleased(void);