#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cell.h"
#include "color.h"
#include "game_state.h"
#include "penguin.h"
#include "player.h"
#include "valid_input.h"

/* VARIANT
 * START_AUTO
 * HEXA_A
 * FISH_NORMAL
 */

int main(void) {
    // Seed the random number generator
    srand(time(NULL));

    // Print the game title

    color(YELLOW BOLD);
    printf( "+****-------****+\n");
    printf(">->> CY-Fish <<-<\n");
    printf("+****-------****+\n");

    printf("\n\n");
    // Initialize the game state
    GameState state;
    state.currentPlayer = 0;
    state.round = 0;

    // Print the player information header
    color(YELLOW);
    printf("******************\n");
    printf("PLAYER INFORMATION\n");
    printf("******************\n");

    // Get the number of players
    int playerCount = validInputInt("Enter the number of players: ", "No. of players could be between 2 and 6.", 2, 6);

    // Initialize the players
    init_players(&state, playerCount);

    // Get the names of the players
    struct Player *players = state.players;
    char prompt[50];
    for (int i = 0; i < state.playerCount; i++) {
        sprintf(prompt, "Enter name of player %d: ", i + 1);
        validInputString(prompt, "Name should be at least 1 character long.", players[i].name, sizeof(players[i].name));
    }

    // Print the grid information header
    color(YELLOW);
    printf("****************\n");
    printf("GRID INFORMATION\n");
    printf("****************\n");

    // Get the number of rows and columns for the grid
    int rows = validInputInt("Enter the number of rows: ", "Rows should be atleast 9.", 9, INT32_MAX);
    int columns = validInputInt("Enter the number of columns: ", "Columns should be atleast 9.", 9, INT32_MAX);

    // Initialize the grid
    init_grid(&state, rows, columns);

    // Print the game play header
    color(YELLOW);
    printf("****************\n");
    printf("   GAME PLAY\n");
    printf("****************\n");

    // Main game loop
    bool isEnd = false;
    while (!isEnd) {
        // Check if all players are knocked out
        bool allKnockedOut = true;
        for (int i = 0; i < state.playerCount; ++i) {
            if (!state.players[i].knockedOut) {
                allKnockedOut = false;
                break;
            }
        }

        // If all players are knocked out, end the game
        if (allKnockedOut) {
            color(PURPLE);
            printf("All players are knocked out.\n");
            isEnd = true;
            continue;
        }

        // If the current player is knocked out, skip to the next player
        if (state.players[state.currentPlayer].knockedOut) {
            next_player(&state);
            continue;
        }

        // Display the game grid
        color(CYAN);
        display_grid(&state);

        // Get the current player
        struct Player *currentPlayer = &state.players[state.currentPlayer];
        color(player_colors[state.currentPlayer]);
        printf("*******************\n");
        printf("%s Turn\n", currentPlayer->name);
        printf("*******************\n");

        // Get the valid moves for each penguin
        struct Move *moves[currentPlayer->penguineCount];
        int moveCount[currentPlayer->penguineCount];
        for (int i = 0; i < currentPlayer->penguineCount; ++i) {
            moves[i] = NULL;
            moveCount[i] = 0;
        }

        for (int i = 0; i < currentPlayer->penguineCount; ++i)
            moves[i] = get_penguin_valid_moves(&state, &currentPlayer->penguins[i], &moveCount[i]);

        // Count the total number of valid moves
        int totalMoves = 0;
        for (int i = 0; i < currentPlayer->penguineCount; ++i)
            totalMoves += moveCount[i];

        // If there are no valid moves, knock out the player
        if (totalMoves == 0) {
            printf("All of your penguines are stuck.\n");
            printf("You are knocked out.\n");
            currentPlayer->knockedOut = true;
            for (int i = 0; i < currentPlayer->penguineCount; ++i)
                free(moves[i]);
            next_player(&state);
            continue;
        }

        // Prompt the player to select a penguin
        color(YELLOW);
        printf("SELECT ONE OF YOUR PENGUINE\n");

        for (int i = 0; i < currentPlayer->penguineCount; ++i) {
            if (moveCount[i] > 0)
                printf("%d. Penguine {%d,%d}\n", i + 1, currentPlayer->penguins[i].column,
                       currentPlayer->penguins[i].row);
            else
                printf("%d. Penguine {%d,%d} (No moves available)\n", i + 1, currentPlayer->penguins[i].column,
                       currentPlayer->penguins[i].row);
        }

        // Get the selected penguin
        int penguinIndex;
        do {
            penguinIndex = validInputInt("Select one: ", "Invalid penguin selection.", 1,
                                         currentPlayer->penguineCount) - 1;
            if (moveCount[penguinIndex] == 0)
                printf("No moves available for this penguin.\n");
        } while (moveCount[penguinIndex] == 0);

        // Prompt the player to select a cell to move to
        color(YELLOW);
        printf("SELECT A CELL TO MOVE\n");

        int row, column;
        bool isValid = false;
        do {
            column = validInputInt("Enter column: ", "Invalid column.", 0, state.columns - 1);
            row = validInputInt("Enter row: ", "Invalid row.", 0, state.rows - 1);

            // Check if the selected cell is a valid move
            for (int i = 0; i < moveCount[penguinIndex]; ++i) {
                if (moves[penguinIndex][i].row == row && moves[penguinIndex][i].column == column) {
                    isValid = true;
                    break;
                }
            }
            if (!isValid) {
                color(RED BOLD);
                printf("Invalid move.\n");
            }
        } while (!isValid);

        // Free the memory allocated for the moves
        for (int i = 0; i < currentPlayer->penguineCount; ++i)
            free(moves[i]);

        // Get the selected penguin and cell
        struct Penguin *penguin = &currentPlayer->penguins[penguinIndex];
        struct Cell *toCell = get_cell(&state, row, column);
        struct Cell *fromCell = get_cell(&state, penguin->row, penguin->column);

        // Update the player's score and the cell's state
        currentPlayer->score += fromCell->fishes;
        fromCell->fishes = 0;
        fromCell->type = MELTED;

        // Move the penguin to the selected cell
        toCell->type = OCCUPIED;
        penguin->row = row;
        penguin->column = column;

        color(BLUE);
        printf("Penguin moved to {%d,%d}\n", column, row);

        // Advance to the next player
        next_player(&state);
    }

    // Print the scores of all players
    printf("\n\n");
    for (int i = 0; i < state.playerCount; ++i) {
        color(player_colors[i]);
        printf("%s scored %d\n", state.players[i].name, state.players[i].score);
    }

    // Determine the winner(s)
    int maxScore = 0;
    int winners[state.playerCount]; // array to store the indices of the winners
    int winnerCount = 0; // count of winners

    printf("\n\n");
    for (int i = 0; i < state.playerCount; ++i) {
        if (state.players[i].score > maxScore) {
            maxScore = state.players[i].score;
            winners[0] = i; // new maximum found, so overwrite the winners array
            winnerCount = 1; // reset the winner count
        } else if (state.players[i].score == maxScore) {
            winners[winnerCount] = i; // add the player to the winners array
            winnerCount++; // increment the winner count
        }
    }

    // Print the winner(s)
    color(MAGENTA);
    if (winnerCount == 1) {
        printf("The winner is %s\n", state.players[winners[0]].name);
    } else {
        printf("Its a tie between ");
        for (int i = 0; i < winnerCount; ++i) {
            printf("%s", state.players[winners[i]].name);
            if (i < winnerCount - 2)
                printf(", ");
            else if (i < winnerCount - 1)
                printf(" and ");
        }
        printf("\n");
    }

    // Print the game end message
    color(PURPLE);
    printf("\n\nTHANK YOU FOR PLAYING\n\n");

    // Free the memory allocated for the game state
    destroy_state(&state);

    return 0;
}
