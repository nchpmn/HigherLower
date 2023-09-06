#ifndef PLAYINGSTATE_H
#define PLAYINGSTATE_H

#include <Arduboy2.h>
#include <ArduboyTones.h>

class PlayingState {
public:
    PlayingState(Arduboy2 &arduboy);

    // Add any public member functions you need for PlayingState
    void init();
    void update();
    void draw();

private:
    // Add any private member functions or variables needed for PlayingState
    Arduboy2 &arduboy;
};

#endif // PLAYINGSTATE_H