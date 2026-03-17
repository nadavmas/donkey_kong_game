#ifndef location_h
#define location_h

// The location struct is used to store the x and y coordinates of an object on the board
// It helps represent the position of various elements like Mario, Donkey Kong, Princess, etc.
struct location {
    int x = -1; // x-coordinate of the location (default value is -1, which indicates an uninitialized position)
    int y = -1; // y-coordinate of the location (default value is -1, which indicates an uninitialized position)
};

#endif 