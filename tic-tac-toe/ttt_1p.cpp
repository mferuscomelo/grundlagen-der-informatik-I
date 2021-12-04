#include <tuple>
#include <limits>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdexcept>

class Board {
    public:
        char board[3][3];

        /**
         * @brief Construct a new Board object
         * 
         */
        Board() {
            memset(board, ' ', sizeof(char) * 3 * 3); // Initialize the board with spaces
        }

        /**
         * @brief Draws the board with the pieces
         * 
         */
        void drawBoard() {
            printf("\n\n\n");
            // Iterate through the board's rows and columns
            for(int row = 0; row < 3; row++) {
                printf("     |     |     \n"); // Add horizontal padding

                for(int col = 0; col < 3; col++) {
                    printf("  %c  ", board[row][col]); // Print with horizontal padding

                    col < 2 ? printf("|") : printf("\n"); // Add horizontal divider
                }

                row < 2 ? printf("_____|_____|_____\n") : printf("     |     |     \n"); // Add vertical padding and vertical line unless in last row
            }
            printf("\n");  
        }

        /**
         * @brief Places a player token
         * 
         * @param position Where the token should be placed (1-9)
         * @return -1: Invalid move
         *          0: Draw
         *          1: Player 1 wins
         *          2: Computer wins
         *        200: Continue
         */
        int placePlayerToken(int position) {
            // TODO: can throw error for invalid position
            const int row = (position - 1) / 3;
            const int col = (position - 1) % 3;

            if (board[row][col] != ' ') return -1;

            ++currentTurn; // Increment currentTurn counter
            board[row][col] = 'X'; // Place token

            if (firstMovePosition == -1) {
                firstMovePosition = position; // Update firstMovePosition
                // if (position == 1 || position == 3 || position == 7 || position == 9) {
                //     firstMovePosition = 0; // Token placed in a corner
                // } else if (position == 5) {
                //     firstMovePosition = 1; // Token placed in the center
                // } else if (position == 2 || position == 4 || position == 6 || position == 8) {
                //     firstMovePosition = 2; // Token placed in the center
                // }
            }

            const int moveResult = checkWin();

            if (moveResult != 200) return moveResult;

            return placeComputerToken();
        }
    
    private:
        int firstMovePosition = -1;
        int currentTurn = 0;

        /**
         * @brief Algorithm to place the computer's token
         * 
         * @return -1: Invalid move
         *          0: Draw
         *          1: Player 1 wins
         *          2: Computer wins
         *        200: Continue
         */
        int placeComputerToken() {
            // Tips taken from here: https://www.wikihow.com/Win-at-Tic-Tac-Toe
            ++currentTurn;

            bool hasPlacedToken = false;

            // Winning
            int row, col;
            std::tie (row, col) = checkAboutToWin('O');
            if (row != -1 && col != -1) {
                board[row][col] = 'O';
                hasPlacedToken = true;
                return checkWin();
            }

            // Blocking
            std::tie (row, col) = checkAboutToWin('X');
            if (row != -1 && col != -1) {
                board[row][col] = 'O';
                hasPlacedToken = true;
                return checkWin();
            }

            // If the player has placed in a corner, place in center
            if ((board[0][0] == 'X' || board[0][2] == 'X' || board[2][0] == 'X' || board[2][2] == 'X') && currentTurn == 2) {
                board[1][1] = 'O';
                hasPlacedToken = true;
            } else if (currentTurn > 2 && firstMovePosition % 2 == 1 && firstMovePosition != 5) {
                // Place token on an edge
                if      (board[0][1] == ' ')    board[0][1] = 'O';
                else if (board[1][0] == ' ')    board[1][0] = 'O';
                else if (board[1][2] == ' ')    board[1][2] = 'O';
                else if (board[2][1] == ' ')    board[2][1] = 'O';
                else                            placeTokenRandomly();

                hasPlacedToken = true;
            }

            // If the player has placed in the center, place in corner
            if (board[1][1] == 'X' && currentTurn == 2) {
                board[0][0] = 'O';
                hasPlacedToken = true;
            }

            // If the player has placed on the edge, place in center
            if ((board[0][1] == 'X' || board[1][0] == 'X' || board[1][2] == 'X' || board[2][1] == 'X') && currentTurn == 2) {
                board[1][1] = 'O';
                hasPlacedToken = true;
            } else if (currentTurn == 4 && firstMovePosition % 2 == 0) {
                // If the player has placed their token so that it forms XOX (vertically or horizontally), place token in corner
                if (
                    (board[1][0] == 'X' && board[1][2] == 'X') ||   // Horizontal pattern placed
                    (board[0][1] == 'X' && board[2][1] == 'X')      // Vertical pattern placed
                    ) {
                    board[0][0] = 'O'; // Place in corner
                    hasPlacedToken = true;
                }
            }

            if (!hasPlacedToken) {
                placeTokenRandomly();
            }

            return checkWin();
        }

