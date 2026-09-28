#include "STM32L432KC_TIMER.h"
#include "STM32L432KC_RCC.h"

void initTIM6(void) {
     // enable TIM6 clock in RCC
     RCC->APB1ENR1 |= (1 << 4);
     // set PSC to change 80Mhz clock to 1Mhz
     TIM6->PSC = 79; 
     // Load prescaler value
     TIM6->EGR |= (1 << 0);
     // Clear update flag caused by EGR
     TIM6->SR &= ~(1 << 0);
}

void initTIM7(void) {
     // enable TIM7 clock in RCC
     RCC->APB1ENR1 |= (1 << 5);
     // set PSC to change clock to 0.1 ms/10kHz ticks
     TIM7->PSC = 7999; 
       // Load prescaler value
     TIM7->EGR |= (1 << 0);
     // Clear update flag caused by EGR
     TIM7->SR &= ~(1 << 0);
}

void setFrequency(int frequency) {
     //Set ARR based on frequency of note
     TIM6->ARR = (1000000/(2*frequency)) - 1;
     //Start counter from 0 for each note
     TIM6->CNT = 0;
     //Clear update flags
     TIM6->SR &= ~(1 << 0);
}

void setDuration(int duration_ms){
     //Set ARR based on note duration
     TIM7->ARR = (duration_ms*10) - 1;\
     //Start counter from 0 for each note
     TIM7->CNT = 0;
     //Clear update flags
     TIM7->SR &= ~(1 << 0);
}