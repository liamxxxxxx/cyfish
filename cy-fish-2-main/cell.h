#ifndef CELL_H
#define CELL_H

// Enum for the type of a cell
enum CellType {
    EMPTY,      // The cell is empty
    OCCUPIED,   // The cell is occupied by a player
    MELTED      // The cell has melted and is no longer available
};

// Struct for a cell on the game board
struct Cell {
    int fishes;             // The number of fishes in the cell
    enum CellType type;     // The type of the cell (EMPTY, OCCUPIED, MELTED)
};

#endif //CELL_H