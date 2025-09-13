#include <iostream>
#include <vector>


class TicTacToe {
private:

    const std::string GAMEDRAW = "The game ends in a draw";
    const char PLAYERCHARACTER = 'X';
    const char COMPUTERCHARACTER = 'O';
    const char PLACEHOLDER = ' ';

    std::vector<std::vector<char>> gameBoard = {
        {PLACEHOLDER, PLACEHOLDER, PLACEHOLDER},
        {PLACEHOLDER, PLACEHOLDER, PLACEHOLDER},
        {PLACEHOLDER, PLACEHOLDER, PLACEHOLDER},
    };

    // Calculating the size of the gameboard one time at initialization of the game
    const int ROWSIZE = gameBoard.size();
    const int COLUMNSIZE = gameBoard[0].size();

    // // A board that can hold a 3x3 for 'O's and 'X's
    // char gameBoard[3][3] = {
    //
    // };

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

        // Horizontal wins [0][0], [0][1], [0][2] or [1][0], [1][1], [1][2] or [2][0], [2][1], [2][2]
        // Vertical wins   [0][0], [1][0], [2][0] or [0][1], [1][1], [2][1] or [0][2], [1][2], [2][2]
        // Diagonal wins   [0][0], [1][1], [2][2] or [0][2], [1][1], [2][0]

        // Checking for horizontal wins:
        for (int i = 0; i < ROWSIZE; i++) {
            for (int j = 0; j < COLUMNSIZE; j++) {

            }

        }

    }


    /**
     *  @brief This function checks to see if the tic-tac-toe games ends in a draw, by checking if the board is full and
     *         can not accept any more moves.
     *  @return boolean if false, the board still has moves that can be made, and if true the board is full.
     */
    bool checkDraw() const {
        for (int i = 0; i < ROWSIZE; i++) {
            for (int j = 0; j < COLUMNSIZE; j++) {
                // Checking if there are still moves that can be made
                if (gameBoard[i][j] == PLACEHOLDER) {
                    return false;
                }
            }
        }
        std::cout << GAMEDRAW << std::endl;
        std::exit(0);
    }


    /**
     * @brief checks to make sure the user input is valid for the gameboard (within the bounds of the board),
     *        and if a marker already exists in
     *        the cell the user is trying to access (rather a human or computer user).
     *
     *        A valid move by either the computer or the player is as follows:
     *          Between the bounds of the tic-tac-board 0-2 (player knows it as 1-3)
     *          Is there already an X or O in the position on the board.
     *
     * @param rowCoordinate    userInputted
     * @param columnCoordinate userInputted
     * @return boolean (true/false)
     *
    */
    bool checkInvalidMove(const int rowCoordinate, const int columnCoordinate) const {
        bool result = false;

        // Making sure the user inputs are within the bounds of the gameboard
        if (rowCoordinate < 0 || rowCoordinate >= ROWSIZE) {
            std::cout
                << "Please enter your first number between 1 and 3 to make place your move "
                << std::endl;
            result = true;
        }
        if (columnCoordinate < 0 || columnCoordinate >= COLUMNSIZE) {
            std::cout
                << "Please enter your second number between 1 and 3 to make place your move "
                << std::endl;
            result = true;
        }

        // Don't continue to the array if user input is out of bounds
        if (result) return result;

        // Making sure that the input isn't placing a move on a spot that has already been played
        if (gameBoard[rowCoordinate][columnCoordinate] == PLAYERCHARACTER) {
            std::cout << " You already placed an " << PLAYERCHARACTER << " at " << rowCoordinate
                    << ", " << columnCoordinate << std::endl;
            result = true;
        }
        if (gameBoard[rowCoordinate][columnCoordinate] == COMPUTERCHARACTER) {
            std::cout << " The computer already placed an " << COMPUTERCHARACTER << " at " << rowCoordinate
                    << ", " << columnCoordinate << std::endl;
            result = true;
        }
        return result;
    }


    /**
     * @brief This function allows the computer to randomly select a cell between 0 and 2 for the rows and columns
     *        in the tic-tac-toe board.
     */
    void computerMove() {
        int rowCoordinate = rand() % 3;
        int columnCoordinate = rand() % 3;

        while (checkInvalidMove(rowCoordinate, columnCoordinate)) {
            rowCoordinate = rand() % 3;
            columnCoordinate = rand() % 3;
        }
        gameBoard[rowCoordinate][columnCoordinate] = COMPUTERCHARACTER;
        drawBoard();

        checkWin();
        checkDraw();
    }

public:
    // Initialize and start the Tic-Tac-Toe game and display the name of the game Tic-Tac-Toe
    TicTacToe() {
        std::cout << " == Welcome to Tic-Tac-Toe == " << std::endl;
        std::cout << " >> Please enter q to quit << " << std::endl;
        std::cout << "------------------------------" << std::endl;
        gameLoop();
    }


    /**
     * @brief This function sets the human players move on the game board.
     *
     * @param rowCoordinate int (between 1 and 3)
     * @param columnCoordinate int (between 1 and 3)
     */
    void playerMove(const int rowCoordinate, const int columnCoordinate) {
        gameBoard[rowCoordinate][columnCoordinate] = PLAYERCHARACTER;
        drawBoard();

        checkDraw();
        checkWin();
    }


    /**
     * This function provides the gameLoop where the game starts and continues to run the Tic-Tac-Toe game.
     */
    void gameLoop() {
        std::cout << "Please enter two numbers between 1 and 3 to play your mark in the row and column respectively."
                << std::endl;
        std::cout << "The first number corresponds to the row, and the second number to the column to pinpoint the cell"
                << std::endl;

        std::string userQuit;

        while (true) {
            int playerMoveRow;
            int playerMoveColumn;

            if (userQuit == "q") {
                std::cout << "The Tic-Tac-Toe game has ended " << std::endl;
                break;
            }

            // Checking the computers move here
            checkWin();

            std::cout << "Please enter your two numbers " << std::endl;
            std::cin >> playerMoveRow >> playerMoveColumn;

            if (std::cin.fail()) {
                std::cout << " Please enter two numbers seperated by a space (0 1) " << std::endl;
                continue;
            }

            // Making the values for the user to be between 1 and 3.
            playerMoveRow -= 1;
            playerMoveColumn -= 1;

            if (checkInvalidMove(playerMoveRow, playerMoveColumn)) {
                continue;
            }

            playerMove(playerMoveRow, playerMoveColumn);
            computerMove();
        }
    }
};


int main() {
    TicTacToe game;
    std::cout << "Thank you for playing " << std::endl;
    return 0;
}
