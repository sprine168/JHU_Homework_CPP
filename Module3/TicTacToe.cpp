#include <iostream>
#include <vector>


class TicTacToe {
private:
    const int ROWSIZE = 3;
    const int COLUMNSIZE = 3;

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
        for (int i = 0; i < ROWSIZE; i++) {
            for (int j = 0; j < COLUMNSIZE; j++) {
                std::cout << gameBoard[i][j];
                if (j < COLUMNSIZE - 1) {
                    std::cout << " | ";
                }
            } //endColumn;

            std::cout << "\n";
            if (i < ROWSIZE - 1) {
                std::cout << "----------" << std::endl;
            }
        } // endRow;

        std::cout << "-----------------------" << std::endl;
    }


    /**
     * @brief A win happens when a player is the first to get three O's or X's in a row (vertically, horizontally,
     *        and diagonally).
     */
    void checkWin() const {
    }


    /**
     *  @brief This function checks to see if the tic-tac-toe games ends in a draw, by checking if the board is full and
     *         can not accept any more moves.
     *  @return boolean if false, the board still has moves that can be made, and if true the board is full.
     */
    bool checkDraw() {
        std::cout << this->gameBoard[0][1] << std::endl;
        for (int i = 0; i < ROWSIZE; i++) {
            for (int j = 0; j < COLUMNSIZE; j++) {
                // Checking if there are still moves that can be made
                if (this->gameBoard[i][j] == ' ') {
                    return false;
                }
            }
        }
        return true;
    }


    /**
     * @brief checks to make sure the user input is valid for the gameboard (within the bounds of the board),
     *        and if a marker already exists in
     *        the cell the user is trying to access (rather a human or computer user).
     *
     * @param rowCoordinate    userInputted
     * @param columnCoordinate userInputted
     * @return void
     *
    */
    bool checkValidPlayerMove(const int rowCoordinate, const int columnCoordinate) const {
        if ((rowCoordinate < 0 || rowCoordinate > ROWSIZE) || (columnCoordinate < 0 || columnCoordinate > COLUMNSIZE)) {
            std::cout << "Move needs to be placed between 1 and 3 inclusively including 1 and 3" << std::endl;
            return false;
        }
        if (this->gameBoard[rowCoordinate][columnCoordinate] != ' ') {
            std::cout << "This cell already contains a: " << this->gameBoard[rowCoordinate][columnCoordinate];
            return false;
        }
        return true;
    }

    /**
     * @brief This function allows the computer to randomly select a cell between 0 and 2 for the rows and columns
     *        in the tic-tac-toe board.
     */
    void computerMove() {
        int rowCoordinate = rand() % 3;
        int columnCoordinate = rand() % 3;

        while (!checkValidPlayerMove(rowCoordinate, columnCoordinate)) {
            rowCoordinate = rand() % 3;
            columnCoordinate = rand() % 3;
        }
        this->gameBoard[rowCoordinate][columnCoordinate] = this->COMPUTERCHARACTER;
    }

public:
    // Initialize the game and display the name of the game Tic-Tac-Toe
    TicTacToe() {
        std::cout << " == Welcome to Tic-Tac-Toe == " << std::endl;
        std::cout << " >> Please enter q to quit << " << std::endl;
        std::cout << "------------------------------" << std::endl;
    }


    /**
     * @brief This function sets the human players move on the game board.
     *
     * @param rowCoordinate int (between 1 and 3)
     * @param columnCoordinate int (between 1 and 3)
     */
    void playerMove(int &rowCoordinate, int &columnCoordinate) {
        this->gameBoard[rowCoordinate][columnCoordinate] = this->PLAYERCHARACTER;

        int endGame = 0;

        // No more cells to occupy, the board is full
        if (checkDraw() ) {
            std::cout << "The game ends in a draw: " << std::endl;
            endGame = 1;
            std::exit(0);
        }

    }


    /**
     * This function provides the gameLoop where the game starts and continues to run the Tic-Tac-Toe game.
     */
    void gameLoop() {
        TicTacToe game;

        std::cout << "Please enter two numbers between 1 and 3 to play your mark in the row and column respectively." <<
                std::endl;
        std::cout << "The first number corresponds to the row, and the second number to the column to pinpoint the cell"
                << std::endl;

        int playerMoveRow;
        int playerMoveColumn;
        std::string userQuit;

        if (userQuit == "q") {
            std::cout << "The Tic-Tac-Toe game has ended " << std::endl;
            return;
        }

        /*
            The human Player makes their move
            The Board is displayed immediately after the human player's move
            The game needs to check if the game is a win or a draw
            The Game automatically plays the computer's move
            The Board is displayed immediately after the computer's move
            The game needs to check if the game is a win or a draw
        */
        int moveCount = 0;
        while (moveCount < 9) {
            game.checkDraw();
            game.checkWin();

            std::cout << "Please enter your two numbers " << std::endl;
            std::cin >> playerMoveRow >> playerMoveColumn;

            if (std::cin.fail()) {
                std::cout << " Please enter two numbers seperated by a space (0 1) " << std::endl;
                continue;
            }

            // Making the values for the user to be between 1 and 3.
            playerMoveRow -= 1;
            playerMoveColumn -= 1;

            if (!checkValidPlayerMove(playerMoveRow, playerMoveColumn)) {
                continue;
            }

            game.playerMove(playerMoveRow, playerMoveColumn);
            moveCount++;

            game.drawBoard();
            game.checkDraw();

            game.checkWin();

            game.computerMove();

            game.checkWin();


            game.drawBoard();
        }
    }
};


int main() {
    TicTacToe game;
    game.gameLoop();
    return 0;
}
