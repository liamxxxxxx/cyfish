#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdbool.h>

#include "penguin.h"

// Struct for the game state
typedef struct {
    struct Player *players;     // Pointer to the array of players
    int playerCount;            // The number of players

    struct Cell *grid;          // Pointer to the game grid
    int rows;                   // The number of rows in the game grid
    int columns;                // The number of columns in the game grid

    int currentPlayer;          // The index of the current player
    int round;                  // The current round number

    char *display_grid;         // Pointer to the display grid
} GameState;

// Struct for a move in the game
struct Move {
    int row;                    // The row of the move
    int column;                 // The column of the move
};

// Function prototypes
void init_grid(GameState *state, int rows, int columns); // Initializes the game grid
void init_players(GameState *state, int playerCount); // Initializes the players
struct Cell *get_cell(GameState *state, int row, int column); // Returns a pointer to the cell at the given row and column
int xy_to_index(GameState *state, int row, int column); // Converts a row and column to an index in the game grid
void display_grid(GameState *state); // Displays the game grid
void next_player(GameState *state); // Advances to the next player
struct Move* get_penguin_valid_moves(GameState *state, struct Penguin *penguin, int *moveCount); // Returns a pointer to an array of valid moves for a penguin
void destroy_state(GameState *state); // Frees the memory allocated for the game state

#endif //GAME_STATE_H