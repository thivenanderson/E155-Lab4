#include "STM32L432KC_TIMER.h"
#include "STM32L432KC_RCC.h"
#include "STM32L432KC_GPIO.h"


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
     TIM7->ARR = (duration_ms*10) - 1;
     //Start counter from 0 for each note
     TIM7->CNT = 0;
     //Clear update flags
     TIM7->SR &= ~(1 << 0);
}

void playNote(int frequency, int duration_ms){
     //Set duration/ARR for TIM7
     setDuration(duration_ms);
     //For rest force output low and ignore frequency
     if (frequency == 0){
        //Turn off pitch timer aka TIM6
        TIM6->CR1 &= ~(1<<0);
        //Turn speaker off
        digitalWrite(SPEAKER_PIN, GPIO_LOW);
        //Start TIM7
        TIM7->CR1 |= (1<<0);
        //Wait for TIM7 to finish
        while(!(TIM7->SR & (1 << 0)));
        //Stop timer once finished
        TIM7->CR1 &= ~(1<<0);
        //Clear update flag
        TIM7->SR &= ~(1 << 0);

     }
     //For not set frequency and toggle LED pin everytime TIM6 finishes
     else{
        //Set frequency
        setFrequency(frequency);
        //Start TIM6
        TIM6->CR1 |= (1<<0);
        //Start TIM7
        TIM7->CR1 |= (1<<0);
        //Wait for TIM7 to finish
        while(!(TIM7->SR & (1 << 0))){
            //wait for TIM6 to finsih
            if (TIM6->SR & (1 << 0)){
                //toggle output pin
                togglePin(SPEAKER_PIN);
                //Clear update flag
                TIM6->SR &= ~(1 << 0);
            }
        }
        //Turn speaker off when note duration finished
        digitalWrite(SPEAKER_PIN, GPIO_LOW);
        //Stop timers once finished
        TIM6->CR1 &= ~(1<<0);
        TIM7->CR1 &= ~(1<<0);
        //Clear update flags
        TIM6->SR &= ~(1 << 0);
        TIM7->SR &= ~(1 << 0);
     }
}