#include <limits>
#include <stdio.h>
#include <string>
#include <iostream>
#include <functional>

#include "board.h"

bool isValidInput(int input, std::vector<int> validInputs) {
    for (int validInput : validInputs) {
        if (validInput == input) return true;
    }

    return false;
}

/**
 * @brief Prompts the user until a valid input is received
 * 
 * @param prompt The string to display to the user, prompting them for input
 * @param errorMessage The string to display if the input is invalid
 * @param validInputs A vector of valid inputs
 * @return input 
 */
int getValidInput(std::string prompt, std::string errorMessage, std::vector<int> validInputs) {
    // (https://www.delftstack.com/howto/cpp/cpp-input-validation/)
    int input;
    while (true) {
        std::cout << prompt;

        if (std::cin >> input && isValidInput(input, validInputs)) {
            break;
        } else {
            std::cout << errorMessage << "\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

    }
    return input;
}

int main(int argc, char *argv[]) {
    std::vector<int> validModeInputs = { 1, 2 };
    std::string argv1 = argv[1] ? argv[1] : "";

    int mode;
    if (argv1 != "1p" && argv1 != "2p") {
        mode = getValidInput(
            "Number of players (1 or 2): ", 
            "Invalid input. Please try again...", 
            validModeInputs
        );
    } else {
        mode = argv1[0] - '0'; // https://stackoverflow.com/questions/628761/convert-a-character-digit-to-the-corresponding-integer-in-c
    }

    printf("%d Player mode selected...\n", mode);

    Board b;
    int playerNum = 1;

    while (true) {
        b.drawBoard();

        int moveResult;
        if (mode == 2 || playerNum == 1) {
            printf("Player %d's turn: \n", playerNum);

            // Get position 
            int position = getValidInput(
                "Position (0-8): ",
                "Invalid position. Please try again...\n",
                b.getFreeSpaces()
            );

            moveResult = b.placePlayerToken(position, playerNum);
        } else if (mode == 1 && playerNum == 2) {
            moveResult = b.placeComputerToken();
        }

        if(moveResult == -1) {
            printf("Invalid position. Please try again...\n");
            continue;
        } else if (moveResult == 0) {
            // It's a draw
            b.drawBoard();
            printf("Draw!\n");
            break;
        } else if (moveResult == 1 || moveResult == 2) {
            b.drawBoard();

            if (mode == 1) {
                if (moveResult == 1)    printf("You win!\n");
                else                    printf("The computer wins!\n");
            } else {
                printf("Player %d wins!\n", playerNum);
            }

            break;
        } else if (moveResult == 200) {
            playerNum = (playerNum == 1) ? 2 : 1; // Change player number
            continue;
        } else {
            printf("Invalid moveResult: %d\nExiting...\n", moveResult);
        }
    }

    return EXIT_SUCCESS;
}