#include "token.h"
#include <stdexcept>

// Tips taken from here: https://www.wikihow.com/Win-at-Tic-Tac-Toe

class Algorithm {
    public:
        /**
         * @brief Get the best position to play
         * 
         * @param board The current state of the board
         * @param currentTurn The current turn
         * @param firstMovePosition The player's first move
         * @return int 
         */
        int getBestPosition(Token board[9], int currentTurn, int firstMovePosition) {
            int bestPosition = -1;

            // Winning
            int winningPos = checkAboutToWin(board, 'O');
            if (winningPos != -1) return winningPos;

            // Blocking
            int blockingPos = checkAboutToWin(board, 'X');
            if (blockingPos != -1) return blockingPos;

            // If the player has placed in a corner, place in center
            if ((board[0].symbol == 'X' || board[2].symbol == 'X' || board[6].symbol == 'X' || board[8].symbol == 'X') && currentTurn == 2) {
                return 4;
            } else if (currentTurn > 2 && firstMovePosition % 2 == 0 && firstMovePosition != 4) {
                // Place token on an edge
                if      (board[1].symbol == ' ')    return 1;
                else if (board[3].symbol == ' ')    return 3;
                else if (board[5].symbol == ' ')    return 5;
                else if (board[7].symbol == ' ')    return 7;
                else                                return getRandomPosition(board);
            }

            // If the player has placed in the center, place in corner
            if (board[4].symbol == 'X' && currentTurn == 2) {
                return 0;
            }

            // If the player has placed on the edge, place in center
            if ((board[1].symbol == 'X' || board[3].symbol == 'X' || board[5].symbol == 'X' || board[7].symbol == 'X') && currentTurn == 2) {
                return 4;
            } else if (currentTurn == 4 && firstMovePosition % 2 == 1) {
                // If the player has placed their token so that it forms XOX (vertically or horizontally), place token in corner
                if (
                    (board[3].symbol == 'X' && board[5].symbol == 'X') ||   // Horizontal pattern placed
                    (board[1].symbol == 'X' && board[7].symbol == 'X')      // Vertical pattern placed
                    ) {
                    return 0;
                }
            }

            return getRandomPosition(board);
        }

    private: 
        /**
         * @brief Checks if there is a possibility to win on the board
         * 
         * @param tokenSymbol The token to look for patterns with
         * @return The position to win. If none available, returns -1
         */
        int checkAboutToWin(Token board[9], char tokenSymbol) {
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
         * @brief Places a token in the first available space
         * 
         */
        int getRandomPosition(Token board[9]) {
            for (int pos = 0; pos < 9; pos++) {
                if (board[pos].symbol == ' ') {
                    return pos;
                }
            }

            throw std::runtime_error("Board is full. Can't select an empty location.");
        }
};