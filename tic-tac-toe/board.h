#include "colors.h"
#include "algorithm.h"

#include <vector>
#include <iostream>

class Board {
    public:
        Token board[9];
        Algorithm algorithm;

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
         * @param position Where the token should be placed (0-8)
         * @return -1: Invalid move
         *          0: Draw
         *          1: Player 1 wins
         *          2: Computer wins
         *        200: Continue
         */
        int placePlayerToken(int position, int playerNum) {
            // TODO: can throw error for invalid position
            if (board[position].symbol != ' ') return -1;

            ++currentTurn; // Increment currentTurn counter

            const char tokenSymbol = (playerNum == 1) ? 'X' : 'O';
            board[position].setSymbol(tokenSymbol); // Place token

            if (firstMovePosition == -1) {
                firstMovePosition = position; // Update firstMovePosition
            }

            return checkWin();
        }

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
            int postion = algorithm.getBestPosition(board, ++currentTurn, firstMovePosition);
            board[postion].setSymbol('O');
            return checkWin();
        }

        std::vector<int> getFreeSpaces() {
            std::vector<int> freeSpaces;

            for (int i = 0; i < 9; i++) {
                if (board[i].symbol == ' ') freeSpaces.push_back(i);
            }

            return freeSpaces;
        }
    
    private:
        int firstMovePosition = -1;
        int currentTurn = 0;

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

        /**
         * @brief If a player has won, highlights the winning tokens in red and turns the rest of the tokens to white. 
         * 
         * @param pos1 The first position of the winning tokens
         * @param pos2 The second position of the winning tokens
         * @param pos3 The third position of the winning tokens
         */
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
         * @brief Check if the board is full
         * 
         * @return bool
         */
        bool isBoardFull() {
            for (Token position : board) {
                if (position.symbol == ' ') return false;
            }

            return true;
        }
};