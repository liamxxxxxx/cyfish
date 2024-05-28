#ifndef PLAYER_H
#define PLAYER_H
#include <stdbool.h>

// Struct for a player in the game
struct Player {
    int id;                 // The unique identifier for the player
    char name[50];          // The name of the player
    struct Penguin *penguins; // Pointer to the array of penguins owned by the player
    int penguineCount;      // The number of penguins owned by the player
    int score;              // The current score of the player
    bool knockedOut;        // Flag indicating whether the player is knocked out of the game or not
};

#endif //PLAYER_H