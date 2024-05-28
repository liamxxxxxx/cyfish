#include "game_state.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cell.h"
#include "color.h"
#include "penguin.h"
#include "player.h"
// Define  mouvement


// Moves the position up
void up(int *row, int *col) {
    *row -= 1;
}

// Moves the position down
void down(int *row, int *col) {
    *row += 1;
}

// Moves the position to the up left
void up_left(int *row, int *col) {
    if (*col % 2 == 0)
        *row -= 1;
    *col -= 1;
}

// Moves the position to the up right
void up_right(int *row, int *col) {
    if (*col % 2 == 0)
        *row -= 1;
    *col += 1;
}

// Moves the position to the down left
void down_left(int *row, int *col) {
    if (*col % 2 != 0)
        *row += 1;
    *col -= 1;
}

// Moves the position to the down right
void down_right(int *row, int *col) {
    if (*col % 2 != 0)
        *row += 1;
    *col += 1;
}

// Checks if the given position is valid
bool isValidPosition(GameState *state, int row, int col) {
    return row >= 0 && row < state->rows && col >= 0 && col < state->columns;
}

// Converts a row and column to an index in the display grid
char *xy_to_index_display_grid(GameState *state, char *display_grid, int row, int column, int x, int y) { // transform y to postion index - 2d index to array index
    return display_grid + (row * 4 + y + (column % 2 != 0 ? 2 : 0)) * (state->columns * 6 + 3) + column * 6 + x;
}

// Converts a row and column to an index in the game grid
int xy_to_index(GameState *state, int row, int column) {
    return row * state->columns + column;
}

// Advances to the next player
void next_player(GameState *state) {
    state->currentPlayer = (state->currentPlayer + 1) % state->playerCount;
    if (state->currentPlayer == 0)
        state->round++; // Changing turns
}

// Frees the memory allocated for the game state
void destroy_state(GameState *state) {
    free(state->grid);
    free(state->display_grid);

    for (int i = 0; i < state->playerCount; ++i)
        free(state->players[i].penguins);
    free(state->players);
}

// Returns a pointer to an array of valid moves for a penguin
struct Move *get_penguin_valid_moves(GameState *state, struct Penguin *penguin, int *moveCount) {
    struct Move *moves = NULL;
    int row, column;

    /*
     * 4,4 -> 4,3  5,3  5,4  4,5  3,4  3,3
     * 3,3 -> 3,2  4,3  4,4  3,4  2,4  2,3 // This function returns all the possible moves the penguin can do in the grid,
                                             // Later the move entered will be checked if it is valid or not
     */

    // UP-WARD
    column = penguin->column;
    row = penguin->row;
    while (true) {
        up(&row, &column);
        if (!isValidPosition(state, row, column)) break;
        if (get_cell(state, row, column)->type == EMPTY) {
            moves = realloc(moves, (*moveCount + 1) * sizeof(struct Move));
            moves[*moveCount].row = row;
            moves[*moveCount].column = column;
            (*moveCount)++;
        } else
            break;
    }

    // DOWN-WARD
    column = penguin->column;
    row = penguin->row;
    while (true) {
        down(&row, &column);
        if (!isValidPosition(state, row, column)) break;
        if (get_cell(state, row, column)->type == EMPTY) {
            moves = realloc(moves, (*moveCount + 1) * sizeof(struct Move));
            moves[*moveCount].row = row;
            moves[*moveCount].column = column;
            (*moveCount)++;
        } else
            break;
    }

    // UP-RIGHT DIAGONAL (KIND OF RIGHT)
    column = penguin->column;
    row = penguin->row;
    while (true) {
        up_right(&row, &column);
        if (!isValidPosition(state, row, column)) break;
        if (get_cell(state, row, column)->type == EMPTY) {
            moves = realloc(moves, (*moveCount + 1) * sizeof(struct Move));
            moves[*moveCount].row = row;
            moves[*moveCount].column = column;
            (*moveCount)++;
        } else
            break;
    }

    // UP-LEFT DIAGONAL (KIND OF LEFT)
    column = penguin->column;
    row = penguin->row;
    while (true) {
        up_left(&row, &column);
        if (!isValidPosition(state, row, column)) break;
        if (get_cell(state, row, column)->type == EMPTY) {
            moves = realloc(moves, (*moveCount + 1) * sizeof(struct Move));
            moves[*moveCount].row = row;
            moves[*moveCount].column = column;
            (*moveCount)++;
        } else
            break;
    }

    // DOWN-RIGHT DIAGONAL
    column = penguin->column;
    row = penguin->row;
    while (true) {
        down_right(&row, &column);
        if (!isValidPosition(state, row, column)) break;
        if (get_cell(state, row, column)->type == EMPTY) {
            moves = realloc(moves, (*moveCount + 1) * sizeof(struct Move));
            moves[*moveCount].row = row;
            moves[*moveCount].column = column;
            (*moveCount)++;
        } else
            break;
    }

