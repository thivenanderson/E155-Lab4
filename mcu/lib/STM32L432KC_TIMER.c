#include "STM32L432KC_TIMER.h"
#include "STM32L432KC_RCC.h"

void initTIM(void) {
    // enable TIM6 clock in RCC
     RCC->APB1ENR1 |= (1 << 4);
    // set PSC
     TIM6->PSC = 79; 
    // set ARR
    
    // enable counter
}