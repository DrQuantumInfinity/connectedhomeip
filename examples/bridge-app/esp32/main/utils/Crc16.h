#pragma once
#include <stdint.h>

/**************************************************************************
 *                                  Constants
 **************************************************************************/
/**************************************************************************
 *                                  Macros
 **************************************************************************/
/**************************************************************************
 *                                  Types
 **************************************************************************/
/**************************************************************************
 *                                  Prototypes
 **************************************************************************/
uint16_t Crc16Block(uint16_t crc, void const *pData, uint32_t numBytes);