#include <limits>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdexcept>

#include "board.h"

/**
 * @brief Checks if the position is between 0-8 and corresponds to a free space on the board
 * 
 * @param position Position to be validated
 * @param b The current board's state
 * @return bool
 */
bool isValidInput(int position, Board b) {
    return position >= 0 && position < 9 && b.board[position].symbol == ' ';
}

int main() {
    Board b;

    for(int i = 0; i < 100; i++) {
        int position;
        b.drawBoard();

        // Get position (https://www.delftstack.com/howto/cpp/cpp-input-validation/)
        while (true) {
            std::cout << "Position (0-8): ";

            if (std::cin >> position && isValidInput(position, b)) {
                break;
            } else {
                printf("Invalid position. Please try again...\n");
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

        }

        const int moveResult = b.placePlayerToken(position);

        if(moveResult == -1) {
            printf("Invalid position. Please try again...\n");
            continue;
        } else if (moveResult == 0) {
            // It's a draw
            b.drawBoard();
            printf("Draw!\n");
            break;
        } else if (moveResult == 1) {
            b.drawBoard();
            printf("You win!\n");
            break;
        } else if (moveResult == 2) {
            b.drawBoard();
            printf("The computer wins!\n");
            break;
        } else if (moveResult == 200) {
            continue;
        } else {
            printf("Invalid moveResult: %d\nExiting...\n", moveResult);
        }
    }

    return EXIT_SUCCESS;
}