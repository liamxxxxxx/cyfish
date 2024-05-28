#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

// Function to make valid integer input within a range
// prompt: The message to display to the user
// error: The error message to display if the input is invalid
// min: The minimum acceptable value
// max: The maximum acceptable value
// Returns: The validated integer input from the user
int validInputInt(const char *prompt, const char *error, int min, int max);

// Function to make valid string input
// prompt: The message to display to the user
// error: The error message to display if the input is invalid
// buffer: The buffer to store the input string
// size: The size of the buffer
void validInputString(const char *prompt, const char *error, char *buffer, int size);

#endif //USER_INTERFACE_H