// STM32L432KC_TIMER.h
// Header for TIMER functions

#ifndef STM32L4_TIMER_H
#define STM32L4_TIMER_H

#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////

#define __IO volatile

// Base addresses for GPIO ports
#define TIMER_BASE (0x40001000UL) // base address of TIM6

///////////////////////////////////////////////////////////////////////////////
// Bitfield struct for GPIO
///////////////////////////////////////////////////////////////////////////////

typedef struct {
  __IO uint32_t CR1;      /*  Address offset: 0x00 */
  __IO uint32_t CR2;     /*           Address offset: 0x04 */
  uint32_t RESERVED;  /*       Address offset: 0x08 */
  __IO uint32_t DIER;       /*         Address offset: 0x0C */
  __IO uint32_t SR;       /*         Address offset: 0x10 */
  __IO uint32_t EGR;    /*  Address offset: 0x14 */
  uint32_t RESERVED;    /*, Address offset: 0x18 */
  uint32_t RESERVED;    // Address offset: 0x1C
  uint32_t RESERVED;    // Address offset: GPIO Offset 0x20
  __IO uint32_t CNT;    // Address offset: 0x24
  __IO uint32_t PSC;    // Address offset: 0x28
  __IO uint32_t APR;    // Address offset: 0x2C
} TIMER_TypeDef;

#define TIMER ((TIMER_TypeDef *) TIMER_BASE)










#endif