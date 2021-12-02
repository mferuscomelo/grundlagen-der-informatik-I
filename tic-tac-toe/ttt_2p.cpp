#include <stdio.h>
#include <string.h>

class Board {
    public:
        char board[3][3];

        Board() {
            memset(board, ' ', sizeof(char) * 3 * 3); // Initialize the board with spaces
        }

        void draw() {
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

        int placeToken(int playerNum, int position) {
            const int row = position / 3;
            const int col = position % 3;

            if (board[row][col] != ' ') return -1;

            const char tokenChar = (playerNum == 1) ? 'X' : 'O';
            board[row][col] = tokenChar;

            // If draw, returns 0
            // If player 1 wins, returns 1
            // If player 2 wins, returns 2
            return checkWin();
        }
    
    private:
        int checkWin() {
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

            return 0;
        }
};


int main() {
    Board b;
    int playerNum = 1, position;

    for(int i = 0; i < 100; i++) {
        b.draw();

        printf("Player %d's turn: \n", playerNum);

        // Get position
        printf("Position: ");
        scanf("%d", &position);

        const int moveResult = b.placeToken(playerNum, position - 1);
        if (moveResult == -1) {
            printf("Invalid position. Please try again...\n");
        } else if (moveResult > 0) {
            b.draw();
            printf("Player %d wins!\n", moveResult); // A player has won
            break;
        } else {
            playerNum = (playerNum == 1) ? 2 : 1; // Change player number
        }
    }

    return 0;
}