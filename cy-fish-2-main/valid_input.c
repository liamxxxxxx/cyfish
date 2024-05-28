#include "valid_input.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "color.h"

// Function to validate integer input within a range
int validInputInt(const char *prompt, const char *error, int min, int max) {
    int input;
    while (true) {
        color(BOLD GREEN);
        printf("%s", prompt);
        // If the input is not an integer or is out of range, display an error message
        color(BLUE);
        if (scanf("%d", &input) != 1 || input < min || input > max) {
            color(RED BOLD);
            printf("%s\n", error);
            // Clear the input buffer
            while (getchar() != '\n');
        } else
            break;
    }
    color(RESET);
    // Clear the input buffer
    while (getchar() != '\n');
    return input;
}

// Function to validate string input
void validInputString(const char *prompt, const char *error, char *buffer, int size) {
    while (true) {
        color(BOLD GREEN);
        printf("%s", prompt);
        color(BLUE);
        fgets(buffer, size, stdin);

        // If the input is an empty string, display an error message
        if (buffer[0] == '\n') {
            color(RED BOLD);
            printf("%s\n", error);
        } else
            break;
    }
    // Remove the trailing newline character from the input string
    buffer[strcspn(buffer, "\n")] = 0;
    color(RESET);
}
