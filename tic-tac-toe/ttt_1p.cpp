#include <limits>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdexcept>

// https://stackoverflow.com/questions/9158150/colored-output-in-c
#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

#define BOLDBLACK   "\033[1m\033[30m"
#define BOLDRED     "\033[1m\033[31m"
#define BOLDGREEN   "\033[1m\033[32m"
#define BOLDYELLOW  "\033[1m\033[33m"
#define BOLDBLUE    "\033[1m\033[34m"
#define BOLDMAGENTA "\033[1m\033[35m"
#define BOLDCYAN    "\033[1m\033[36m"
#define BOLDWHITE   "\033[1m\033[37m"

class Token {
    public:
        char symbol;
        std::string color;

        Token() {
            symbol = ' ';
            color = RESET;
        }

        void setSymbol(char s) {
            symbol = s;
            color = (s == 'X') ? YELLOW : BLUE;
        }
};

class Board {
    public:
        Token board[9];

        /**
         * @brief Construct a new Board object
         * 
         */
        Board() {
            // memset(board, ' ', sizeof(char) * 3 * 3); // Initialize the board with spaces
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
                    const int pos = (3 * row) + col;
                    Token token = board[pos];
                    std::cout << token.color << "  " << token.symbol << "  " << RESET; // Print token with color and horizontal padding

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
            if (board[position].symbol != ' ') return -1;

            ++currentTurn; // Increment currentTurn counter
            board[position].setSymbol('X'); // Place token

            if (firstMovePosition == -1) {
                firstMovePosition = position; // Update firstMovePosition
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
            int winningPos = checkAboutToWin('O');
            if (winningPos != -1) {
                board[winningPos].setSymbol('O');
                hasPlacedToken = true;
                return checkWin();
            }

            // Blocking
            int blockingPos = checkAboutToWin('X');
            if (blockingPos != -1) {
                board[blockingPos].setSymbol('O');
                hasPlacedToken = true;
                return checkWin();
            }

            // If the player has placed in a corner, place in center
            if ((board[0].symbol == 'X' || board[2].symbol == 'X' || board[6].symbol == 'X' || board[8].symbol == 'X') && currentTurn == 2) {
                board[4].setSymbol('O');
                hasPlacedToken = true;
            } else if (currentTurn > 2 && firstMovePosition % 2 == 0 && firstMovePosition != 4) {
                // Place token on an edge
                if      (board[1].symbol == ' ')    board[1].setSymbol('O');
                else if (board[3].symbol == ' ')    board[3].setSymbol('O');
                else if (board[5].symbol == ' ')    board[5].setSymbol('O');
                else if (board[7].symbol == ' ')    board[7].setSymbol('O');
                else                            placeTokenRandomly();

                hasPlacedToken = true;
            }

            // If the player has placed in the center, place in corner
            if (board[4].symbol == 'X' && currentTurn == 2) {
                board[0].setSymbol('O');
                hasPlacedToken = true;
            }

            // If the player has placed on the edge, place in center
            if ((board[1].symbol == 'X' || board[3].symbol == 'X' || board[5].symbol == 'X' || board[7].symbol == 'X') && currentTurn == 2) {
                board[4].setSymbol('O');
                hasPlacedToken = true;
            } else if (currentTurn == 4 && firstMovePosition % 2 == 1) {
                // If the player has placed their token so that it forms XOX (vertically or horizontally), place token in corner
                if (
                    (board[3].symbol == 'X' && board[5].symbol == 'X') ||   // Horizontal pattern placed
                    (board[1].symbol == 'X' && board[7].symbol == 'X')      // Vertical pattern placed
                    ) {
                    board[0].setSymbol('O'); // Place in corner
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
                int pos1 = (3 * i);
                int pos2 = (3 * i) + 1;
                int pos3 = (3 * i) + 2;

                Token t1 = board[pos1];
                Token t2 = board[pos2];
                Token t3 = board[pos3];

                if (t1.symbol == t2.symbol && t2.symbol == t3.symbol && t1.symbol != ' ') {
                    highlightWinningPositions(pos1, pos2, pos3);
                    return (t1.symbol == 'X') ? 1 : 2;
                }
            }

            // Check all the columns
            for (int i = 0; i < 3; i++) {
                int pos1 = i;
                int pos2 = i + 3;
                int pos3 = i + 6;

                Token t1 = board[pos1];
                Token t2 = board[pos2];
                Token t3 = board[pos3];

                if (t1.symbol == t2.symbol && t2.symbol == t3.symbol && t1.symbol != ' ') {
                    highlightWinningPositions(pos1, pos2, pos3);
                    return (t1.symbol == 'X') ? 1 : 2;
                }
            }

            // Check diagonals
            if (board[0].symbol == board[4].symbol && board[4].symbol == board[8].symbol && board[0].symbol != ' ') {
                highlightWinningPositions(1, 5, 9);
                return (board[0].symbol == 'X') ? 1 : 2;
            } else if (board[2].symbol == board[4].symbol && board[4].symbol == board[6].symbol && board[2].symbol != ' ') {
                highlightWinningPositions(3, 5, 7);
                return (board[2].symbol == 'X') ? 1 : 2;
            }

            return 200;
        }

        void highlightWinningPositions(int pos1, int pos2, int pos3) {
            for (int currentPos = 0; currentPos < 9; currentPos++) {
                if (currentPos == pos1 || currentPos == pos2 || currentPos == pos3) {
                    board[currentPos].color = RED;
                } else {
                    board[currentPos].color = RESET;
                }
            }
        }

        /**
         * @brief Checks if there is a possibility to win on the board
         * 
         * @param tokenSymbol The token to look for patterns with
         * @return The row and column to win. If none available, returns (-1, -1)
         */
        int checkAboutToWin(char tokenSymbol) {
            // Check all the rows
            for (int i = 0; i < 3; i++) {
                int pos1 = (3 * i);
                int pos2 = (3 * i) + 1;
                int pos3 = (3 * i) + 2;

                Token t1 = board[pos1];
                Token t2 = board[pos2];
                Token t3 = board[pos3];

                if (t1.symbol == tokenSymbol && t2.symbol == tokenSymbol && t3.symbol == ' ') {
                    return pos3;
                } else if (t1.symbol == tokenSymbol && t2.symbol == ' ' && t3.symbol == tokenSymbol) {
                    return pos2;
                } else if (t1.symbol == ' ' && t2.symbol == tokenSymbol && t3.symbol == tokenSymbol) {
                    return pos1;
                }
            }

            // Check all the columns
            for (int i = 0; i < 3; i++) {
                int pos1 = i;
                int pos2 = i + 3;
                int pos3 = i + 6;

                Token t1 = board[pos1];
                Token t2 = board[pos2];
                Token t3 = board[pos3];

                if (t1.symbol == tokenSymbol && t2.symbol == tokenSymbol && t3.symbol == ' ') {
                    return pos3;
                } else if (t1.symbol == tokenSymbol && t2.symbol == ' ' && t3.symbol == tokenSymbol) {
                    return pos2;
                } else if (t1.symbol == ' ' && t2.symbol == tokenSymbol && t3.symbol == tokenSymbol) {
                    return pos1;
                }
            }

            // Check the diagonals (LTR)
            if (board[0].symbol == tokenSymbol && board[4].symbol == tokenSymbol && board[8].symbol == ' ') {
                return 8;
            } else if (board[0].symbol == tokenSymbol && board[4].symbol == ' ' && board[8].symbol == tokenSymbol) {
                return 4;
            } else if (board[0].symbol == ' ' && board[4].symbol == tokenSymbol && board[8].symbol == tokenSymbol) {
                return 0;
            }

            // Check the diagonals (RTL)
            if (board[2].symbol == tokenSymbol && board[4].symbol == tokenSymbol && board[6].symbol == ' ') {
                return 6;
            } else if (board[2].symbol == tokenSymbol && board[4].symbol == ' ' && board[6].symbol == tokenSymbol) {
                return 4;
            } else if (board[2].symbol == ' ' && board[4].symbol == tokenSymbol && board[6].symbol == tokenSymbol) {
                return 2;
            }

            return -1;
        }

        /**
         * @brief Check if the board is full
         * 
         * @return bool
         */
        bool isBoardFull() {
            for (int pos = 0; pos < 9; pos++) {
                if (board[pos].symbol == ' ') return false;
            }

            return true;
        }

        /**
         * @brief Places a token in the first available space
         * 
         */
        bool placeTokenRandomly() {
            for (int pos = 0; pos < 9; pos++) {
                if (board[pos].symbol == ' ') {
                    board[pos].setSymbol('O');
                    return true;
                }
            }

            return false;
        }
};

/**
 * @brief Checks if the position is between 1-9 and corresponds to a free space on the board
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