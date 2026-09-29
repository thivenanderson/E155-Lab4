// lab4_starter.c
// Fur Elise, E155 Lab 4
// Updated Fall 2024
#include "STM32L432KC_RCC.h"
#include "STM32L432KC_GPIO.h"
#include "STM32L432KC_FLASH.h"
#include "STM32L432KC_TIMER.h"

#define SPEAKER_PIN 7 // Speaker output on PB7
#define TIM6_COUNT_HZ 991000 // Observed TIM6 counter frequency after prescale
#define TIM7_TICKS_PER_MS 10 // TIM7 ticks per millisecond

//FUR ELISE
// Pitch in Hz, duration in ms
//const int notes[][2] = {
//{659,	125},
//{623,	125},
//{659,	125},
//{623,	125},
//{659,	125},
//{494,	125},
//{587,	125},
//{523,	125},
//{440,	250},
//{  0,	125},
//{262,	125},
//{330,	125},
//{440,	125},
//{494,	250},
//{  0,	125},
//{330,	125},
//{416,	125},
//{494,	125},
//{523,	250},
//{  0,	125},
//{330,	125},
//{659,	125},
//{623,	125},
//{659,	125},
//{623,	125},
//{659,	125},
//{494,	125},
//{587,	125},
//{523,	125},
//{440,	250},
//{  0,	125},
//{262,	125},
//{330,	125},
//{440,	125},
//{494,	250},
//{  0,	125},
//{330,	125},
//{523,	125},
//{494,	125},
//{440,	250},
//{  0,	125},
//{494,	125},
//{523,	125},
//{587,	125},
//{659,	375},
//{392,	125},
//{699,	125},
//{659,	125},
//{587,	375},
//{349,	125},
//{659,	125},
//{587,	125},
//{523,	375},
//{330,	125},
//{587,	125},
//{523,	125},
//{494,	250},
//{  0,	125},
//{330,	125},
//{659,	125},
//{  0,	250},
//{659,	125},
//{1319,	125},
//{  0,	250},
//{623,	125},
//{659,	125},
//{  0,	250},
//{623,	125},
//{659,	125},
//{623,	125},
//{659,	125},
//{623,	125},
//{659,	125},
//{494,	125},
//{587,	125},
//{523,	125},
//{440,	250},
//{  0,	125},
//{262,	125},
//{330,	125},
//{440,	125},
//{494,	250},
//{  0,	125},
//{330,	125},
//{416,	125},
//{494,	125},
//{523,	250},
//{  0,	125},
//{330,	125},
//{659,	125},
//{623,	125},
//{659,	125},
//{623,	125},
//{659,	125},
//{494,	125},
//{587,	125},
//{523,	125},
//{440,	250},
//{  0,	125},
//{262,	125},
//{330,	125},
//{440,	125},
//{494,	250},
//{  0,	125},
//{330,	125},
//{523,	125},
//{494,	125},
//{440,	500},
//{  0,	0}};

//TOKYO DRIFT
const int notes[][2] = {
    { 123, 234},
    {   0, 235},
    { 123, 234},
    {   0, 235},
    { 123, 234},
    {   0, 234},
    { 123, 235},
    {   0, 234},
    { 123, 234},
    {   0, 118},
    { 131, 234},
    {   0, 117},
    { 165, 234},
    { 123, 235},
    {   0, 234},
    { 123, 235},
    {   0, 234},
    { 123, 234},
    {   0, 118},
    { 131, 234},
    {   0, 117},
    { 165, 235},
    { 123, 234},
    {   0, 234},
    { 123, 235},
    {   0, 234},
    { 123, 234},
    {   0, 118},
    { 131, 234},
    {   0, 117},
    { 165, 234},
    { 185, 235},
    {   0, 234},
    { 185, 235},
    {   0, 234},
    { 220, 234},
    {   0, 118},
    { 196, 234},
    {   0, 117},
    { 185, 235},
    { 165, 234},
    {   0, 234},
    { 165, 235},
    {   0, 234},
    { 220, 234},
    {   0, 118},
    { 196, 234},
    {   0, 117},
    { 185, 234},
    { 165, 235},
    {   0, 234},
    { 165, 235},
    {   0, 234},
    { 123, 234},
    {   0, 118},
    { 131, 234},
    {   0, 117},
    { 165, 235},
    { 123, 234},
    {   0, 234},
    { 123, 235},
    {   0, 234},
    { 123, 234},
    {   0, 118},
    { 131, 234},
    {   0, 117},
    { 165, 234},
    { 123, 235},
    {   0, 234},
    { 123, 235},
    {0, 0}
};

int numNotes = (sizeof(notes) / sizeof(notes[0]))-1; // Set number of notes by finding rows of notes array

int frequencyToARR(int frequency) {
    return (TIM6_COUNT_HZ/(2*frequency)) - 1; // Convert pitch to TIM6 ARR value
}

int durationToARR(int duration_ms) {
    return (duration_ms*TIM7_TICKS_PER_MS) - 1; // Convert duration to TIM7 ARR value
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
        clearTIMFlag(TIM6); // Clear old pitch flag
        startTIM(TIM6); // Start pitch timer
        startTIM(TIM7); // Start duration timer

        while(!timerDone(TIM7)) {
            if (timerDone(TIM6)) {
                togglePin(SPEAKER_PIN); // Toggle speaker output every half period
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
    initTIM(TIM6, 79);
    initTIM(TIM7, 7999);

    // Play song
    for(int i=0; i<numNotes; i++) {
        playNote(notes[i][0], notes[i][1]);
    }

    // Tests
    //playNote(1000,2000);

    return 0;
}
