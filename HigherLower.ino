// TUTORIAL: HIGHER LOWER GAME
// https://community.arduboy.com/t/7928
//
// 15 August 2023
// Nathan Chapman -- @nchpmn

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

// Setup code, to run once
void setup() {
    a.begin();
    a.setFrameRate(60);

    randomlimit = 11; // Upper limit of guessable numbers (+1 to account for random())
    a.initRandomSeed(); // Seeds the random() function - remove this line to produce identical results every time

    restartGame();
}

// restartGame() - to reset variables and prepare for a new game
void restartGame() {
    randomnumber = random(1,randomlimit); // random() generates lower to upper-1
    // randomnumber = 0;
    attempts = 0;
    playerwin = false;
    guessednumber = 0;
    a.digitalWriteRGB(RGB_OFF,RGB_OFF,RGB_OFF);

    // Happy little chiptune to start the game!
    // const uint16_t song1[] PROGMEM = {
    //     NOTE_GS3,1000, NOTE_REST,250, NOTE_C3,500, NOTE_B3,200,
    //     TONES_END };

    // sound.tones(song1);
}

// endScreen() - a final screen when the game is over, win or lose
void endScreen(bool playerWin) {
    a.setCursor(0,0);
    if (playerWin) {
        a.print("\n\n      YOU WIN!");
        a.digitalWriteRGB(RGB_OFF,RGB_ON,RGB_OFF);
    } else {
        // sound.tone(NOTE_D5,300, NOTE_G4,300, NOTE_C4,200);
        a.print("\n\n      YOU LOSE!");
    }

    a.print("\n  Correct Number: ");
    a.print(randomnumber);
    a.print("\n\n\n");
    a.print("Press A to play again");

    // Let the player start a new game - reinitialise variables
    if (a.justPressed(A_BUTTON)) {
        restartGame();
    }
}

// Main code, to run repeatedly so long as the program is running
void loop() {
    if (!a.nextFrame()) {
        return;
    }
    a.clear(); // clear the screen
    a.pollButtons(); // track button presses since the last loop - used to only register single button presses at a time

    if (playerwin) {
        // Tell the player that they won!
        endScreen(playerwin);

    } else {
        // Ask the player for a number and play the game
        if (attempts == 7) {
            // Too many guesses - game over!
            sound.tone(NOTE_D5,300, NOTE_G4,300, NOTE_C4,200);
            endScreen(playerwin);
        } else {
            // Player has more attempts
            if (a.justPressed(UP_BUTTON)) { // Guessed number higer
                guessednumber = guessednumber + 1;
                a.digitalWriteRGB(RGB_OFF,RGB_OFF,RGB_OFF);
                sound.tone(NOTE_E2,80);
            }

            if (a.justPressed(DOWN_BUTTON)) { // Guessed number lower
                guessednumber = guessednumber -1;
                a.digitalWriteRGB(RGB_OFF,RGB_OFF,RGB_OFF);
                sound.tone(NOTE_D2,80);
            }

            if (a.justPressed(A_BUTTON)) { // Submit guess
                if (guessednumber == randomnumber) {
                    playerwin = true;
                } else {
                    attempts = attempts + 1;
                    lastguess = guessednumber;
                    a.digitalWriteRGB(RGB_ON,RGB_OFF,RGB_OFF);
                    sound.tone(NOTE_D1,60, NOTE_REST,20, NOTE_D1,60);
                }

            }

            a.setCursor(0,0);
            a.print("   GUESS THE NUMBER");
            a.print("\n  BETWEEN 1 AND ");
            a.print(randomlimit - 1);
            a.print("\n\n     Attempt: ");
            a.print(attempts);
            a.print("\n Number to Guess: ");
            a.print(guessednumber);

            if(attempts > 0) {
                // After the first guess, give a hint
                a.print("\n\n\n    ");
                a.print(lastguess);
                if (lastguess > randomnumber) {
                    a.print(" was too high");
                } else {
                    a.print(" was too low");
                }
            }
        }
    }

    a.display();

}