        /**
         * @brief Checks result of a move
         * 
         * @return  0: Draw
         *          1: Player 1 wins
         *          2: Computer wins
         *        200: Continue
         */
        int checkWin() {
            // Check if a draw
            if (isBoardFull()) return 0;

            // Check all the rows
            for (int i = 0; i < 3; i++) {
                if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ') {
                    return (board[i][0] == 'X') ? 1 : 2;
                }
            }

            // Check all the columns
            for (int i = 0; i < 3; i++) {
                if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ') {
                    return (board[0][i] == 'X') ? 1 : 2;
                }
            }

            // Check diagonals
            if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ') {
                return (board[0][0] == 'X') ? 1 : 2;
            } else if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ') {
                return (board[0][2] == 'X') ? 1 : 2;
            }

            return 200;
        }

        /**
         * @brief Checks if there is a possibility to win on the board
         * 
         * @param tokenSymbol The token to look for patterns with
         * @return The row and column to win. If none available, returns (-1, -1)
         */
        std::tuple<int, int> checkAboutToWin(char tokenSymbol) {
            // Check all the rows
            for (int i = 0; i < 3; i++) {
                if (board[i][0] == tokenSymbol && board[i][1] == tokenSymbol && board[i][2] == ' ') {
                    return { i, 2 };
                } else if (board[i][0] == tokenSymbol && board[i][1] == ' ' && board[i][2] == tokenSymbol) {
                    return { i, 1 };
                } else if (board[i][0] == ' ' && board[i][1] == tokenSymbol && board[i][2] == tokenSymbol) {
                    return { i, 0 };
                }
            }

            // Check all the columns
            for (int i = 0; i < 3; i++) {
                if (board[0][i] == tokenSymbol && board[1][i] == tokenSymbol && board[2][i] == ' ') {
                    return { 2, i };
                } else if (board[0][i] == tokenSymbol && board[1][i] == ' ' && board[2][i] == tokenSymbol) {
                    return { 1, i };
                } else if (board[0][i] == ' ' && board[1][i] == tokenSymbol && board[2][i] == tokenSymbol) {
                    return { 0, i };
                }
            }

            // Check the diagonals (LTR)
            if (board[0][0] == tokenSymbol && board[1][1] == tokenSymbol && board[2][2] == ' ') {
                return { 2, 2 };
            } else if (board[0][0] == tokenSymbol && board[1][1] == ' ' && board[2][2] == tokenSymbol) {
                return { 1, 1 };
            } else if (board[0][0] == ' ' && board[1][1] == tokenSymbol && board[2][2] == tokenSymbol) {
                return { 0, 0 };
            }

            // Check the diagonals (RTL)
            if (board[0][2] == tokenSymbol && board[1][1] == tokenSymbol && board[2][0] == ' ') {
                return { 2, 0 };
            } else if (board[0][2] == tokenSymbol && board[1][1] == ' ' && board[2][0] == tokenSymbol) {
                return { 1, 1 };
            } else if (board[0][2] == ' ' && board[1][1] == tokenSymbol && board[2][0] == tokenSymbol) {
                return { 0, 2 };
            }

            return { -1, -1 };
        }

        /**
         * @brief Check if the board is full
         * 
         * @return bool
         */
        bool isBoardFull() {
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    if (board[row][col] == ' ') return false;
                }
            }

            return true;
        }

        /**
         * @brief Places a token in the first available space
         * 
         */
        bool placeTokenRandomly() {
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    if (board[row][col] == ' ') {
                        board[row][col] = 'O';
                        return true;
                    }
                }
            }

            return false;
        }
};


int main() {
    Board b;

    for(int i = 0; i < 100; i++) {
        int position;
        b.drawBoard();

        // Get position (https://www.delftstack.com/howto/cpp/cpp-input-validation/)
        while (true) {
            std::cout << "Position (1-9): ";

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

/**
 * @brief Checks if the position is between 1-9 and corresponds to a free space on the board
 * 
 * @param position Position to be validated
 * @param b The current board's state
 * @return bool
 */
bool isValidInput(int position, Board b) {
    return position > 0 && position < 10 && b.board[(position - 1) / 3][(position - 1) % 3] == ' ';
}