// STM32L432KC_TIMER.h
// Header for TIMER functions

#ifndef STM32L4_TIMER_H
#define STM32L4_TIMER_H
#define SPEAKER_PIN  7
#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////

#define __IO volatile

// Base addresses for TIMx ports
#define TIM6_BASE (0x40001000UL) // base address of TIM6
#define TIM7_BASE (0x40001400UL) // base address of TIM6

///////////////////////////////////////////////////////////////////////////////
// Bitfield struct for TIMx
///////////////////////////////////////////////////////////////////////////////

typedef struct {
  __IO uint32_t CR1;    // Address offset: 0x00
  __IO uint32_t CR2;    // Address offset: 0x04
  uint32_t RESERVED;    // Address offset: 0x08
  __IO uint32_t DIER;   // Address offset: 0x0C 
  __IO uint32_t SR;     // Address offset: 0x10
  __IO uint32_t EGR;    // Address offset: 0x14
  uint32_t RESERVED0;   // Address offset: 0x18
  uint32_t RESERVED1;   // Address offset: 0x1C
  uint32_t RESERVED2;   // Address offset: 0x20
  __IO uint32_t CNT;    // Address offset: 0x24
  __IO uint32_t PSC;    // Address offset: 0x28
  __IO uint32_t ARR;    // Address offset: 0x2C
} TIMx_TypeDef;

#define TIM6 ((TIMx_TypeDef *) TIM6_BASE)
#define TIM7 ((TIMx_TypeDef *) TIM7_BASE)

void initTIM(TIM_TypeDef *TIMx, uint16_t prescaler);
void setFrequency(int frequency);
void setDuration(int duration_ms);
void playNote(int frequency, int duration_ms);

#endif