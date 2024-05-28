#include "color.h"

#include <stdio.h>

// Define the player colors
char *player_colors[] = {PLAYER1, PLAYER2, PLAYER3, PLAYER4, PLAYER5, PLAYER6};

// Function to change the color of the text
void color(const char *color) {
    printf("%s",color);
}
