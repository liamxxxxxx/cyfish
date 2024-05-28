#ifndef COLOR_H
#define COLOR_H

#define RESET "\033[0m"
#define BLACK "\033[30m"      /* Black */
#define RED "\033[31m"        /* Red */
#define GREEN "\033[32m"      /* Green */
#define YELLOW "\033[33m"     /* Yellow */
#define BLUE "\033[34m"       /* Blue */
#define MAGENTA "\033[35m"    /* Magenta */
#define CYAN "\033[36m"       /* Cyan */
#define WHITE "\033[37m"      /* White */
#define BOLD "\033[1m"        /* Bold */
#define PURPLE "\033[0;35m"   /* Purple */

// 6 different color for different players
#define PLAYER1 "\033[0;31m"  /* Red */
#define PLAYER2 "\033[0;32m"  /* Green */
#define PLAYER3 "\033[0;33m"  /* Yellow */
#define PLAYER4 "\033[0;34m"  /* Blue */
#define PLAYER5 "\033[0;35m"  /* Magenta */
#define PLAYER6 "\033[0;36m"  /* Cyan */

// Array of colors for the players
extern char *player_colors[];

// Function to change the color of the text
void color(const char *color);

#endif //COLOR_H
