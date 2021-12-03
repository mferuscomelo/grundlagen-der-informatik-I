#include <stdio.h>
#include <string.h>

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
         * @param playerNum The number of the player (1 or 2). Determines the token symbol
         * @param position Where the token should be placed (1-9)
         * @return -1: Invalid move
         *          0: Draw
         *          1: Player 1 wins
         *          2: Player 2 wins
         *        200: Continue
         */
        int placeToken(int playerNum, int position) {
            const int row = position / 3;
            const int col = position % 3;

            if (board[row][col] != ' ') return -1;

            const char tokenChar = (playerNum == 1) ? 'X' : 'O';
            board[row][col] = tokenChar;

            return checkWin();
        }
    
    private:
        /**
         * @brief Checks result of a move
         * 
         * @return  0: Draw
         *          1: Player 1 wins
         *          2: Player 2 wins
         *        200: Player 2 wins
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
         * @brief Check if the board is full
         * 
         * @return boolean
         */
        bool isBoardFull() {
            for (int row = 0; row < 3; row++) {
                for (int col = 0; col < 3; col++) {
                    if (board[row][col] == ' ') return false;
                }
            }

            return true;
        }
};


int main() {
    Board b;
    int playerNum = 1, position;

    for(int i = 0; i < 100; i++) {
        b.drawBoard();

        printf("Player %d's turn: \n", playerNum);

        // Get position
        printf("Position: ");
        scanf("%d", &position);

        const int moveResult = b.placeToken(playerNum, position - 1);

        if(moveResult == -1) {
            printf("Invalid position. Please try again...\n");
            continue;
        } else if (moveResult == 0) {
            // It's a draw
            b.drawBoard();
            printf("Draw!\n");
            break;
        } else if (moveResult == 1 || moveResult == 2) {
            // A player has won
            b.drawBoard();
            printf("Player %d wins!\n", moveResult);
            break;
        } else if (moveResult == 200) {
            playerNum = (playerNum == 1) ? 2 : 1; // Change player number
        } else {
            printf("Invalid moveResult: %d\nExiting...\n", moveResult);
        }

    }

    return 0;
}