    // DOWN-LEFT DIAGONAL
    column = penguin->column;
    row = penguin->row;
    while (true) {
        down_left(&row, &column);
        if (!isValidPosition(state, row, column)) break;
        if (get_cell(state, row, column)->type == EMPTY) {
            moves = realloc(moves, (*moveCount + 1) * sizeof(struct Move));
            moves[*moveCount].row = row;
            moves[*moveCount].column = column;
            (*moveCount)++;
        } else
            break;
    }


    return moves;
}

// Initializes the game grid
void init_grid(GameState *state, int rows, int columns) {
    state->rows = rows;
    state->columns = columns;

    state->grid = (struct Cell *) malloc(rows * columns * sizeof(struct Cell));

    for (int i = 0; i < state->rows; ++i) {
        for (int j = 0; j < state->columns; ++j) {
            get_cell(state, i, j)->type = EMPTY;
            get_cell(state, i, j)->fishes = 0;
        }
    }

    // fill the grid with fishes
    int totalPenquines = state->playerCount * state->players[0].penguineCount;
    int cellWithOneFish = 0;

    do {
        for (int i = 0; i < state->rows; ++i) {
            for (int j = 0; j < state->columns; ++j) {  // Filling the grid with fishes as many times until a random grid  with fishes with enough
                                                         // one fish cells for each person
                int fishes = 1 + rand() % 3;
                get_cell(state, i, j)->fishes = fishes;
                if (fishes == 1)
                    cellWithOneFish++;
            }
        }
    } while (cellWithOneFish < totalPenquines); // make sure there are enough cells with one fish for each penguin

    // Setting up the penguins on the grid
    for (int i = 0; i < state->playerCount; ++i) {
        for (int j = 0; j < state->players[i].penguineCount; ++j) {
            int x, y;
            struct Cell *cell;
            do {
                y = rand() % state->rows;
                x = rand() % state->columns;
                cell = get_cell(state, y, x);
            } while (cell->fishes != 1 || cell->type != EMPTY);

            cell->type = OCCUPIED;
            state->players[i].penguins[j].row = y;
            state->players[i].penguins[j].column = x;
        }
    }

    int display_grid_size = (rows * 4 + 3) * (columns * 6 + 3);
    state->display_grid = (char *) malloc(display_grid_size * sizeof(char)); 
   
    //  Initialising the display grid with a hexagonal comb visual 
   
    for (int i = 0; i < display_grid_size; ++i) { 
        if (i % (columns * 6 + 3) == columns * 6 + 2)
            state->display_grid[i] = '\n';
        else
            state->display_grid[i] = ' ';
    }
    state->display_grid[display_grid_size - 1] = '\0';

    for (int i = 0; i <= rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            char *temp = xy_to_index_display_grid(state, state->display_grid, i, j, 2, 0);
            for (int k = 0; k < 4; ++k)
                temp[k] = '_';

            if (i < rows || (j % 2 == 0 && j > 0)) {
                *xy_to_index_display_grid(state, state->display_grid, i, j, 1, 1) = '/';
                *xy_to_index_display_grid(state, state->display_grid, i, j, 0, 2) = '/';
            }
            if (i < rows || (j % 2 == 0 && j < columns - 1)) {
                *xy_to_index_display_grid(state, state->display_grid, i, j, 6, 1) = '\\';
                *xy_to_index_display_grid(state, state->display_grid, i, j, 7, 2) = '\\';
            }

            if (i < rows) {
                if (j == 0) {
                    *xy_to_index_display_grid(state, state->display_grid, i, j, 0, 3) = '\\';
                    *xy_to_index_display_grid(state, state->display_grid, i, j, 1, 4) = '\\';
                } else if (j == columns - 1) {
                    *xy_to_index_display_grid(state, state->display_grid, i, j, 7, 3) = '/';
                    *xy_to_index_display_grid(state, state->display_grid, i, j, 6, 4) = '/';
                }
            }
        }
    }
}

