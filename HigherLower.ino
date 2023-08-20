// HIGHERLOWER
// A game of logic, to guess the random number using only two clues: HIGHER or LOWER!
//
// This started life as the tutorial by @Crait
// https://community.arduboy.com/t/7928
//
// Code & Graphics licenced under GNU GPL 3.0
//
// 15 August 2023 (Original)
// 17 August 2023 (Major Refactor)
// 20 August 2023 (Public Release - 1.0.0)
// Nathan Chapman -- @nchpm

// Library Setup
#include <Arduboy2.h>
Arduboy2 a;
#include <ArduboyTones.h>
#include <ArduboyTonesPitches.h>
ArduboyTones sound(a.audio.enabled);

// Variable Setup
bool playerwin;
int attempts;
int guessednumber;
int randomlimit;
int randomnumber;
int lastguess;

// These are all for button-debounce-repeat
// Sourced from Mr Blinky: https://community.arduboy.com/t/5856/3
int buttonDebounceState; 
int buttonDebounceDelay; 
int buttonRepeatDelay;
int oldButtonState;
int buttonState;
#define BUTTON_DEBOUNCE_DELAY 10 // time for button states to stabilise
#define BUTTON_REPEAT_DELAY 250 // time before buttons start to repeat
#define BUTTON_REPEAT_SPEED 125 // button repeat interval


// State Machine Setup
enum class GameState {
    Title,
	Playing,
	EndScreen,
    Credits
};
// Variable for tracking State Machine current state
GameState gameState = GameState::Playing;


// Title Screen!
const uint8_t PROGMEM title[] = {
128, 64,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0x00, 0x00, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0x00, 0x00, 0x80, 0x80, 0xe0, 0xe0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xe0, 0xe0, 0x80, 0x80, 0x00, 0x00, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0x00, 0x00, 0xc0, 0xc0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0x00, 0xc0, 0xc0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0x07, 0x03, 0xc3, 0xc3, 0xc7, 0xc7, 0xc7, 0xc7, 0xc7, 0xc7, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe3, 0xe3, 0xe3, 0xe3, 0xe3, 0xe3, 0xe3, 0xe3, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0x03, 0x03, 0x03, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0x07, 0x07, 0x07, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xc0, 0xc0, 0xc3, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0x07, 0x07, 0x07, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xc7, 0xc7, 0xc7, 0xc7, 0xc7, 0xc7, 0xc7, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x0f, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xe7, 0xc1, 0xc1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xff, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0xf0, 0xf0, 0xf0, 0xf0, 0xff, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0xf0, 0xff, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00, 0x03, 0x03, 0x0f, 0x0f, 0x0f, 0x3f, 0x3f, 0xff, 0xff, 0xff, 0x3f, 0x3f, 0x0f, 0x03, 0x03, 0x00, 0x00, 0x00, 0x30, 0x3f, 0xff, 0xff, 0x3f, 0x3f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x30, 0x3f, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x30, 0x00, 0x03, 0x03, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x3f, 0x3f, 0xf0, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x3f, 0x3f, 0xff, 0xff, 0xff, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7c, 0x7c, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xfc, 0xfc, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xfc, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x3f, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xfc, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xfc, 0xfc, 0xff, 0xff, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xfc, 0xfc, 0xff, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xfc, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xfc, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xc0, 0xf6, 0xc0, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xc0, 0xc0, 0xfe, 0xfe, 0xff, 0xc0, 0xc0, 0xde, 0xc0, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc8, 0xc8, 0xda, 0xc2, 0xc2, 0xfe, 0xfe, 0xc0, 0xc0, 0xfe, 0xfe, 0xff, 0xc0, 0xc0, 0xf6, 0xc0, 0xc0, 0xff, 0xc0, 0xc0, 0xf6, 0xc0, 0xc8, 0xff, 0xfe, 0xfe, 0xc0, 0xc0, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
};

