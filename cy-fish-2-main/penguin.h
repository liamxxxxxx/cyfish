#ifndef PENGUINE_H
#define PENGUINE_H

// Struct for a penguin in the game
struct Penguin {
    int id;                 // The unique identifier for the penguin
    struct Player *owner;   // Pointer to the player who owns this penguin

    int row;                // The current row position of the penguin on the game board
    int column;             // The current column position of the penguin on the game board
};

#endif //PENGUINE_H