// Initializes the players
void init_players(GameState *state, int playerCount) {
    state->playerCount = playerCount;
    state->players = (struct Player *) malloc(playerCount * sizeof(struct Player));

    for (int i = 0; i < playerCount; ++i) {
        state->players[i].id = i + 1;
        state->players[i].penguineCount = 0;
        state->players[i].penguins = NULL;
        state->players[i].score = 0;
        state->players[i].knockedOut = false;
    }

    // Setting up the penguins for each player
    int penguinePerPlayer;
    if (state->playerCount == 2)
        penguinePerPlayer = 4;
    else if (state->playerCount == 3)
        penguinePerPlayer = 3;
    else if (state->playerCount == 4)
        penguinePerPlayer = 2;
    else
        penguinePerPlayer = 1;

    struct Player *players = state->players;
    for (int i = 0; i < state->playerCount; ++i) {
        players[i].penguineCount = penguinePerPlayer;
        players[i].penguins = (struct Penguin *) malloc(penguinePerPlayer * sizeof(struct Penguin));
        for (int j = 0; j < penguinePerPlayer; ++j) {
            players[i].penguins[j].id = j + 1;
            players[i].penguins[j].owner = &players[i];
        }
    }
}

// Returns a pointer to the cell at the given row and column
struct Cell *get_cell(GameState *state, int row, int column) {
    return &state->grid[xy_to_index(state, row, column)];
}

// Display the game grid
void display_grid(GameState *state) {
    char *dup = strdup(state->display_grid);

    int fishCount = 0;
    for (int i = 0; i < state->rows; ++i) {
        for (int j = 0; j < state->columns; ++j) { // 2D transformation
            int fishes = get_cell(state, i, j)->fishes;

            if (fishes == 0) {
                for (int m = 2; m < 6; ++m)
                    for (int n = 1; n < 4; ++n)
                        *xy_to_index_display_grid(state, dup, i, j, m, n) = '*';
                *xy_to_index_display_grid(state, dup, i, j, 1, 2) = '*';
                *xy_to_index_display_grid(state, dup, i, j, 1, 3) = '*';
                *xy_to_index_display_grid(state, dup, i, j, 6, 2) = '*';
                *xy_to_index_display_grid(state, dup, i, j, 6, 3) = '*';
            } else {
                while (fishes != 0) {
                    int pos = rand() % 5;
                    // {2, 1}, {4, 1}, {3, 2}, {2, 3}, {4, 3}
                    int xs[] = {2, 4, 3, 2, 4};
                    int ys[] = {1, 1, 2, 3, 3};
                    int x = xs[pos];
                    int y = ys[pos];

                    char *cell = xy_to_index_display_grid(state, dup, i, j, x, y);
                    if (*cell == ' ') {
                        *cell = 'F';
                        fishCount++;
                        --fishes;
                    }
                }
            }
        }
    }

    int penguinCount = 0;
    for (int i = 0; i < state->playerCount; ++i) {
        for (int j = 0; j < state->players[i].penguineCount; ++j) {
            *xy_to_index_display_grid(state, dup, state->players[i].penguins[j].row,    ////// To display the screen there is 3 transformations - a grid comb that is mmade in the start
                                                                                             /// of the game when rows and colums are defined , as they will not be changed during the game       
                                                                                             /// The second transformation adds a penguine fishes to duplicated the first transformation.
                                                                                             /// Basically adding P and F to the required position. Fish and penguin emoji cannot directly be
                                                                                             // added as they are 4 bytes and characters are one . The solution is the 3rd transformation which we 
                                                                                             /// create a new grid but bigger in size in array to accomodate the emojies, Then we replace P and F with emojies
                                      state->players[i].penguins[j].column, 1, 2) = 'P';
            penguinCount++;
        }
    }

    int dup_len = strlen(dup);
    int advance_display_size = dup_len + (penguinCount + fishCount) * 2 + 1;      
    char *advance_display = malloc(advance_display_size);
    advance_display[advance_display_size - 1] = '\0';


    for (int i = 0, j = 0; i < dup_len; ++i, ++j) {
        if (dup[i] != 'F' && dup[i] != 'P')
            advance_display[j] = dup[i];
        else if (dup[i] == 'P') {
            advance_display[j] = '\xF0';
            advance_display[j + 1] = '\x9F';
            advance_display[j + 2] = '\x90';
            advance_display[j + 3] = '\xA7';
            i++;
            j += 3;
        } else if (dup[i] == 'F') {
            advance_display[j] = '\xF0';
            advance_display[j + 1] = '\x9F';
            advance_display[j + 2] = '\x90';
            advance_display[j + 3] = '\x9F';
            i++;
            j += 3;
        }
    }

    // Display the grid
    free(dup);
    puts(advance_display);
    free(advance_display);

    // Display the player scores
    printf("\n");
    color(WHITE);
    printf("%20s   %-8s Penguins\n", "Name", "Score");
    for (int i = 0; i < state->playerCount; ++i) {
        color(player_colors[i]);
        printf("%20s   %-8d ", state->players[i].name, state->players[i].score);
        for (int j = 0; j < state->players[i].penguineCount; ++j)
            printf("{%d,%d} ", state->players[i].penguins[j].column, state->players[i].penguins[j].row);
        printf("\n");
    }
    printf("\n");
}