// Win Screen!
const uint8_t PROGMEM win[] = {
128, 64,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x3f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x3f, 0x3f, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf0, 0xfc, 0xf0, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x3f, 0x3f, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x70, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xfc, 0x7c, 0x7f, 0x7f, 0x7c, 0x7c, 0x70, 0xf0, 0xf0, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0xf0, 0x70, 0x70, 0x70, 0x7c, 0x7c, 0x7f, 0xff, 0xfc, 0xfc, 0xf0, 0xf0, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0xf0, 0xf0, 0x10, 0x10, 0x00, 0x04, 0x04, 0x07, 0x1f, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x3f, 0x3f, 0x1f, 0x0f, 0x0f, 0x07, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x3f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x3f, 0x3f, 0x1f, 0x0f, 0x0f, 0x07, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf0, 0x80, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xf0, 0xf0, 0x40, 0x40, 0x40, 0x40, 0xf0, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0x3f, 0x1f, 0x1f, 0x0f, 0x07, 0x07, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xf8, 0xe0, 0xe0, 0x80, 0x80, 0x81, 0x81, 0x81, 0xe0, 0xe0, 0xf8, 0xf8, 0xe0, 0xe0, 0x81, 0x81, 0x81, 0x81, 0x80, 0x80, 0xe0, 0xe0, 0xf8, 0xf8, 0xff, 0xff, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xff, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xff, 0xff, 0xff, 0xf8, 0xf8, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xff, 0xff, 0xe1, 0xe1, 0x80, 0x80, 0x80, 0x80, 0xe1, 0xe1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0x3f, 0x1f, 0x1f, 0x0f, 0x87, 0x87, 0x83, 0x81, 0x81, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x80, 0x80, 0xbf, 0xbf, 0x89, 0xbf, 0x3f, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x81, 0x81, 0xbf, 0xbf, 0x81, 0x01, 0x80, 0xbf, 0xbf, 0xa1, 0xbf, 0xbf, 0x00, 0x00, 0x00, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0x3f, 0x1f, 0x1f, 0x0f, 0x07, 0x07, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x04, 0x1f, 0x1b, 0x00, 0x1f, 0x1f, 0x12, 0x1a, 0x18, 0x00, 0x1b, 0x1b, 0x12, 0x1e, 0x1e, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x04, 0x1f, 0x1f, 0x00, 0x1f, 0x1f, 0x04, 0x1f, 0x1b, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

// Lose Screen :(
const uint8_t PROGMEM lose[] = {
128, 64,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x07, 0x07, 0x0f, 0x0f, 0x1f, 0x1f, 0x3f, 0x3f, 0x7f, 0x7f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0xef, 0x0f, 0x0f, 0xef, 0xef, 0xff, 0x0f, 0x0f, 0x6f, 0x0f, 0x8f, 0xff, 0x8f, 0x8f, 0x3f, 0x3f, 0x8f, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x0f, 0x6f, 0x0f, 0x0f, 0xff, 0x0f, 0x0f, 0xef, 0x4f, 0x4f, 0xff, 0x0f, 0x0f, 0x6f, 0x0f, 0x0f, 0xff, 0xef, 0x0f, 0x0f, 0xef, 0xff, 0x0f, 0x0f, 0x9f, 0x3f, 0x0f, 0x0f, 0xff, 0xff, 0xff, 
0x00, 0x00, 0x00, 0x00, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0x00, 0x00, 0xc0, 0xc0, 0xf0, 0xf0, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xf0, 0xf0, 0xc0, 0xc0, 0x00, 0x00, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0x00, 0x01, 0x01, 0x03, 0x03, 0x07, 0x07, 0x0f, 0x0f, 0x1f, 0x1f, 0x3f, 0x3f, 0x3f, 0x7c, 0x7c, 0xff, 0xff, 0xff, 0xfc, 0xfc, 0xff, 0xfc, 0xfc, 0xff, 0xff, 0x0f, 0x0c, 0x6c, 0x0f, 0x0f, 0xff, 0x0f, 0x0f, 0x6f, 0x0f, 0x8f, 0xfc, 0x0c, 0x0f, 0xac, 0xac, 0xef, 0xfc, 0x8c, 0x8d, 0xac, 0x2c, 0x2f, 0xfc, 0x8c, 0x8f, 0xac, 0x2c, 0x2f, 0xfd, 0xfc, 0xfc, 0xfd, 0xff, 0xfc, 0x0c, 0x0f, 0x6f, 0x0c, 0x0c, 0xff, 0xff, 0xff, 
0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0x03, 0x00, 0x03, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x07, 0x07, 0x0f, 0x0f, 0x1c, 0x1c, 0x3f, 0x3f, 0x3f, 0x7f, 0x7c, 0xfc, 0xff, 0xfc, 0xfc, 0xff, 0xfc, 0xfc, 0xfd, 0xfc, 0xfc, 0xff, 0xfc, 0xfc, 0xfd, 0xfc, 0xfc, 0xff, 0xfc, 0xfc, 0xfd, 0xfc, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xfc, 0xff, 0xfc, 0xfc, 0xff, 0xff, 0xff, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x07, 0x07, 0x0f, 0x0f, 0x1f, 0x1f, 0x3f, 0x3f, 0x3f, 0x7f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0x00, 0x00, 0x00, 0x00, 0xe0, 0xe0, 0xe3, 0xe3, 0xe3, 0xe3, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x80, 0x80, 0xe0, 0xe0, 0xe3, 0xe3, 0xe3, 0xe3, 0xe3, 0xe3, 0xe3, 0x83, 0x83, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0xc0, 0xc0, 0xe0, 0xe0, 0xe3, 0xe3, 0xe3, 0xe3, 0xe3, 0xe3, 0xe3, 0xe3, 0xe3, 0xc3, 0xc3, 0x03, 0x03, 0x80, 0x80, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0x00, 0xf8, 0xf8, 0xfe, 0xfe, 0xfe, 0xfe, 0xf8, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x07, 0x07, 0x0f, 0x0f, 0x1f, 0x1f, 0x1f, 0x3f, 0x3f, 0x7f, 0x7f, 0xff, 0xff, 
0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0x1f, 0x1f, 0x07, 0x1f, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0x00, 0x00, 0x3f, 0x3f, 0x7f, 0x7f, 0xff, 0xff, 0xc7, 0xc7, 0xc7, 0xc7, 0x87, 0x8f, 0x8f, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xc7, 0xc7, 0xc7, 0xc7, 0xc7, 0xc7, 0xc7, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x80, 0x80, 0x80, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xf8, 0xf8, 0xf8, 0xf8, 0xfb, 0xfb, 0x83, 0x87, 0x87, 0x87, 0x87, 0xff, 0xff, 0xfc, 0xfc, 0xf8, 0xf8, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0x00, 0x83, 0x83, 0xef, 0xef, 0xef, 0xef, 0x83, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x00, 0x07, 0x07, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x07, 0x07, 0x00, 0x00, 0x07, 0x07, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x07, 0x07, 0x00, 0x00, 0x07, 0x07, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x00, 0x07, 0x07, 0x1f, 0x1f, 0x1f, 0x1f, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

// Play Screen Title
const uint8_t PROGMEM playHeader[] = {
128, 23,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0x07, 0xf7, 0x27, 0x27, 0xff, 0x07, 0x07, 0xff, 0x07, 0x07, 0xff, 0x07, 0x07, 0xd7, 0x57, 0x77, 0xff, 0x47, 0x47, 0xd7, 0x17, 0x17, 0xff, 0x47, 0x47, 0xd7, 0x17, 0x17, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xf7, 0x07, 0x07, 0xf7, 0xf7, 0xff, 0x07, 0x07, 0xdf, 0x07, 0x07, 0xff, 0x07, 0x07, 0xd7, 0x57, 0x77, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0x07, 0xcf, 0x9f, 0x07, 0x07, 0xff, 0x07, 0x07, 0xff, 0x07, 0x07, 0xff, 0x07, 0x07, 0xcf, 0x9f, 0xcf, 0x07, 0x07, 0xff, 0x07, 0x07, 0xd7, 0x07, 0x27, 0xff, 0x07, 0x07, 0xd7, 0x57, 0x77, 0xff, 0x07, 0x07, 0xb7, 0x07, 0x47, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0x03, 0x6a, 0x02, 0x12, 0xfe, 0x02, 0x03, 0x6a, 0x2a, 0x3a, 0xfe, 0xfa, 0xfb, 0x02, 0x02, 0xfa, 0xfa, 0xfe, 0x03, 0x02, 0x9e, 0xce, 0x9e, 0x02, 0x03, 0xfe, 0x02, 0x02, 0x6a, 0x2a, 0x3b, 0xff, 0x03, 0x03, 0x6b, 0x2b, 0x3b, 0xff, 0x02, 0x02, 0xe7, 0xcf, 0x03, 0x02, 0xfe, 0xff, 0xfe, 0xfe, 0xff, 0xfe, 0x7a, 0x02, 0x02, 0x7e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x02, 0x02, 0xdb, 0x03, 0x02, 0xfe, 0x03, 0x02, 0xe6, 0xce, 0x02, 0x02, 0xff, 0x02, 0x02, 0x7b, 0x03, 0x83, 0xfe, 0xfe, 0xff, 0xfe, 0xfe, 0xfe, 0x7a, 0x02, 0x03, 0x7e, 0xfe, 0x02, 0x02, 0x7a, 0x03, 0x02, 0xfe, 0x03, 0x02, 0x7a, 0x03, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 
};

// Credits
const uint8_t PROGMEM credits[] = {
128, 64,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0x03, 0x7b, 0x33, 0x33, 0xff, 0x03, 0x03, 0x7b, 0x03, 0x03, 0xff, 0x03, 0x03, 0x7b, 0x03, 0x83, 0xff, 0x03, 0x03, 0x6b, 0x2b, 0x3b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xdf, 0x07, 0x07, 0xdf, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0x03, 0x7b, 0x13, 0x13, 0xff, 0x03, 0x03, 0xdb, 0x03, 0x23, 0xff, 0x03, 0x03, 0xdb, 0x03, 0x03, 0xff, 0x03, 0x03, 0xdb, 0xc3, 0xc3, 0xff, 0x03, 0x03, 0xef, 0x03, 0x03, 0xff, 0x7b, 0x03, 0x03, 0x7b, 0xff, 0x03, 0x03, 0x7b, 0x33, 0x33, 0xff, 0x23, 0x23, 0x6b, 0x0b, 0x0b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0x03, 0xe7, 0xcf, 0x03, 0x03, 0xff, 0x03, 0x03, 0x7b, 0x33, 0x33, 0xff, 0x03, 0x03, 0xef, 0x03, 0x03, 0xff, 0x03, 0x03, 0xdb, 0xc3, 0xc3, 0xff, 0x03, 0x03, 0xe7, 0xcf, 0xe7, 0x03, 0x03, 0xff, 0x03, 0x03, 0xe7, 0xcf, 0x03, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x0f, 0x0f, 0x0f, 0x0f, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0x7f, 0x7f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x1f, 0x1f, 0x1f, 0x1f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x3f, 0x25, 0x3f, 0x3b, 0x00, 0x3f, 0x3f, 0x09, 0x3f, 0x3f, 0x00, 0x37, 0x37, 0x25, 0x3d, 0x3d, 0x00, 0x3f, 0x3f, 0x25, 0x35, 0x31, 0x00, 0x3f, 0x3f, 0x21, 0x3f, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x3f, 0x21, 0x3f, 0x3f, 0x00, 0x3f, 0x3f, 0x06, 0x0c, 0x3f, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x3f, 0x21, 0x33, 0x33, 0x00, 0x3f, 0x3f, 0x21, 0x3f, 0x3f, 0x00, 0x3f, 0x3f, 0x21, 0x3f, 0x1f, 0x00, 0x3f, 0x3f, 0x25, 0x35, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x3f, 0x25, 0x3f, 0x3b, 0x00, 0x07, 0x07, 0x3c, 0x3c, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x3f, 0x21, 0x33, 0x33, 0x00, 0x3f, 0x3f, 0x09, 0x3f, 0x37, 0x00, 0x3f, 0x3f, 0x09, 0x3f, 0x3f, 0x00, 0x21, 0x3f, 0x3f, 0x21, 0x00, 0x01, 0x01, 0x3f, 0x3f, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

// Setup code, to run once
void setup() {
    a.begin();
    a.setFrameRate(60);

    randomlimit = 101; // Upper limit of guessable numbers (+1 to account for random())
    a.initRandomSeed(); // Seeds the random() function - remove this line to produce identical results every time

    gameState = GameState::Title;
    restartGame();
}


void endScreen(bool playerWin) {
    a.setCursor(0,0);
    if (playerWin) {
        // Write our win grapic (array) to screen at position x0 y0
        Sprites::drawOverwrite(0, 0, win, 0);
        a.digitalWriteRGB(RGB_OFF,RGB_ON,RGB_OFF);
    } else {
        // Write our win grapic (array) to screen at position x0 y0
        Sprites::drawOverwrite(0, 0, lose, 0);
    }
}

// restartGame() - to reset variables and prepare for a new game
void restartGame() {
    // reset game variables
    attempts = 0;
    playerwin = false;
    guessednumber = 1;

    // random another taget number
    randomnumber = random(1,randomlimit); // random() generates lower to upper-1
    // randomnumber = 0;

    // turn off the LED
    a.digitalWriteRGB(RGB_OFF,RGB_OFF,RGB_OFF);

    if (a.justPressed(A_BUTTON)) {
        gameState = GameState::Playing;
    }
}


// Main code, to run repeatedly so long as the program is running
void loop() {
	// If it's not time to draw the next frame
	if (!a.nextFrame())
		// Exit the loop function early
		return;
	
    a.clear(); // clear the screen
    a.pollButtons(); // track button presses since the last loop - used to only register single button presses at a time
    int currentButtonState = a.buttonsState();

    if (currentButtonState == buttonDebounceState) {
        if (buttonDebounceDelay > 0) { // If still in debounce timeout period
            buttonDebounceDelay--;
        } else {
            // debounce timeout expired, buttons state is not changing
            if (currentButtonState == oldButtonState) {
                if (buttonRepeatDelay > 0) {
                    // still in repeat timeout period
                    buttonRepeatDelay--;
                    buttonState = 0;
                } else {
                    // repeat timeout expired, repeat button
                    buttonState = oldButtonState;
                    buttonRepeatDelay = BUTTON_REPEAT_SPEED;
                }
            } else {
                // pressed buttons changed
                buttonState = (~oldButtonState) & currentButtonState;
                oldButtonState = currentButtonState;
                buttonRepeatDelay = BUTTON_REPEAT_DELAY;
            }
        } ////////////////////////////
    } else {
        // Still bouncing
        buttonDebounceState = currentButtonState;
        buttonDebounceDelay = BUTTON_DEBOUNCE_DELAY;
    }

	// Choose what to do based on
	// the value of the 'gameState' variable
	switch(gameState) {
        // If 'gameState' is 'GameState::Title'
        case GameState::Title: { // Show the title screen and wait for button press
            // Write our title grapic (array) to screen at position x0 y0
            Sprites::drawOverwrite(0, 0, title, 0);

            if (a.justPressed(A_BUTTON)) {
                gameState = GameState::Playing;
            }

            if (a.justPressed(B_BUTTON)) {
                gameState = GameState::Credits;
            }
        }
        break;


		// If 'gameState' is 'GameState::Playing'
		case GameState::Playing: { // Ask the player for a number and play the game
            if (attempts == 7) { // Too many guesses - game over!

                // Initiate the 'you lose' tune
                sound.tone(NOTE_D5,300, NOTE_G4,300, NOTE_C4,200);

                // Transition the game into the 'player lost' state
                gameState = GameState::EndScreen;
            } else {
                if(a.pressed(UP_BUTTON)) { // If we're holding the up button
                    
                }



                if (buttonState & UP_BUTTON) { // Guessed number lower
                    a.digitalWriteRGB(RGB_OFF,RGB_OFF,RGB_OFF);
                    if ((guessednumber - 1) > 0) {
                        guessednumber = guessednumber - 1;
                        sound.tone(NOTE_E2,80);
                    } else {
                        return; // do nothing?
                    }
                }

                if (a.justPressed(A_BUTTON)) { // Submit guess
                    if (guessednumber == randomnumber) { // Correct guess!
                        // Initiate the 'you win' tune
                        sound.tone(NOTE_D5,300, NOTE_G5,300, NOTE_C6,200);

                        // Transition the game into the 'player lost' state
                        playerwin = true;
                        gameState = GameState::EndScreen;
                        endScreen(playerwin);
                        
                    } else { // If the guess is incorrect
                        attempts = attempts + 1;
                        lastguess = guessednumber;
                        a.digitalWriteRGB(RGB_ON,RGB_OFF,RGB_OFF);
                        sound.tone(NOTE_D1,60, NOTE_REST,20, NOTE_D1,60);
                    }

                }
                
                Sprites::drawOverwrite(0, 0, playHeader, 0);

                a.setCursor(0,30);
                a.print("     Attempt: ");
                a.print(attempts);
                a.print("\n Number to Guess: ");
                a.print(guessednumber);

                if(attempts > 0) {
                    // After the first guess, give a hint
                    a.print("\n\n    ");
                    a.print(lastguess);
                    if (lastguess > randomnumber) {
                        a.print(" was too high");
                    } else {
                        a.print(" was too low");
                    }
                }
            }
        }
        break;
			
		
		case GameState::EndScreen: {  // If 'gameState' is 'GameState::EndScreen'
            endScreen(playerwin);          
            // Let the player start a new game - reinitialise variables
            if (a.justPressed(A_BUTTON)) {
                restartGame();
            }
        }
        break;

        case GameState::Credits: {  // If 'gameState' is 'GameState::Credits'
            Sprites::drawOverwrite(0, 0, credits, 0);

            a.setCursor(0,50);
            a.print("v1.0.0");

            // Let the player start a new game - reinitialise variables just in case
            if (a.justPressed(A_BUTTON)||a.justPressed(B_BUTTON)) {
                restartGame();
            }
        }
        break;
	}
	
	// Update the screen
	a.display();
}