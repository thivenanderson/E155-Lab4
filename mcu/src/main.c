//Name: Thiven Anderson
//Email: thanderson@g.hmc.edu
//Date: 9/29/2026
//Description: Main program for lab 4 of E155 that uses timers to play note arrays through a speaker
#include "STM32L432KC_RCC.h"
#include "STM32L432KC_GPIO.h"
#include "STM32L432KC_FLASH.h"
#include "STM32L432KC_TIMER.h"
//#include "tokyo_drift.h"
//#include "fur_elise.h"
#include "kaikai_kitan.h"


#define SPEAKER_PIN 7 // Speaker output on PB7
#define TIMER_CLOCK_HZ 79280000 // Observed timer clock based on oscilloscope measurement
#define TIM6_PRESCALER 79 // Prescale timer clock to about 991 kHz
#define TIM7_PRESCALER 7999 // Prescale timer clock to about 9.91 kHz
#define TIM6_COUNT_HZ (TIMER_CLOCK_HZ/(TIM6_PRESCALER+1)) // TIM6 counter frequency
#define TIM7_COUNT_HZ (TIMER_CLOCK_HZ/(TIM7_PRESCALER+1)) // TIM7 counter frequency



int frequencyToARR(int frequency) {
    return (TIM6_COUNT_HZ/(2*frequency)) - 1; // Convert pitch to TIM6 ARR value
}

int durationToARR(int duration_ms) {
    return ((duration_ms*TIM7_COUNT_HZ)/1000) - 1; // Convert duration to TIM7 ARR value
}

void playNote(int frequency, int duration_ms) {
    setTIMARR(TIM7, durationToARR(duration_ms)); // Set note duration
    resetTIM(TIM7); // Start duration counter from 0
    clearTIMFlag(TIM7); // Clear old duration flag

    // Play rest
    if (frequency == 0) {
        stopTIM(TIM6); // Make sure pitch timer is off
        digitalWrite(SPEAKER_PIN, GPIO_LOW); // Turn speaker off
        startTIM(TIM7); // Start duration timer
        while(!timerDone(TIM7)); // Wait for rest duration to finish
        stopTIM(TIM7); // Stop duration timer
        clearTIMFlag(TIM7); // Clear duration flag
    }
    // Play note
    else {
        setTIMARR(TIM6, frequencyToARR(frequency)); // Set note pitch
        resetTIM(TIM6); // Start pitch counter from 0
        clearTIMFlag(TIM6); // Clear old pitch flag to be safe
        startTIM(TIM6); // Start pitch timer
        startTIM(TIM7); // Start duration timer

        while(!timerDone(TIM7)) {
            if (timerDone(TIM6)) {
                togglePin(SPEAKER_PIN); // Toggle speaker output every half period of note
                clearTIMFlag(TIM6); // Clear pitch flag
            }
        }

        digitalWrite(SPEAKER_PIN, GPIO_LOW); // Turn speaker off after note
        stopTIM(TIM6); // Stop pitch timer
        stopTIM(TIM7); // Stop duration timer
        clearTIMFlag(TIM6); // Clear pitch flag
        clearTIMFlag(TIM7); // Clear duration flag
    }
}

int main(void) {
    configureFlash(); // Configure flash to add waitstates to avoid timing errors
    configureClock(); // Setup the PLL and switch clock source to the PLL
    RCC->AHB2ENR |= (1 << 1); // Turn on clock to GPIOB
    pinMode(SPEAKER_PIN, GPIO_OUTPUT); // Set speaker pin as output
    digitalWrite(SPEAKER_PIN, GPIO_LOW); // Start speaker output low

    // Initialize timers
    initTIM(TIM6, TIM6_PRESCALER);
    initTIM(TIM7, TIM7_PRESCALER);

    // Play song
    for(int i=0; i<numNotes; i++) {
        playNote(notes[i][0], notes[i][1]);
    }

    // Tests
    //playNote(1000,2000);

    return 0;
}
