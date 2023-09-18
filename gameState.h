// GameState.h

#ifndef GAME_STATE_H
#define GAME_STATE_H

// Global State Machine Setup
enum class GameState {
    Title,
    Credits,
    ModeSelect,
    GameSetup,
    LevelScreen,
    Playing,
    EndScreen
};

extern GameState gameState; // Declare gameState as an extern variable

#endif
