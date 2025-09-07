
#include <iostream>

/**
 * @Brief DesignChoices:
 *  A 3x3 game board to be able to place markers at:
 *  Human Player gets an X
 *  Computer Player gets an O
 *
 *  The gameboard shall take 1 to 3 for the coordinates to be tested with.
 *      I simply modify user input to match the starting index at 0. When a user inputs 1, it is modified to 0 and if a
 *      user inputs 3 it is simply subtracted to 2.
 *
 *  Check if the player has won after each match
 *  Check if the game is a draw, where the board is completely filled out with no winner.*
 *  Check if the move is valid (no existing piece found at the position the player places their mark).
 *
 */
class TicTacToe {
private:
    // A board that can hold a 3x3 for 'O's and 'X's
    char gameBoard[3][3] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '},
    };

    const char PLAYERCHARACTER = 'X';
    const char COMPUTERCHARACTER = 'O';

    /**
     * @brief This function is used to draw the gameboard with the marks that have been placed
     *        on the grid. When the player (computer or human) makes a play, the game board will be drawn.
     *        It always uses the gameBoard variable and updates using setters.
     */
    void drawBoard() const {
        // Drawing the rows and then the following loop draws the columns
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                std::cout << gameBoard[i][j];
                if (j < 2) {
                    std::cout << " | ";
                }
            } //endColumn;

            std::cout << "\n";
            if (i < 2) {
                std::cout << "----------" << std::endl;
            }
        } // endRow;

        std::cout << "-----------------------" << std::endl;
    }

    /**
     * @brief This function allows the user and computer to make moves and update the board with
     *        their mark.
     *
     * @param rowCoordinate integer that exists between 0 and 2
     * @param columnCoordinate integer that exists between 0 and 2
     * @param character The char that the player uses to mark the board for their play
     *
     */
    void updateBoard(int rowCoordinate, int columnCoordinate, char character) {
    }

    /**
     * @brief A win happens when a player is the first to get three O's or X's in a row (vertically, horizontally,
     *        and diagonally).
     */
    void checkWin() const {
    }

    /**
     * @brief A draw happens where all cells are completely filled in.
     *        This method is called after each turn.
     */
    void checkDraw() const {
    }

    /**
     * @brief checks to make sure the user input is valid for the gameboard, and if a marker already exists in
     *        the cell the user is trying to access (rather a human or computer user).
     *
     * @param rowCoordinate
     * @param columnCoordinate
     * @return void
     */
    bool checkValidMove(int rowCoordinate, int columnCoordinate) const {
        if ((rowCoordinate < 0 || rowCoordinate > 2) || (columnCoordinate < 0 || columnCoordinate > 2)) {
            std::cout << "Invalid move has occurred" << std::endl;
            return false;
        }
        if (this->gameBoard[rowCoordinate][columnCoordinate] != ' ') {
            if (this->gameBoard[rowCoordinate][columnCoordinate] == this->PLAYERCHARACTER)
                std::cout << "An " << this->PLAYERCHARACTER << " has already been placed here\n" << std::endl;

            else if (this->gameBoard[rowCoordinate][columnCoordinate] == this->COMPUTERCHARACTER)
                std::cout << "An " << this->COMPUTERCHARACTER << " has already been placed here\n" << std::endl;

            return false;
        }
        return true;
    }

    void computerMove() {
    }

public
:
    // Initialize the game and display the name of the game Tic-Tac-Toe
    TicTacToe() {
        std::cout << "  Tic-Tac-Toe " << std::endl;
        std::cout << "=======================" << std::endl;
    }

    /**
     * @brief This function sets the human players move on the game board.
     *
     * @param rowCoordinate int (between 1 and 3)
     * @param columnCoordinate int (between 1 and 3)
     */
    void playerMove(int rowCoordinate, int columnCoordinate) {
        rowCoordinate = rowCoordinate - 1;
        columnCoordinate = columnCoordinate - 1;
        if (!checkValidMove(rowCoordinate, columnCoordinate)) {
            return;
        }
        this->gameBoard[rowCoordinate][columnCoordinate] = this->PLAYERCHARACTER;
        drawBoard();
    }
};


int main() {
    TicTacToe game;

    game.playerMove(1, 3);
    game.playerMove(1, 3);
    game.playerMove(1, 2);
    game.playerMove(1, 1);

    // game.playerMove(1, 1);
    // game.playerMove(1, 2);

    return 0;
}
